//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_CLIENTCONNECTION_H
#define FRPCWEBUI_CLIENTCONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace xc {
    namespace httpserver {

        class ClientConnection {
        public:
            ClientConnection(int sockFd, struct sockaddr_in address);
            void workAndDestroy();
        private:
            int sockFd;
            struct sockaddr_in address;
            void workLoop();
        };

    } // xc
} // httpserver

#endif //FRPCWEBUI_CLIENTCONNECTION_H
