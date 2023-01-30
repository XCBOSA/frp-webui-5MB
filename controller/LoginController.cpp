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

    ResponseData *LoginController(RequestData request) {
        return new TemplateResponseData({
            Framework7Document({
                FormView({
                    FormInputView("userName", "用户名", "text", "输入您的用户名"),
                    FormInputView("password", "密码", "password", "输入您的密码")
                }),
                BlockView({
                    BlockColumnView({
                        ButtonView("登陆"),
                        VerticalSpacer(10),
                        p("如果您需要注册，请联系管理员")
                    })
                })
                }, {
                a("2023 © Frp-WebUI by XCBOSA")
                    .classAdd("link")
                    .onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
            })
        });
    }

    ContentGeneratorDefineWithNameS("LoginController", false, LoginController(request))

}
