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

    ResponseData *EntryController(RequestData request) {
        bool isLogin = false;
        return new TemplateResponseData({
            If(isLogin, {
                Framework7Document({
                    p("登陆成功")
                }, {
                    a("2023 © Frp-WebUI by XCBOSA")
                        .classAdd("link")
                        .onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
                })
            }, {
                ContentGeneratorReference("LoginController", request)
            })
        });
    }

    ContentGeneratorDefineS(request.getURL() == "/" || request.getURL().length() == 0, EntryController(request))

}
