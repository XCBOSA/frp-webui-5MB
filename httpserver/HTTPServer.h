//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "httpserver-private.h"

using namespace std;

namespace xc {
    namespace httpserver {

        class HTTPServer {
        public:
            HTTPServer(ushort bindPort);
            void serverLoop();
        private:
            int serverSocketFd;
            ushort bindPort;
        };

    } // xc
} // httpserver
