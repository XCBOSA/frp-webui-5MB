//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include <vector>
#include <map>
#include <sys/stat.h>
#include "webui.h"
#include "utils/utils.h"

using namespace std;
using namespace xc::utils;

namespace xc::conf {
    const int clientSocketTimeoutSeconds = 3;
    const int taskProcessTimeoutSeconds = 1;
    const int mtu = 1536;
    const bool enableStaticAssetsController = false;

    const string title("Frp-WebUI-XCBOSA");

    const string userPasswordSalt("eDGJ&v,.W0U(66.lVQFsKfWb*bm*M+Lj");
    const string userJWTSecret("r)xB=P-6A4dpqXLk%03=f+*8TlXDM@%r");
    const string userDataDir = "data/users";

    inline string getUserDataDir() {
        struct stat buffer;
        if (stat(userDataDir.c_str(), &buffer) == 0) {
            assertx(!S_ISREG(buffer.st_mode), "xc::conf::userDataDir must be configured to a folder, but a file located there.");
        }
        mkdir(userDataDir.c_str(), S_IRWXU);
        return userDataDir;
    }

    const map<string, string> fileExtensionToMimeTypes = {
            { ".html", "text/html" },
            { ".htm", "text/html" },
            { ".js", "text/javascript" },
            { ".ts", "text/typescript" },
            { ".xcnb", "text/xc-notebook" },
            { ".ccdproj", "application/c-code-develop-project" },
            { ".png", "image/png" },
            { ".jpg", "image/jpeg" },
            { ".jpeg", "image/jpeg" },
            { ".tiff", "image/tiff" },
            { ".css", "text/css" },
            { ".less", "text/css" },
            { ".scss", "text/css" },
            { ".svg", "image/svg+xml" },
            { ".ttf", "application/x-font-ttf" },
            { ".ttc", "application/x-font-ttf" },
            { ".woff", "application/x-font-woff" },
            { ".woff2", "application/x-font-woff" },
            { "default", "application/octet-stream" },
    };

    const vector<string> defaultFiles = {
            "index.html",
            "index.htm"
    };

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

    const auto errorPageTimeout = errorPage.applyReplacements(550, {
        Replacement("errorMessage", "服务器任务处理已超时，可能服务器访问量过大，请稍后重试"),
        Replacement("errorCode", "550")
    });
}
