//
// Created by xcbosa on 2023/1/31.
//

#pragma once

#include <string>
#include "utils/utils.h"
#include "webuiconf.h"
#include "fs.hpp"
#include <signal.h>
#include <strstream>

using namespace std;
using namespace xc;
using namespace xc::utils;

namespace frp {

    static set<int> profileUsingPorts(string profile) {
        set<int> usingPorts;
        INIFile ini(conf::getFrpcDir() + "/" + profile);
        string value = ini.getMust("common")->get("webui_allowServerPorts");
        auto v = split(value, ",");
        int low(0), len(0);
        if (v.size() == 2) {
            try {
                low = stoi(v[0]);
                len = stoi(v[1]);
            } catch (...) { }
        }
        for (int i = low; i < low + len; i++) {
            usingPorts.insert(i);
        }
        return usingPorts;
    }

    static set<int> serverUsingPorts(string serverIp) {
        set<int> usingPorts;
        for (string file : fs::contentsOfDirectory(conf::getFrpcDir())) {
            INIFile ini(conf::getFrpcDir() + "/" + file);
            if (ini.getMust("common")->get("server_addr") == serverIp) {
                auto profilePorts = profileUsingPorts(file);
                for (auto port : profilePorts) {
                    usingPorts.insert(port);
                }
            }
        }
        return usingPorts;
    }

    static void addProfile(string name, string ip, string port, string token) {
        INI ini;
        auto frpCommon = ini.getMust("common");
        frpCommon->set("server_addr", ip);
        frpCommon->set("server_port", port);
        frpCommon->set("token", token);
        frpCommon->set("tls_enable", "true");
        auto alreadyUsingPorts = serverUsingPorts(ip);
        int low = 0, len = conf::allowPortCountPerProfile;
        for (int i = 10000; i < 60000; i += len) {
            bool badPage = false;
            for (int j = i; j < i + len; j++) {
                if (alreadyUsingPorts.count(j)) {
                    badPage = true;
                    break;
                }
            }
            if (!badPage) {
                low = i;
                break;
            }
        }
        if (low == 0) {
            len = 0;
        }
        ostringstream oss;
        oss << low;
        oss << ",";
        oss << len;
        frpCommon->set("webui_allowServerPorts", oss.str());
        saveTextFile(conf::getFrpcDir() + "/" + name, ini.getINIString());
    }

    class FrpProcessWrapper {
    public:
        FrpProcessWrapper(string fileName, string filePath) {
            this->fileName = fileName;
            this->filePath = filePath;
            this->updated = false;
        }

        void startAndKeepRunning() {
            cout << "[FrpProcessWrapper] [" << fileName << "] Start" << endl;
            this->doKill();
            this->doStart();
        }

        void update() {
            if (getRunningPid() == 0) {
                cout << "[FrpProcessWrapper] [" << fileName << "] Exit unexpectedly, restarting..." << endl;
                this->doStart();
            }
        }

        int getRunningPid() {
            ::FILE *psStdoutFd = popen("ps -ef", "r");
            ostringstream oss;
            while (true) {
                int ch = ::fgetc(psStdoutFd);
                if (ch == -1) break;
                oss << (char) ch;
            }
            ::pclose(psStdoutFd);
            string str = oss.str();
            auto lines = split(str, "\n");
            for (auto line : lines) {
                if (line.find("frpc -c") != line.npos && line.find(this->filePath) != line.npos) {
                    strstream ss;
                    ss << line;
                    string owner;
                    int pid;
                    ss >> owner;
                    ss >> pid;
                    return pid;
                }
            }
            return 0;
        }

        void stop() {
            cout << "[FrpProcessWrapper] [" << fileName << "] Stop" << endl;
            this->doKill();
        }

        void reloadConfig() {
            this->stop();
            this->startAndKeepRunning();
        }

        bool updated;
        string fileName;
        string filePath;
    private:
        void doStart() {
            ostringstream oss;
            oss << "frpc -c " << this->filePath << " &";
            string launchCmd = oss.str();
            ::system(launchCmd.c_str());
        }

        void doKill() {
            int pid = getRunningPid();
            if (pid > 0) {
                kill(pid, SIGKILL);
            }
        }
    };

    set<string> reloadConfigForFilePathRequests;
    mutex reloadConfigForFilePathRequestsLocker;

    static void reloadProfileFilePath(string filePath) {
        reloadConfigForFilePathRequestsLocker.lock();
        reloadConfigForFilePathRequests.insert(filePath);
        reloadConfigForFilePathRequestsLocker.unlock();
    }

    static void frpDaemon() {
        char readBuff[1024];
        vector<FrpProcessWrapper *> frpProcesses;
        while (true) {
            for (auto it : frpProcesses) {
                it->updated = false;
            }
            for (string file : fs::contentsOfDirectory(conf::getFrpcDir())) {
                string filePath = conf::getFrpcDir() + "/" + file;
                bool updated = false;
                for (auto process : frpProcesses) {
                    if (process->filePath == filePath) {
                        process->update();
                        process->updated = true;
                        updated = true;
                        break;
                    }
                }
                if (!updated) {
                    auto newProcess = new FrpProcessWrapper(file, filePath);
                    newProcess->startAndKeepRunning();
                    newProcess->updated = true;
                    frpProcesses.push_back(newProcess);
                }
            }
            for (auto it = frpProcesses.begin(); it != frpProcesses.end(); ) {
                FrpProcessWrapper *proc = *it;
                if (!proc->updated) {
                    proc->stop();
                    delete proc;
                    it = frpProcesses.erase(it);
                } else {
                    reloadConfigForFilePathRequestsLocker.lock();
                    if (reloadConfigForFilePathRequests.count(proc->filePath)) {
                        reloadConfigForFilePathRequests.erase(proc->filePath);
                        reloadConfigForFilePathRequestsLocker.unlock();
                        proc->reloadConfig();
                    } else {
                        reloadConfigForFilePathRequestsLocker.unlock();
                    }
                    it++;
                }
            }
            usleep(1000 * 1000);
        }
    }

}
