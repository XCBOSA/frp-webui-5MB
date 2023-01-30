//
// Created by xcbosa on 2023-01-30
//

#include "../processor/processor.h"
#include "../utils/utils.h"
#include "../webuiconf.h"

using namespace std;
using namespace xc::processor;
using namespace xc::utils;
using namespace xc::processor::templates;

namespace xc::controller {

    static string ControllerPath = "/index.js";

    static ResponseData *controllerResponse(RequestData request) {
        return new TextResponseData(200,
            string("let app = new Framework7({\n") + 
            string("    // App root element\n") + 
            string("    el: \'#app\',\n") + 
            string("    // App Name\n") + 
            string("    name: \'FRPCWebUI\',\n") + 
            string("    // App id\n") + 
            string("    id: \'org.forgetive.frpcwebui\',\n") + 
            string("    // Enable swipe panel\n") + 
            string("    panel: {\n") + 
            string("        swipe: true,\n") + 
            string("    },\n") + 
            string("    // Add default routes\n") + 
            string("    routes: [\n") + 
            string("        {\n") + 
            string("            path: \'/about/\',\n") + 
            string("            url: \'about.html\',\n") + 
            string("        },\n") + 
            string("    ],\n") + 
            string("    // ... other parameters\n") + 
            string("});\n")
        , mimeTypeOfFile(ControllerPath));
    }

    ContentGeneratorDefineS(request.getURL() == ControllerPath, controllerResponse(request))

}
