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
            char *requestBuff = (char *)::calloc(1, urlRequestBuffSize);
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

                while (bzero(requestBuff, urlRequestBuffSize), ::fgets(requestBuff, urlRequestBuffSize, clRead)) {
                    string line(requestBuff);

                    replace_all(line, "\r\n", "");
                    trim(line);
                    if (line.length() == 0) {
                        break; // TODO: POST Body not read.
                    }

                    auto lineParts = split(line, ":");
                    for (auto &it : lineParts) {
                        trim(it);
                    }

                    if (lineParts.size() == 2) {
                        headers[lineParts[0]] = lineParts[1];
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