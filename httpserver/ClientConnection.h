//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "httpserver-private.h"

using namespace std;

namespace xc {
    namespace httpserver {

        class ClientConnection {
        public:
            ClientConnection(int sockFd, struct sockaddr_in address);
            void workAndDestroy();
        private:
            int sockFd;
            struct sockaddr_in address;
            ::FILE *clRead;
            ::FILE *clWrite;
            char *requestBuff;
            void workLoop();
            void cleanUpAndDestroy();
        };

    } // xc
} // httpserver
