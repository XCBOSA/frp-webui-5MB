//
// Created by xcbosa on 2023/1/31.
//

#pragma once

#include <string>
#include <set>

using namespace std;
using namespace xc;
using namespace xc::utils;

namespace xc::frp {

    void frpDaemon();

    /*仅当用户没有更改，但用户要求强制重启时调用，FrpDaemon会自动检查文件变化并自动更新。*/
    void reloadProfileFilePath(string filePath);

    set<int> profileUsingPorts(string profile);
    set<int> serverUsingPorts(string serverIp);
    void addProfile(string name, string ip, string port, string token);

    class ProfilePortInfo {
    public:
        ProfilePortInfo();
        ProfilePortInfo(string localIp, int localPort, int remotePort);
        string localIp;
        int localPort;
        int remotePort;
        CONFIGOR_BIND(json::value, ProfilePortInfo, REQUIRED(localIp), REQUIRED(localPort), REQUIRED(remotePort))
    };

    class ProfileInfo {
    public:
        ProfileInfo();
        ProfileInfo(string profileName);
        string getServerAddr();
        int getAllowPortLow();
        int getAllowPortCount();
        void addPortInfo(ProfilePortInfo portInfo);
        void removePort(int remotePort);
        bool availableForUser(string name);
        void addUser(string name);
        void removeUser(string name);
        void save() const;
        CONFIGOR_BIND(json::value, ProfileInfo,
                      REQUIRED(users),
                      REQUIRED(profileName),
                      REQUIRED(serverAddr),
                      REQUIRED(serverPort),
                      REQUIRED(token),
                      REQUIRED(allowPortLow),
                      REQUIRED(allowPortCount),
                      REQUIRED(ports))
        vector<ProfilePortInfo> ports;
    private:
        set<string> users;
        string profileName;
        string serverAddr;
        int serverPort;
        string token;
        int allowPortLow;
        int allowPortCount;
    };

    vector<ProfileInfo> listUserAvailableProfiles(string user);

}
