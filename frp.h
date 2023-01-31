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

    set<int> profileUsingPorts(string profile);
    set<int> serverUsingPorts(string serverIp);
    void addProfile(string name, string ip, string port, string token);
    void reloadProfileFilePath(string filePath);
    void frpDaemon();

}
