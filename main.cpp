#include <iostream>
#include "webui.h"

#include "httpserver/http-server.h"
#include "utils/utils.h"

using namespace xc::httpserver;

int main() {
    std::cout << "Hello, World!" << std::endl;
    HTTPServer server(8192);
    thread serverThread([&server] {
        server.serverLoop();
    });
    serverThread.detach();
    while (true) ;
    return 0;
}
