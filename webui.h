//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "webuiconf.h"

using namespace std;

#define assertx(expr, message) \
    if (!(expr)) { std::cerr << message << std::endl; exit(-1); }

namespace xc {
    constexpr int urlRequestBuffSize = 65536;

    namespace utils {
        string contentsOfTextFile(string filePath);
    }
}
