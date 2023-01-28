#include <iostream>
#include <sys/stat.h>

#include "httpserver/http-server.h"
#include "utils/utils.h"
#include "processor/processor.h"

using namespace xc::httpserver;
using namespace xc::processor;

ContentGeneratorDefine({
    if (request.getMethod() != "GET") { return false; }
    struct stat buffer;
    string filePath = "html/" + request.getURL();
    return stat(filePath.c_str(), &buffer) == 0;
}, {
    string filePath = "html" + request.getURL();
    return new BinaryResponseData(200, filePath, mimeTypeOfFile(filePath));
})

ContentGeneratorDefine(return request.getURL() == "/test1", {
    return new TextResponseData(200, "test1 controller response");
})

ContentGeneratorDefine(return request.getURL() == "/test2", {
    return new TextResponseData(200, "test2 controller response");
})

int main() {
    std::cout << "Hello, World!" << std::endl;
    HTTPServer server(8192);
    thread([&server] { server.serverLoop(); }).detach();
    RequestProcessWorker worker;
    thread([&worker] { worker.workerLoop(); }).detach();
    while (true) usleep(1000);
}
