//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_WEBUICONF_H
#define FRPCWEBUI_WEBUICONF_H

#include <vector>
#include "utils/FileResponseData.h"

using namespace std;

namespace xc::conf {
    using namespace utils;

    const FileResponseData errorPage400(400, "html/error.html", "text/html", {
        Replacement("{{ERR_MSG}}", "请求格式错误，无法解析请求"),
        Replacement("{{ERR_CODE}}", "400")
    });

    const FileResponseData errorPage404(404, "html/error.html", "text/html", {
        Replacement("{{ERR_MSG}}", "不存在指定的资源"),
        Replacement("{{ERR_CODE}}", "404")
    });

    const FileResponseData errorPage500(500, "html/error.html", "text/html", {
        Replacement("{{ERR_MSG}}", "服务器内部错误，可能是服务器访问量过大，请稍后重试"),
        Replacement("{{ERR_CODE}}", "500")
    });
}

#endif //FRPCWEBUI_WEBUICONF_H
