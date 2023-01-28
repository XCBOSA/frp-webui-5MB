//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "RequestData.h"
#include "ResponseData.h"
#include "TextResponseData.h"
#include "FileResponseData.h"
#include "BinaryResponseData.h"

using namespace std;

namespace xc::utils {
    string contentsOfTextFile(string filePath);
    string mimeTypeOfFile(string filePath);
}
