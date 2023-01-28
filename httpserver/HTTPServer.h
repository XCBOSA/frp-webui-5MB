//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_HTTPSERVER_H
#define FRPCWEBUI_HTTPSERVER_H

#include "../webui.h"

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

#endif //FRPCWEBUI_HTTPSERVER_H
