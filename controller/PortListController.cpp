//
// Created by xcbosa on 2023/1/30.
//

#include "../processor/processor.h"
#include "../utils/utils.h"
#include "../webuiconf.h"
#include "../processor/templates/framework7/Framework7Document.hpp"
#include "../user.hpp"
#include "../frp.h"

using namespace std;
using namespace xc::processor;
using namespace xc::processor::templates;
using namespace xc::processor::templates::framework7;
using namespace xc::utils;
using namespace configor;

namespace xc::controller {

    double divide(double a, double b, double zeroIf) {
        if (b == 0) return zeroIf;
        return a / b;
    }

    ResponseData *PortListController(RequestData request) {
        string username = user::getTokenUserName(request.getCookie("Token"));
        auto profiles = frp::listUserAvailableProfiles(username);
        int totalPortCnt(0), usedPortCnt(0);
        for (auto profile : profiles) {
            totalPortCnt += profile.getAllowPortCount();
            usedPortCnt += profile.ports.size();
        }
        double guagePercent = 1 - usedPortCnt / (double)totalPortCnt;
        if (totalPortCnt == 0) {
            guagePercent = 0;
        }
        return new TemplateResponseData({
            Framework7Document({
                a("退出登陆").classAdd("link").onclick("window.location='/quitLogin'"),
                a("添加端口").classAdd("link").onclick("window.location='/createPort'")
            }, {
                BlockTitleView("总览"),
                BlockView({
                    h1("端口总览").style("text-align", "center"),
                    GuageView(to_string(usedPortCnt) + " / " + to_string(totalPortCnt) + "端口", "已开通", guagePercent, 300)
                }),
                Foreach(profiles, [] (frp::ProfileInfo profile) {
                    return View("", {
                        BlockTitleView(""), center({
                        templates::div({
                            templates::div({
                                templates::div({
                                    templates::div({
                                        profile.getServerAddr(),
                                        VerticalSpacer(10),
                                        small("您可用范围： " + to_string(profile.getAllowPortLow()) + " - " + to_string(profile.getAllowPortLow() + profile.getAllowPortCount() - 1)).style("opacity", "0.7"),
                                        GuageView(to_string(profile.ports.size()) + "/" + to_string(profile.getAllowPortCount()) + "端口",
                                                  "可在此服务器创建", divide(profile.getAllowPortCount() - profile.ports.size(), profile.getAllowPortCount(), 0), 200)
                                    }).classAdd("card-header text-color-white display-block"),
                                    Link("关闭").href("#").classAdd("link card-close card-opened-fade-in color-white").prop("style", "position: absolute; right: 15px; top: 15px")
                                }).classAdd("bg-class-red").style("height", "300px"),
                                templates::div({
                                    templates::div({
                                        templates::div({
                                            templates::div({
                                                templates::div({
                                                    "This is a simple card with pla"
                                                }).classAdd("card-content card-content-padding")
                                            }).classAdd("card")
                                        }).classAdd("page-content")
                                    }).classAdd("page")
                                }).classAdd("card-content-padding")
                            }).classAdd("card-content")
                        }).classAdd("card card-expandable")})
                    });
                })
            }, {
                a("2023 © Frp-WebUI by XCBOSA").classAdd("link").onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
            })
        });
    }

    ContentGeneratorDefineWithNameS("PortListController", false, PortListController(request))

    ResponseData *CreatePortController(RequestData request) {
        string username = user::getTokenUserName(request.getCookie("Token"));
        if (!username.empty()) {
            
        }
    }

    ContentGeneratorDefineWithNameS("/createPort", request.getURLPath() == "/createPort", CreatePortController(request))

}
