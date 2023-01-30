//
// Created by xcbosa on 2023/1/30.
//

#include "../processor/processor.h"
#include "../webuiconf.h"
#include "../processor/templates/framework7/Framework7Document.hpp"

using namespace std;
using namespace xc::processor;
using namespace xc::processor::templates;
using namespace xc::processor::templates::framework7;
using namespace configor;

namespace xc::controller {

    ResponseData *PortListController(RequestData request) {
        return new TemplateResponseData({
            Framework7Document({
                a("退出登陆").classAdd("link").onclick("window.location='/quitLogin'")
            }, {
                p("登陆成功")
            }, {
                a("2023 © Frp-WebUI by XCBOSA")
                .classAdd("link")
                .onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
            })
        });
    }

    ContentGeneratorDefineWithNameS("PortListController", false, PortListController(request))

}
