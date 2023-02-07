//
// Created by xcbosa on 2023-02-07
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
            string("function createGuage(elAppendix, valueText, description, value, size) {\n") + 
            string("    app.gauge.create({\n") + 
            string("        el: \'.gauge_template_\' + elAppendix,\n") + 
            string("        type: \'circle\',\n") + 
            string("        value: value,\n") + 
            string("        size: size,\n") + 
            string("        borderColor: \'#2196f3\',\n") + 
            string("        borderWidth: 10,\n") + 
            string("        valueText: valueText,\n") + 
            string("        valueFontSize: 41,\n") + 
            string("        valueTextColor: \'#2196f3\',\n") + 
            string("        labelText: description,\n") + 
            string("    })\n") + 
            string("}\n") + 
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
            string("}\n") + 
            string("\n") + 
            string("function isValidIP(ip) {\n") + 
            string("    let reg = /^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$/\n") + 
            string("    return reg.test(ip)\n") + 
            string("}\n") + 
            string("\n") + 
            string("function doModify(uuid) {\n") + 
            string("    let data = app.form.convertToData(\"#modifyForm_\" + uuid)\n") + 
            string("    if (!isValidIP(data.address)) {\n") + 
            string("        app.dialog.alert(\"IP地址格式错误，请重新填写。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    if (data.port < 0 && data.port >= 65536) {\n") + 
            string("        app.dialog.alert(\"内部端口号必须在0-65535范围内。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    window.location = \"/changePortSettings?v=\" + JSON.stringify(data)\n") + 
            string("    return\n") + 
            string("}\n") + 
            string("\n") + 
            string("function doCreate() {\n") + 
            string("    let data = app.form.convertToData(\"#createNewForm\")\n") + 
            string("    if (data.address == \"\") {\n") + 
            string("        app.dialog.alert(\"请填写内部IP地址。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    if (!isValidIP(data.address)) {\n") + 
            string("        app.dialog.alert(\"IP地址格式错误，请重新填写。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    if (data.port == \"\") {\n") + 
            string("        app.dialog.alert(\"请填写内部端口。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    if (data.port < 0 && data.port >= 65536) {\n") + 
            string("        app.dialog.alert(\"内部端口号必须在0-65535范围内。\")\n") + 
            string("        return\n") + 
            string("    }\n") + 
            string("    window.location = \"/createPort?v=\" + JSON.stringify(data)\n") + 
            string("    return\n") + 
            string("}\n") + 
            string("\n") + 
            string("function doDelete(uuid) {\n") + 
            string("    let req = {\n") + 
            string("        \"uuid\": uuid\n") + 
            string("    }\n") + 
            string("    window.location = \"/removePort?v=\" + JSON.stringify(req)\n") + 
            string("}\n")
        , mimeTypeOfFile(ControllerPath));
    }

    ContentGeneratorDefineS(request.getURLPath() == ControllerPath, controllerResponse(request))

}
