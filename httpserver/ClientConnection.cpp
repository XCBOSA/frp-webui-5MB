//
// Created by xcbosa on 2023/1/28.
//

#include "ClientConnection.h"

using namespace std;

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
                ptr = strtok_r(NULL, " ", &p);
                if (ptr != nullptr) {
                    url = string(ptr);
                }
            }

            if (method == "GET1") {

            } else if (method == "POST1") {

            } else {
                conf::errorPage.applyReplacements(400, {
                    Replacement("errorCode", "400"),
                    Replacement("errorMessage", "未知的协议 " + method)
                }).writeTo(clWrite);
                cleanUpAndDestroy();
                return;
            }

            ResponseData(200, string(requestBuff)).writeTo(clWrite);
            cleanUpAndDestroy();
            return;
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