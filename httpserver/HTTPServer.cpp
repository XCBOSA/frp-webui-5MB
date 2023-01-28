//
// Created by xcbosa on 2023/1/28.
//

#include "HTTPServer.h"
#include "ClientConnection.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace xc {
    namespace httpserver {

        HTTPServer::HTTPServer(ushort bindPort) {
            this->bindPort = bindPort;
        }

        void HTTPServer::serverLoop() {
            this->serverSocketFd = socket(PF_INET, SOCK_STREAM, 0);
            assertx(this->serverSocketFd != -1, "Socket init failed.");
            struct sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(this->bindPort);
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            bzero(&(serverAddress.sin_zero), 8);
            if (::bind(this->serverSocketFd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr)) == -1) {
                cerr << "Unable to bind port" << this->bindPort << endl;
                assertx(0, "");
            }
            if (listen(this->serverSocketFd, 5) == -1) {
                cerr << "Unable to bind port" << this->bindPort << endl;
                assertx(0, "");
            }
            cout << "Bind to" << this->bindPort << endl;
            while (true) {
                struct sockaddr_in clientAddress;
                socklen_t clientAddressSize = sizeof(clientAddress);
                int clientSocketFd = accept(this->serverSocketFd, (struct sockaddr*)&clientAddress, &clientAddressSize);
                auto client = new ClientConnection(clientSocketFd, clientAddress);
                client->workAndDestroy();
            }
        }

    } // xc
} // httpserver