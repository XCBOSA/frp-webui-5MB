//
// Created by xcbosa on 2023/1/30.
//

#include "../processor/processor.h"
#include "../webuiconf.h"
#include "../processor/templates/framework7/Framework7Document.hpp"
#include "../user.hpp"

using namespace std;
using namespace xc::processor;
using namespace xc::processor::templates;
using namespace xc::processor::templates::framework7;
using namespace configor;

namespace xc::controller {

    ResponseData *EntryController(RequestData request) {
        string cookieToken = request.getCookie("Token");
        bool isUserLogin = user::isLogin(cookieToken);
        auto resp = new TemplateResponseData({
            If(isUserLogin, {
                ContentGeneratorReference("PortListController", request)
            }, {
                ContentGeneratorReference("LoginController", request)
            })
        });
        if (!isUserLogin) {
            resp->addCookie("Token", "");
        }
        resp->addCookie("message", "");
        return resp;
    }

    ContentGeneratorDefineS(request.getURLPath() == "/", EntryController(request))

}
