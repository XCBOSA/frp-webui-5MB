//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "RequestData.h"
#include "ResponseData.h"
#include "TextResponseData.h"
#include "FileResponseData.h"
#include "BinaryResponseData.h"
#include "INI.h"
#include "RedirectResponse.h"

using namespace std;

namespace xc::utils {
    string contentsOfTextFile(string filePath);
    void saveTextFile(string filePath, string content);
    string mimeTypeOfFile(string filePath);
}
