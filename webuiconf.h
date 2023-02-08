//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include <vector>
#include <map>
#include <sys/stat.h>
#include "utils/utils.h"
#include "processor/processor.h"

using namespace std;
using namespace xc::utils;
using namespace xc::processor;
using namespace xc::processor::templates;

namespace xc::conf {
    const int clientSocketTimeoutSeconds = 3;
    const int taskProcessTimeoutSeconds = 1;
    const int mtu = 1536;
    const bool enableStaticAssetsController = false;

    const string natMachineIPPrefix = "10.";
    const int natMachineLocalhostPortBegin = 50000;
    const int natMachineLocalhostPortSize = 10000;

    static queue<int> __natMachineLocalhostPortFreeList;
    static int __natMachineLocalhostPortAllocator = natMachineLocalhostPortBegin;

    /*分配NAT映射端口*/
    static int natMachineLocalhostPortAllocate() {
        if (__natMachineLocalhostPortFreeList.empty()) {
            int allocated = __natMachineLocalhostPortAllocator++;
            assert(allocated < natMachineLocalhostPortBegin + natMachineLocalhostPortSize);
        }
        int port = __natMachineLocalhostPortFreeList.front();
        return port;
    }

    /*释放NAT映射端口*/
    static int natMachineLocalhostPortFree(int port) {
        __natMachineLocalhostPortFreeList.push(port);
    }

    const string title("Frp-WebUI-XCBOSA");
    const int allowPortCountPerProfile(10);

    const string userPasswordSalt("eDGJ&v,.W0U(66.lVQFsKfWb*bm*M+Lj");
    const string userJWTSecret("r)xB=P-6A4dpqXLk%03=f+*8TlXDM@%r");
    const string rootDir = "/etc/frpcwebui";
    const int userTokenExpireSeconds = 60 * 60 * 24;
    const vector<string> supportTypes = { "tcp", "udp" };

    inline string getDirAndMakesureExists(string dirPath) {
        struct stat buffer;
        if (stat(dirPath.c_str(), &buffer) == 0) {
            assert(!S_ISREG(buffer.st_mode));
            return dirPath;
        }
        mode_t old_mask = umask(0);
        int n_ret = mkdir(dirPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
        umask(old_mask);
        if (n_ret != 0) {
            ::perror("");
            assert(n_ret == 0);
        }
        return dirPath;
    }

    inline string getRootDir() {
        return getDirAndMakesureExists(rootDir);
    }

    inline string getUserDataDir() {
        return getDirAndMakesureExists(getRootDir() + "/users");
    }

    inline string getFrpcDir() {
        return getDirAndMakesureExists(getRootDir() + "/frpc");
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

    class ErrorView: public View {
    public:
        ErrorView(int errorCode, string errorMessage): View("") {
            ostringstream oss;
            oss << "Error ";
            oss << errorMessage;
            oss << " ";
            oss << errorCode;
            html html({
                head({
                    meta().charset("UTF-8"),
                    title(oss.str())
                }),
                body({
                    p(oss.str())
                    .style("text-align", "center")
                    .pointer(&this->messageView),
                    p("FRPC-WebUI / XCHttpServer 1.0")
                    .style("text-align", "center")
                })
            });
            this->inner(html);
        }

        void setMessage(string text) {
            this->messageView->inner(text);
        }
    private:
        View *messageView;
    };

    const TemplateResponseData errorPage400(400, { ErrorView(400, "请求格式错误，无法解析请求") });

    const TemplateResponseData errorPage404(400, { ErrorView(404, "不存在指定的资源") });

    const TemplateResponseData errorPage500(400, { ErrorView(500, "服务器内部错误，可能是服务器访问量过大，请稍后重试") });

    const TemplateResponseData errorPageTimeout(550, { ErrorView(550, "服务器任务处理已超时，可能服务器访问量过大，请稍后重试") });

}
