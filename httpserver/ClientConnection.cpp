//
// Created by xcbosa on 2023/1/28.
//

#include "ClientConnection.h"
#include "../processor/processor.h"

using namespace std;
using namespace xc::processor;

namespace xc {
    namespace httpserver {

        ClientConnection::ClientConnection(int sockFd, struct sockaddr_in address) {
            this->sockFd = sockFd;
            this->address = address;
            this->clRead = nullptr;
            this->clWrite = nullptr;
            this->requestBuff = nullptr;
        }

        void ClientConnection::workAndDestroy() {
            thread(&ClientConnection::workLoop, this).detach();
        }

        void ClientConnection::workLoop() {
            ::FILE *clRead = fdopen(this->sockFd, "r");
            ::FILE *clWrite = fdopen(dup(this->sockFd), "w");

            struct timeval timeout = { conf::clientSocketTimeoutSeconds, 0 };
            if (setsockopt(this->sockFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
                cleanUpAndDestroy();
                return;
            }

            this->clRead = clRead;
            this->clWrite = clWrite;
            char *requestBuff = (char *)::malloc(urlRequestBuffSize);
            this->requestBuff = requestBuff;

            ::fgets(requestBuff, urlRequestBuffSize, clRead);

            if (::strstr(requestBuff, "HTTP/") == NULL) {
                conf::errorPage400.writeTo(clWrite);
                cleanUpAndDestroy();
                return;
            }

            string method, url;

            char *ptr, *p;
            ptr = strtok_r(requestBuff, " ", &p);
            if (ptr != nullptr) {
                method = string(ptr);
                ptr = strtok_r(p, " ", &p);
                if (ptr != nullptr) {
                    url = string(ptr);
                }
            }

            if (method == "GET" || method == "POST") {
                map<string, string> headers;
                ostringstream body;

                bool isHeader = true;
                bool lastLineIsEmpty = false;
                bool lastLineEmptyAndZero = false;
                while (::fgets(requestBuff, urlRequestBuffSize, clRead)) {
                    int len = ::strlen(requestBuff);
                    char *lineBuff = (char *) ::malloc(len + 1), *pLineBuff = lineBuff;
                    bool leftHasContext = false;
                    for (int i = 0; i < len; i++) {
                        if (requestBuff[i] == '\r' || requestBuff[i] == '\n') continue;
                        if (requestBuff[i] == ' ' || requestBuff[i] == '\t') {
                            if (leftHasContext) {
                                *pLineBuff++ = requestBuff[i];
                            }
                        } else {
                            leftHasContext = true;
                            *pLineBuff++ = requestBuff[i];
                        }
                    }
                    if (leftHasContext) {
                        pLineBuff--;
                        while (pLineBuff >= lineBuff && (*pLineBuff == ' ' || *pLineBuff == '\t')) {
                            *pLineBuff = 0; // 从右往左，删掉右边的空白符
                            pLineBuff--;
                        }
                    }
                    if (::strlen(lineBuff) == 0) {
                        // TODO: Fix Linux POST Method
//                        isHeader = false;
//                        lastLineIsEmpty = true;
//                        if (lastLineEmptyAndZero) {
//                            break;
//                        } else {
//                            lastLineEmptyAndZero = false;
//                        }
//                        if (method == "GET") {
                            break;
//                        }
                        continue;
                    }
                    if (lineBuff[0] == '0') {
                        if (lastLineIsEmpty) {
                            lastLineEmptyAndZero = true;
                            lastLineIsEmpty = false;
                        }
                    }
                    if (isHeader) {
                        char *headerValue;
                        char *headerName = strtok_r(lineBuff, ":", &headerValue);
                        if (headerName != nullptr && headerValue != nullptr) {
                            string name(headerName);
                            string value(headerValue);
                            headers[name] = value;
                        }
                    } else {
                        body << lineBuff << endl;
                    }
                }

                RequestData requestData(url, method, headers, body.str());
                cout << "[HTTPServer] Received " << method << " Request URL = " << url << endl;
                if (url.find("..") != string::npos) {
                    conf::errorPage400.writeTo(clWrite);
                    cleanUpAndDestroy();
                    return;
                }
                RequestProcessTask *task = new RequestProcessTask(requestData);
                processor::enqueueTask(task);
                ::time_t start, now;
                ::time(&start);
                while (true) {
                    usleep(1000 * 10);
                    if (task->isFinish()) {
                        ResponseData *taskResponse = task->getResponse();
                        taskResponse->writeTo(clWrite);
//                        taskResponse->writeTo(stdout);
                        delete taskResponse;
                        break;
                    }
                    ::time(&now);
                    if (::difftime(now, start) > conf::taskProcessTimeoutSeconds) {
                        cout << "[HTTPServer-Warn] Task failed because time out" << endl;
                        conf::errorPageTimeout.writeTo(clWrite);
                        processor::deleteTask(task);
                        task->setHttpDiscarded(true);
                        break;
                    }
                }
                cleanUpAndDestroy();
                return;
            } else {
                conf::ErrorView view(400, "");
                view.setMessage("未知的协议 " + method);
                TemplateResponseData({ view }).writeTo(clWrite);
                cleanUpAndDestroy();
                return;
            }
        }

        void ClientConnection::cleanUpAndDestroy() {
            if (this->clRead) {
                ::fclose(this->clRead);
                this->clRead = nullptr;
            }
            if (this->clWrite) {
                ::fclose(this->clWrite);
                this->clWrite = nullptr;
            }
            if (this->requestBuff) {
                ::free(this->requestBuff);
                this->requestBuff = nullptr;
            }
            ::close(this->sockFd);
            delete this;
        }

    } // xc
} // httpserver