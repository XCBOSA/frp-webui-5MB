//
// Created by xcbosa on 2023-01-31
//

#include "../../processor/processor.h"
#include "../../utils/utils.h"
#include "../../webuiconf.h"

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
            string("    // Add default routes,\n") + 
            string("    autoDarkMode: true,\n") + 
            string("    routes: [\n") + 
            string("        {\n") + 
            string("            path: \'/about/\',\n") + 
            string("            url: \'about.html\',\n") + 
            string("        },\n") + 
            string("    ]\n") + 
            string("});\n") + 
            string("\n") + 
            string("function doLogin(salt) {\n") + 
            string("    let data = app.form.convertToData(\"#loginForm\")\n") + 
            string("    if (data.username.length == 0) {\n") + 
            string("        app.dialog.alert(\"请输入用户名\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    if (data.password.length == 0) {\n") + 
            string("        app.dialog.alert(\"请输入密码\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    data.password = sha256_digest(data.password + salt)\n") + 
            string("    window.location = \"/login?v=\" + JSON.stringify(data)\n") + 
            string("    return\n") + 
            string("}\n")
        , mimeTypeOfFile(ControllerPath));
    }

    ContentGeneratorDefineS(request.getURLPath() == ControllerPath, controllerResponse(request))

}
