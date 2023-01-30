//
// Created by xcbosa on 2023/1/30.
//

#pragma once

#include <sys/stat.h>
#include <string>
#include <time.h>
#include "utils/utils.h"
#include "webuiconf.h"
#include "fs.hpp"
#include "thirdparty/sha256.hpp"

using namespace std;
using namespace xc;
using namespace xc::utils;

namespace user {

    inline string generateToken(string payload) __attribute__((weak)) {
        ostringstream oss;
        oss << payload;
        oss << "/";
        ::time_t t;
        ::time(&t);
        oss << (t + conf::userTokenExpireSeconds);
        oss << "/";
        oss << sha256(oss.str());
        return oss.str();
    }

    inline string tryLogin(string username, string password) __attribute__((weak)) {
        string userInfoFile = conf::getUserDataDir() + "/" + username;
        if (fs::existsFile(userInfoFile)) {
            INIFile ini(userInfoFile);
            string iniPwd = ini.getMust("info")->get("password");
            if (iniPwd == password) {
                return generateToken(username);
            }
        }
        return "";
    }

    static vector<string> split(const string& str, const string& delim) {
        vector<string> res;
        if ("" == str) return res;
        char * strs = new char[str.length() + 1];
        strcpy(strs, str.c_str());
        char * d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());
        char *p = strtok(strs, d);
        while(p) {
            string s = p;
            res.push_back(s);
            p = strtok(NULL, d);
        }
        return res;
    }

    static std::string& trim(std::string &s) {
        if (s.empty()) { return s; }
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }

    inline string getTokenUserName(string token) __attribute__((weak)) {
        auto list = split(token, "/");
        if (list.size() != 3) {
            return "";
        }
        string username = list[0];
        string time = list[1];
        string hash = list[2];
        trim(username);
        trim(time);
        trim(hash);
        try {
            long timeSec = stol(time);
            ::time_t t;
            ::time(&t);
            if (t > timeSec) {
                return "";
            }
        }
        catch (...) {
            return "";
        }
        ostringstream oss;
        oss << username;
        oss << "/";
        oss << time;
        oss << "/";
        string rhash = sha256(oss.str());
        if (hash != rhash) {
            return "";
        }
        return username;
    }

    inline bool isLogin(string token) __attribute__((weak)) {
        return !getTokenUserName(token).empty();
    }

}
