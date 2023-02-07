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
#include <sstream>
#include <fstream>
#include <functional>
#include <assert.h>
#include <string.h>
#include <filesystem>

using namespace std;

namespace xc::utils {
    string contentsOfTextFile(string filePath);
}
