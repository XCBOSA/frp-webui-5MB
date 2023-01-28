//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_WEBUI_H
#define FRPCWEBUI_WEBUI_H

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
    constexpr int smallBuffSize = 32;

    namespace utils {
        string contentsOfTextFile(string filePath);
    }
}


#endif //FRPCWEBUI_WEBUI_H
