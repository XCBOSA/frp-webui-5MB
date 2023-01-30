//
// Created by xcbosa on 2023/1/30.
//

#include "../processor/processor.h"
#include "../webuiconf.h"
#include "../processor/templates/framework7/Framework7Document.hpp"
#include "../fs.hpp"
#include "../user.hpp"

using namespace std;
using namespace xc::processor;
using namespace xc::processor::templates;
using namespace xc::processor::templates::framework7;
using namespace configor;

namespace xc::controller {

    ResponseData *LoginController(RequestData request) {
        return new TemplateResponseData({
            Framework7Document({
                BlockTitleView("需要登陆"),
                FormView({
                    FormInputView("username", "用户名", "text", "输入您的用户名").id("username"),
                    FormInputView("password", "密码", "password", "输入您的密码").id("password"),
                    FormItemView({
                        BlockView({
                            ButtonView("登陆").onclick("doLogin('" + conf::userPasswordSalt + "')"),
                            VerticalSpacer(10),
                            Label("如果您需要注册，请联系管理员")
                        })
                    })
                }).action("/").method("get").id("loginForm"),
                }, {
                Link("2023 © Frp-WebUI by XCBOSA")
                    .classAdd("link")
                    .onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
            })
        });
    }

    ContentGeneratorDefineWithNameS("LoginController", false, LoginController(request))

    ResponseData *ValidAuthController(RequestData request) {
        string arg = request.getURLArgument("v");
        JsonModel model = json::parse(arg);
        string username = model["username"];
        string password = model["password"];
        string token = user::tryLogin(username, password);
//        TemplateResponseData *resp = new TemplateResponseData({
//            If(token.empty(), {
//                ContentGeneratorReference("LoginController", request)
//            }, {
//                ContentGeneratorReference("PortListController", request)
//            })
//        });
        auto resp = new RedirectResponse("/");
        resp->addCookie("Token", token);
        return resp;
    }

    ContentGeneratorDefineS(request.getURLPath() == "/login", ValidAuthController(request))

}
