//
// Created by xcbosa on 2023/1/28.
//

#include "ClientConnection.h"
#include "../webui.h"

namespace xc {
    namespace httpserver {

        ClientConnection::ClientConnection(int sockFd, struct sockaddr_in address) {
            this->sockFd = sockFd;
            this->address = address;
        }

        void ClientConnection::workAndDestroy() {
            thread(workLoop);
        }

        void ClientConnection::workLoop() {
            ::FILE *clRead = fdopen(this->sockFd, "r");
            ::FILE *clWrite = fdopen(dup(this->sockFd), "w");
            char smallBuf[smallBuffSize];
            ::fgets(smallBuf, smallBuffSize, clRead);
            if (::strstr(smallBuf, "HTTP/") == NULL) {
                
            }
        }

    } // xc
} // httpserver