#include <iostream>
#include "webui.h"

#include "httpserver/http-server.h"
#include "utils/utils.h"
#include "processor/processor.h"

using namespace xc::httpserver;
using namespace xc::processor;

int main() {
    std::cout << "Hello, World!" << std::endl;
    HTTPServer server(8192);
    thread serverThread([&server] {
        server.serverLoop();
    });
    serverThread.detach();
    RequestProcessWorker worker;
    thread workerThread([&worker] {
        worker.workerLoop();
    });
    while (true) ;
    return 0;
}
