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
        bool isUserLogin = user::isLogin(request.getCookie("Token"));
        auto data = new TemplateResponseData({
            If(isUserLogin, {
                ContentGeneratorReference("PortListController", request)
            }, {
                ContentGeneratorReference("LoginController", request)
            })
        });
        if (!isUserLogin) {
            data->addCookie("Token", "");
        }
        return data;
    }

    ContentGeneratorDefineS(request.getURLPath() == "/", EntryController(request))

}
