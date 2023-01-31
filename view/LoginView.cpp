//
// Created by xcbosa on 2023/1/29.
//

#include "../processor/processor.h"
#include "../processor/templates/framework7/Framework7Document.hpp"
#include "../webuiconf.h"
#include "view.h"

using namespace xc;
using namespace xc::utils;
using namespace xc::processor::templates;
using namespace xc::processor::templates::framework7;

namespace xc::view {

    View LoginView() {
        return templates::div({
            templates::div(conf::title).classAdd("login-screen-title"),
                form({
                    templates::div({
                        ul({
                            li({
                                templates::div({
                                    templates::div("用户名").classAdd("item-title item-label"),
                                    templates::div({
                                        input().type("text").name("username").placeholder("用户名")
                                    }).classAdd("item-input-wrap")
                                }).classAdd("item-inner")
                            }).classAdd("item-content item-input"),
                            li({
                                templates:: div({
                                    templates::div("密码").classAdd("item-title item-label"),
                                    templates::div({
                                        input().type("password").name("password").placeholder("密码")
                                    }).classAdd("item-input-wrap")
                                }).classAdd("item-inner")
                            }).classAdd("item-content item-input")
                        })
                    }).classAdd("list"),
                    templates::div({
                        ul({
                            li({
                                Link("登陆").classAdd("list-button")
                            })
                        }),
                        templates::div({
                            p(""),
                            p({
                                Link("Github开源地址")
                            })
                        }).classAdd("block-footer")
                    }).classAdd("list")
                })
            }).classAdd("page-content login-screen-content");
    }

}
