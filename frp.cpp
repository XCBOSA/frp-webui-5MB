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
#include "frp.h"
#include <iomanip>
#include <chrono>
#include <functional>
#include <random>

using namespace std;
using namespace xc;
using namespace xc::utils;

namespace xc::frp {

    set<int> profileUsingPorts(string profile) {
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

    set<int> serverUsingPorts(string serverIp) {
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

    void addProfile(string name, string ip, string port, string token) {
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
            struct stat buf;
            if (stat(this->filePath.c_str(), &buf) == 0) {
                if (this->mutationTime != buf.st_mtime) {
                    this->mutationTime = buf.st_mtime;
                    this->reloadConfig();
                    return;
                }
            }
            INIFile profileINI(this->filePath);
            bool shouldRunning = profileINI.data.size() > 1;
            if (getRunningPid() == 0) {
                if (shouldRunning) {
                    cout << "[FrpProcessWrapper] [" << fileName << "] Exit unexpectedly, restarting..." << endl;
                    this->doStart();
                }
            } else {
                if (!shouldRunning) {
                    this->doKill();
                }
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
            cout << "[FrpProcessWrapper] [" << fileName << "] Changes Detected, Reload..." << endl;
            this->doKill();
            this->doStart();
        }

        bool updated;
        string fileName;
        string filePath;
        long mutationTime;
    private:
        void doStart() {
            struct stat buf;
            if (stat(this->filePath.c_str(), &buf) == 0) {
                this->mutationTime = buf.st_mtime;
            }
            INIFile profileINI(this->filePath);
            if (profileINI.data.size() < 2) {
                return;
            }
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

    void reloadProfileFilePath(string filePath) {
        reloadConfigForFilePathRequestsLocker.lock();
        reloadConfigForFilePathRequests.insert(filePath);
        reloadConfigForFilePathRequestsLocker.unlock();
    }

    void frpDaemon() {
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

    static string create_uuid() {
        ostringstream stream;
        auto random_seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 seed_engine(random_seed);
        std::uniform_int_distribution<std::size_t> random_gen;
        std::size_t value = random_gen(seed_engine);
        stream << hex << value;
        return stream.str();
    }

    ProfilePortInfo::ProfilePortInfo() { }

    ProfilePortInfo::ProfilePortInfo(string localIp, int localPort, int remotePort):
        localIp(localIp), localPort(localPort), remotePort(remotePort), uuid(create_uuid()) { }

    ProfilePortInfo::ProfilePortInfo(string localIp, int localPort, int remotePort, string uuid):
        localIp(localIp), localPort(localPort), remotePort(remotePort), uuid(uuid) { }

    ProfileInfo::ProfileInfo() { }

    ProfileInfo::ProfileInfo(string profileName) {
        this->profileName = profileName;
        INIFile file(conf::getFrpcDir() + "/" + profileName);
        bool needSave = false;
        for (auto &it : file.data) {
            if (it.getTitle() == "common") {
                this->serverAddr = it.get("server_addr");
                this->serverPort = to_int(it.get("server_port"), 0);
                this->token = it.get("token");
                string users = it.get("webui_availableForUsers");
                for (auto it : split(users, "/")) {
                    trim(it);
                    if (!it.empty()) {
                        this->users.insert(it);
                    }
                }
                string allowPortStr = it.get("webui_allowServerPorts");
                auto list = split(allowPortStr, ",");
                if (list.size() == 2) {
                    this->allowPortLow = to_int(list[0], 0);
                    this->allowPortCount = to_int(list[1], 0);
                }
            } else {
                string type = it.get("type");
                string localIp = it.get("local_ip");
                bool success;
                int localPort = to_int(it.get("local_port"), success);
                if (!success || !is_in(localPort, 0, 65536)) { continue; }
                int remotePort = to_int(it.get("remote_port"), success);
                if (!success || !is_in(localPort, 0, 65536)) { continue; }
                string id = it.get("id");
                if (id.empty()) {
                    id = create_uuid();
                    needSave = true;
                }
                it.set("id", id);
                if (type == "tcp" || type == "udp") {
                    bool alreadyHave = false;
                    for (auto v : this->ports) {
                        if (v.remotePort == remotePort) {
                            alreadyHave = true;
                        }
                    }
                    if (alreadyHave) { continue; }
                    this->ports.push_back(ProfilePortInfo(localIp, localPort, remotePort, id));
                }
            }
        }
        if (needSave) {
            file.save();
        }
    }

    int ProfileInfo::getFirstFreeRemotePort() {
        set<int> usingPorts;
        for (auto port : this->ports) {
            usingPorts.insert(port.remotePort);
        }
        for (int i = this->allowPortLow; i < this->allowPortLow + this->allowPortCount; i++) {
            if (!usingPorts.count(i)) {
                return i;
            }
        }
        return -1;
    }

    int ProfileInfo::getFirstFreeRemotePort(int ifNoneThenReturn) {
        int value = getFirstFreeRemotePort();
        if (value == -1) return ifNoneThenReturn;
        return value;
    }

    vector<int> ProfileInfo::getFreeRemotePorts() {
        return getFreeRemotePorts(65536);
    }

    vector<int> ProfileInfo::getFreeRemotePorts(int maxCnt) {
        set<int> usingPorts;
        vector<int> res;
        for (auto port : this->ports) {
            usingPorts.insert(port.remotePort);
        }
        int cnt = 0;
        for (int i = this->allowPortLow; i < this->allowPortLow + this->allowPortCount; i++) {
            if (!usingPorts.count(i)) {
                res.push_back(i);
                cnt++;
                if (cnt >= maxCnt) {
                    break;
                }
            }
        }
        return res;
    }

    vector<int> ProfileInfo::getFreeRemotePortsAndAppend(int me) {
        vector<int> ports = getFreeRemotePorts();
        ports.push_back(me);
        std::sort(ports.begin(), ports.end());
        return ports;
    }

    void ProfileInfo::addPortInfo(ProfilePortInfo portInfo) {
        for (auto &port : this->ports) {
            if (port.remotePort == portInfo.remotePort) {
                port.localPort = portInfo.localPort;
                port.localIp = portInfo.localIp;
                return;
            }
        }
        this->ports.insert(this->ports.begin(), portInfo);
    }

    void ProfileInfo::removePort(int remotePort) {
        for (auto ptr = this->ports.begin(); ptr != this->ports.end(); ptr++) {
            if (ptr->remotePort == remotePort) {
                this->ports.erase(ptr);
                return;
            }
        }
    }

    string ProfileInfo::getServerAddr() { return this->serverAddr; }

    int ProfileInfo::getAllowPortLow() { return this->allowPortLow; }

    int ProfileInfo::getAllowPortCount() { return this->allowPortCount; }

    void ProfileInfo::save() const {
        string path = conf::getFrpcDir() + "/" + this->profileName;
        INIFile ini(path);
        ini.data.clear();
        ini.getMust("common")->set("server_addr", this->serverAddr);
        ini.getMust("common")->set("server_port", to_string(this->serverPort));
        ini.getMust("common")->set("token", this->token);
        ini.getMust("common")->set("tls_enable", "true");
        ostringstream usersOss;
        for (auto user : this->users) {
            usersOss << user;
            usersOss << "/";
        }
        ini.getMust("common")->set("webui_availableForUsers", usersOss.str());
        ostringstream oss;
        oss << this->allowPortLow << "," << this->allowPortCount;
        ini.getMust("common")->set("webui_allowServerPorts", oss.str());
        for (auto port : this->ports) {
            for (auto type : conf::supportTypes) {
                ostringstream titleOss;
                titleOss << "frpc-webui-" << port.remotePort << "-" << type;
                ini.getMust(titleOss.str())->set("type", type);
                ini.getMust(titleOss.str())->set("local_ip", port.localIp);
                ini.getMust(titleOss.str())->set("local_port", to_string(port.localPort));
                ini.getMust(titleOss.str())->set("remote_port", to_string(port.remotePort));
                ini.getMust(titleOss.str())->set("use_encryption", "true");
                ini.getMust(titleOss.str())->set("use_compression", "true");
            }
        }
        ini.save();
    }

    void ProfileInfo::addUser(string name) {
        this->users.insert(name);
    }

    void ProfileInfo::removeUser(string name) {
        this->users.erase(name);
    }

    bool ProfileInfo::availableForUser(string name) {
        return this->users.count(name) > 0;
    }

    vector<ProfileInfo> listUserAvailableProfiles(string user) {
        vector<ProfileInfo> profiles;
        for (auto profile : fs::contentsOfDirectory(conf::getFrpcDir())) {
            ProfileInfo profileInfo(profile);
            if (profileInfo.availableForUser(user)) {
                profiles.push_back(profileInfo);
            }
        }
        return profiles;
    }

    vector<string> listingAvailableServerAndPortForUser(string username) {
        auto profiles = listUserAvailableProfiles(username);
        vector<string> items;
        for (auto profile : profiles) {
            for (auto port : profile.getFreeRemotePorts()) {
                items.push_back(profile.getServerAddr() + ":" + to_string(port));
            }
        }
        return items;
    }

}
