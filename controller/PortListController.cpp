//
// Created by xcbosa on 2023/1/30.
//

#include "../processor/processor.h"
#include "../utils/utils.h"
#include "../webuiconf.h"
#include "../processor/templates/framework7/Framework7Document.hpp"
#include "../user.hpp"
#include "../frp.h"

#define RequireLogin(username) \
string username = user::getTokenUserName(request.getCookie("Token")); if (username.empty()) { return new RedirectResponse("/"); }

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
        RequireLogin(username)
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
        auto resp = new TemplateResponseData({
            Framework7Document({
                a("退出登陆").classAdd("link").onclick("window.location='/quitLogin'")
            }, {
                If(!request.getCookie("message").empty(), {
                    OnLoadScript("app.dialog.alert('" + request.getCookie("message") + "')")
                }),
                BlockTitleView("总览"),
                BlockView({
                    h1("端口总览").style("text-align", "center"),
                    GuageView(to_string(usedPortCnt) + " / " + to_string(totalPortCnt) + "端口", "已开通", guagePercent, 300)
                }),
                Foreach(profiles, [] (frp::ProfileInfo profile) {
                    return View("", {
                        BlockTitleView("IP地址 " + profile.getServerAddr() + " (端口" + to_string(profile.getAllowPortLow()) + "-" + to_string(profile.getAllowPortLow() + profile.getAllowPortCount() - 1) + ")"),
                        Foreach(profile.ports, [&profile] (frp::ProfilePortInfo portInfo) {
                            return View("", {
                                templates::div({
                                    templates::div({
                                        templates::div({
                                            templates::div({
                                                profile.getServerAddr() + ":" + to_string(portInfo.remotePort),
                                                VerticalSpacer(10),
                                                small({
                                                    "转发给： ",
                                                    portInfo.localIp,
                                                    ":",
                                                    to_string(portInfo.localPort)
                                                }).style("opacity", "0.7")
                                            }).classAdd("card-header text-color-white display-block"),
                                            Link("关闭").href("#").classAdd("link card-close card-opened-fade-in color-white").prop("style", "position: absolute; right: 15px; top: 15px")
                                        }).style("height", "300px").style("background-color", "#123456"),
                                        templates::div({
                                            FormView({
                                                input().name("uuid").value(portInfo.uuid).prop("hidden", "true"),
                                                FormInputView("address", "内部IP地址", "text", "例如：10.0.0.2", portInfo.localIp),
                                                FormInputView("port", "内部端口", "number", "例如：22 (SSH服务端口)", to_string(portInfo.localPort)),
                                                FormCustomInputView("外部端口 (在" + profile.getServerAddr() + "上)", {
                                                    templates::select({
                                                        Foreach(profile.getFreeRemotePortsAndAppend(portInfo.remotePort), [&portInfo] (int port) {
                                                            return If(portInfo.remotePort == port, {
                                                                View("option", to_string(port)).value(to_string(port)).prop("selected", "selected")
                                                            }, {
                                                                View("option", to_string(port)).value(to_string(port))
                                                            });
                                                        })
                                                    }).name("remotePort")
                                                })
                                            }).id("modifyForm_" + portInfo.uuid),
                                            VerticalSpacer(25),
                                            button("保存端口设置").classAdd("button button-fill").onclick("doModify('" + portInfo.uuid + "')"),
                                            VerticalSpacer(10),
                                            button("删除此条").classAdd("button button-fill color-red").onclick("doDelete('" + portInfo.uuid +"')")
                                        }).classAdd("card-content-padding")
                                    }).classAdd("card-content")
                                }).classAdd("card card-expandable"),
                            });
                        }),
                    });
                }),
                BlockView({
                    ButtonView("创建新的端口").classAdd("button-large button-fill").classAdd("sheet-open").prop("data-sheet", ".create-new")
                }),
                templates::div({
                    templates::div({
                        templates::div({
                            templates::div({
                                a("取消").classAdd("link sheet-close")
                            }).classAdd("left"),
                            templates::div({
                                p("创建端口")
                            }).classAdd("center"),
                            templates::div({
                                a("创建").classAdd("link").onclick("doCreate()")
                            }).classAdd("right")
                        }).classAdd("toolbar-inner")
                    }).classAdd("toolbar"),
                    templates::div({
                        templates::div({
                            BlockTitleView("端口信息"),
                            FormView({
                                FormInputView("address", "内部IP地址", "text", "例如：10.0.0.2"),
                                FormInputView("port", "内部端口", "number", "例如：22 (SSH服务端口)"),
                                FormCustomInputView("转发到", {
                                    templates::select({
                                        Foreach(frp::listingAvailableServerAndPortForUser(username), [] (string pair) {
                                            return View("option", pair);
                                        })
                                    }).name("remotePair")
                                })
                            }).id("createNewForm")
                        }).classAdd("page-content")
                    }).classAdd("sheet-modal-inner")
                }).classAdd("sheet-modal sheet-modal-push create-new")
            }, {
                a("2023 © Frp-WebUI by XCBOSA").classAdd("link").onclick("window.open('https://github.com/XCBOSA/frp-webui-500k.git')")
            })
        });
        return resp;
    }

    ResponseData *CreatePortController(RequestData request) {
        RequireLogin(username)
        string arg = request.getURLArgument("v");
        JsonModel model = json::parse(arg);
        string localAddress = model["address"];
        int localPort = to_int(model["port"], -1);
        string remotePair = model["remotePair"];
        auto remoteList = split(remotePair, ":");
        if (remoteList.size() != 2) {
            return new RedirectResponse("/");
        }
        string serverIp = remoteList[0];
        int serverPort = to_int(remoteList[1], -1);

        auto profiles = frp::listUserAvailableProfiles(username);
        for (auto &profile : profiles) {
            if (profile.getServerAddr() != serverIp) { continue; }
            if (!utils::is_in(serverPort, profile.getAllowPortLow(), profile.getAllowPortLow() + profile.getAllowPortCount())) { continue; }
            auto freePorts = profile.getFreeRemotePorts();
            if (std::count(freePorts.begin(), freePorts.end(), serverPort)) {
                profile.addPortInfo(frp::ProfilePortInfo(localAddress, localPort, serverPort));
                profile.save();
                auto resp = new RedirectResponse("/");
                resp->addCookie("message", "添加成功。");
                return resp;
            } else {
                auto resp = new RedirectResponse("/");
                resp->addCookie("message", "该端口已被占用，可能在您编辑期间，他人占用了这个端口，请重新选择。");
                return resp;
            }
        }

        auto resp = new RedirectResponse("/");
        resp->addCookie("message", "添加失败，您可能没有权限使用指定服务器和端口号段。");
        return resp;
    }

    ResponseData *ChangePortSettings(RequestData request) {
        RequireLogin(username)
        string arg = request.getURLArgument("v");
        JsonModel model = json::parse(arg);
        string uuid = model["uuid"];
        int remotePort = to_int(model["remotePort"], -1);
        string localAddress = model["address"];
        int localPort = to_int(model["port"], -1);

        auto profiles = frp::listUserAvailableProfiles(username);
        for (auto &profile : profiles) {
            for (auto &port : profile.ports) {
                if (port.uuid == uuid) {
                    auto allowPortsIncludedMe = profile.getFreeRemotePortsAndAppend(port.remotePort);
                    if (std::count(allowPortsIncludedMe.begin(), allowPortsIncludedMe.end(), remotePort)) {
                        port.remotePort = remotePort;
                        port.localPort = localPort;
                        port.localIp = localAddress;
                        profile.save();
                        auto resp = new RedirectResponse("/");
                        resp->addCookie("message", "修改成功");
                        return resp;
                    } else {
                        auto resp = new RedirectResponse("/");
                        resp->addCookie("message", "您指定的远程端口" + to_string(remotePort) + "并不在其所在的配置文件允许的端口范围中，或者正在使用。");
                        return resp;
                    }
                }
            }
        }

        auto resp = new RedirectResponse("/");
        resp->addCookie("message", "找不到您刚刚修改的端口配置，它可能在您编辑的时候被其他人删除了。");
        return resp;
    }

    ResponseData *RemovePort(RequestData request) {
        RequireLogin(username)

        string arg = request.getURLArgument("v");
        JsonModel model = json::parse(arg);
        string uuid = model["uuid"];

        auto profiles = frp::listUserAvailableProfiles(username);
        for (auto &profile : profiles) {
            for (auto port : profile.ports) {
                if (port.uuid == uuid) {
                    profile.removePort(port.remotePort);
                    profile.save();
                    auto resp = new RedirectResponse("/");
                    resp->addCookie("message", "删除成功。");
                    return resp;
                }
            }
        }

        auto resp = new RedirectResponse("/");
        resp->addCookie("message", "找不到您要删除的端口，它可能已经被删除。");
        return resp;
    }

    ContentGeneratorDefineWithNameS("PortListController", false, PortListController(request))
    ContentGeneratorDefineWithNameS("/createPort", request.getURLPath() == "/createPort", CreatePortController(request))
    ContentGeneratorDefineWithNameS("/changePortSettings", request.getURLPath() == "/changePortSettings", ChangePortSettings(request))
    ContentGeneratorDefineWithNameS("/removePort", request.getURLPath() == "/removePort", RemovePort(request))

}
