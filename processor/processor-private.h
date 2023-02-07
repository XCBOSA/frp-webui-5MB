//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include <vector>
#include <map>
#include <deque>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <functional>
#include <assert.h>
#include "../thirdparty/configor/json.hpp"

#include "../utils/utils.h"

using namespace std;
using namespace xc::utils;

namespace xc::processor {
    class ContentGenerator;
    extern ContentGenerator *generators[];
    extern int generatorsCnt;
}
