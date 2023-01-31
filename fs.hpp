#pragma once

#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include "utils/utils.h"

using namespace std;

namespace fs {

    inline bool existsNothing(string filePath) {
        struct stat buffer;
        return stat(filePath.c_str(), &buffer) != 0;
    }

    inline bool existsAnything(string filePath) {
        struct stat buffer;
        return stat(filePath.c_str(), &buffer) == 0;
    }

    inline bool existsFile(string filePath) {
        struct stat buffer;
        if (stat(filePath.c_str(), &buffer) == 0) {
            return S_ISREG(buffer.st_mode);
        }
        return false;
    }

    inline bool deleteFile(string filePath) {
        return ::remove(filePath.c_str()) == 0;
    }

    inline bool existsDirectory(string filePath) {
        struct stat buffer;
        if (stat(filePath.c_str(), &buffer) == 0) {
            return !S_ISREG(buffer.st_mode);
        }
        return false;
    }

    inline vector<string> contentsOfDirectory(string filePath) {
        DIR *pDir;
        struct dirent* ptr;
        vector<string> ret;
        if (!(pDir = opendir(filePath.c_str()))){
            return ret;
        }
        while ((ptr = readdir(pDir)) != 0) {
            if (::strlen(ptr->d_name) > 0) {
                if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0 && ptr->d_name[0] != '.') {
                    ret.push_back(ptr->d_name);
                }
            }
        }
        closedir(pDir);
        return ret;
    }

}
