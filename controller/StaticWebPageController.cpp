//
// Created by xcbosa on 2023/1/28.
//

#include "../processor/processor.h"
#include "../webuiconf.h"
#include <sys/stat.h>

using namespace std;
using namespace xc::processor;
using namespace xc::processor::templates;

using namespace configor;

namespace xc::controller {

    ContentGeneratorDefine({
                               if (request.getMethod() != "GET") { return false; }
                               if (request.getURL().length() == 0) { return false; }
                               struct stat buffer;
                               string filePath = "html";
                               if (request.getURL()[0] == '/') {
                                   filePath += request.getURL();
                               } else {
                                   filePath += "/" + request.getURL();
                               }
                               if (stat(filePath.c_str(), &buffer) == 0) {
                                   if (S_ISREG(buffer.st_mode)) {
                                       return true;
                                   } else {
                                       for (auto file: conf::defaultFiles) {
                                           string newFilePath = filePath;
                                           if (filePath[filePath.length() - 1] == '/') {
                                               newFilePath += file;
                                           } else {
                                               newFilePath += "/" + file;
                                           }
                                           if (stat(newFilePath.c_str(), &buffer) == 0) {
                                               return S_ISREG(buffer.st_mode);
                                           }
                                       }
                                   }
                               }
                               return false;
                           }, {
                               struct stat buffer;
                               string filePath = "html";
                               if (request.getURL()[0] == '/') {
                                   filePath += request.getURL();
                               } else {
                                   filePath += "/" + request.getURL();
                               }
                               if (stat(filePath.c_str(), &buffer) == 0) {
                                   if (S_ISREG(buffer.st_mode)) {
                                       return (ResponseData *) new BinaryResponseData(200, filePath,
                                                                                      mimeTypeOfFile(filePath));
                                   } else {
                                       for (auto file: conf::defaultFiles) {
                                           string newFilePath = filePath;
                                           if (filePath[filePath.length() - 1] == '/') {
                                               newFilePath += file;
                                           } else {
                                               newFilePath += "/" + file;
                                           }
                                           if (stat(newFilePath.c_str(), &buffer) == 0) {
                                               if (S_ISREG(buffer.st_mode)) {
                                                   return (ResponseData *) new BinaryResponseData(200, newFilePath,
                                                                                                  mimeTypeOfFile(
                                                                                                          newFilePath));
                                               }
                                           }
                                       }
                                   }
                               }

                               return (ResponseData *) new FileResponseData(conf::errorPage404);
                           })

    ResponseData *test1(RequestData request) {
        vector<map<string, string>> model = {
                {
                        { "name", "user1" },
                        { "pwd", "123456" }
                },
                {
                        { "name", "user2" },
                        { "pwd", "234567" }
                }
        };
        return new TemplateResponseData({
            p("Hello world"),
            Foreach(model, [](map<string, string> it) {
                return p({
                    "Hello ",
                    b(it["name"]),
                    ", You password is ",
                    it["pwd"]
                }).style("text-align", "center");
            })
        });
    }

    ContentGeneratorDefineS(request.getURL() == "/test1",
                            test1(request))

    ContentGeneratorDefineS(request.getURL() == "/test2",
                            new TextResponseData(200, "test2 controller response"))

}

