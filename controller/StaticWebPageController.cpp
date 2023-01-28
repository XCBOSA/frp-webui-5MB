//
// Created by xcbosa on 2023/1/28.
//

#include "../processor/processor.h"
#include "../utils/utils.h"
#include <sys/stat.h>

using namespace std;
using namespace xc::processor;

namespace xc::controller {

    static ContentGenerator staticWebPageController([] (auto p) {
        if (p.getMethod() != "GET") { return false; }
        struct stat buffer;
        string filePath = "html/" + p.getURL();
        return stat(filePath.c_str(), &buffer) == 0;
    }, [] (auto p) {
        string filePath = "html/" + p.getURL();
        return new BinaryResponseData(200, filePath, "");
    });

}

