#pragma once

#include <sys/stat.h>
#include <string>
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

    inline bool existsDirectory(string filePath) {
        struct stat buffer;
        if (stat(filePath.c_str(), &buffer) == 0) {
            return !S_ISREG(buffer.st_mode);
        }
        return false;
    }

}
