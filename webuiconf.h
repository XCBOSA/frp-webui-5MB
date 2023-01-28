//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include <vector>
#include "utils/utils.h"

using namespace std;
using namespace xc::utils;

namespace xc::conf {
    const int clientSocketTimeoutSeconds = 3;
    const int mtu = 1536;

    const IncompleteFileResponseData errorPage(FileResponseData(500, "html/error.html", "text/html"));

    const auto errorPage400 = errorPage.applyReplacements(400, {
        Replacement("errorMessage", "请求格式错误，无法解析请求"),
        Replacement("errorCode", "400")
    });

    const auto errorPage404 = errorPage.applyReplacements(404, {
        Replacement("errorMessage", "不存在指定的资源"),
        Replacement("errorCode", "404")
    });

    const auto errorPage500 = errorPage.applyReplacements(500, {
        Replacement("errorMessage", "服务器内部错误，可能是服务器访问量过大，请稍后重试"),
        Replacement("errorCode", "500")
    });
}
