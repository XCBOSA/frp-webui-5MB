#include <iostream>
#include <sys/stat.h>

#include "httpserver/http-server.h"
#include "utils/utils.h"
#include "processor/processor.h"

using namespace xc;
using namespace xc::httpserver;
using namespace xc::processor;

int main() {
    std::cout << "Hello, World!" << std::endl;
    HTTPServer server(8192);
    thread([&server] { server.serverLoop(); }).detach();
    RequestProcessWorker worker;
    thread([&worker] { worker.workerLoop(); }).detach();
    while (true) usleep(1000);
}
