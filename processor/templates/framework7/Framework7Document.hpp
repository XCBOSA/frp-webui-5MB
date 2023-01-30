//
// Created by xcbosa on 2023/1/29.
//

#ifndef FRPCWEBUI_FRAMEWORK7DOCUMENT_H
#define FRPCWEBUI_FRAMEWORK7DOCUMENT_H

#include "../../processor.h"

namespace xc::processor::templates::framework7 {
    class Framework7Document: public View {
    public:
        Framework7Document(ViewCollection pageContent, ViewCollection tabBarContent): View({ }) {
            html html({
                head({
                    meta().charset("utf-8"),
                    meta().name("viewport").content("width=device-width, initial-scale=1, maximum-scale=1, minimum-scale=1, user-scalable=no, viewport-fit=cover"),
                    meta().name("apple-mobile-web-app-capable").content("yes"),
                    meta().name("theme-color").content("#2196f3"),
                    templates::title(conf::title),
                    link().rel("stylesheet").href("framework7/framework7-bundle.min.css"),
                    link().rel("stylesheet").href("index.css")
                }),
                body({
                    div({
                        div({
                            div({
                                div().classAdd("navbar-bg"),
                                div({
                                    div(conf::title).classAdd("title")
                                }).classAdd("navbar-inner")
                            }).classAdd("navbar"),
                            div({
                                div(tabBarContent).classAdd("toolbar-inner")
                            }).classAdd("toolbar toolbar-bottom"),
                            div(pageContent).classAdd("page-content")
                        }).prop("data-name", "home").classAdd("page")
                    }).classAdd("view view-main")
                }).id("app"),
                script().type("text/javascript").src("framework7/framework7-bundle.min.js"),
                script().type("text/javascript").src("index.js"),
            });
            this->inner(html);
        }
    };

    class FormView: public form {
    public:
        FormView(ViewCollection items): form() {
            this->inner(items);
            this->classAdd("list");
        }
    };

    class FormItemView: public ul {
    public:
        FormItemView(ViewCollection content): ul(li(content)) { }
    };

    class FormInputView: public FormItemView {
    public:
        FormInputView(string keyName, string displayName, string contentType, string placeholder): FormItemView({
            div({
                div({
                    div(displayName).classAdd("item-title item-label"),
                    div({
                        input()
                            .type(contentType)
                            .name(keyName)
                            .placeholder(placeholder)
                    }).classAdd("item-input-wrap")
                }).classAdd("item-inner")
            }).classAdd("item-content item-input")
        }) { }
    };

    class BlockView: public div {
    public:
        BlockView(ViewCollection content): div(content) {
            this->classAdd("block block-strong row");
        }
    };

    class BlockColumnView: public div {
    public:
        BlockColumnView(ViewCollection content): div(content) {
            this->classAdd("col");
        }
    };

    const string ClassButtonViewTypeFill("button-fill");
    const string ClassButtonViewTypeRound("button-round");
    const string ClassButtonViewTypeSmall("button-small");
    const string ClassButtonViewTypeLarge("button-large");
    const string ClassButtonViewTypeRaised("button-raised");
    const string ClassButtonViewTypeOutline("button-outline");
    const string ClassButtonViewTypeActive("button-active");
    const string ClassButtonViewTypePreloader("button-preloader");
    const string ClassButtonViewTypeLoading("button-loading");
    const string ClassColorRed("color-red");
    const string ClassColorGreen("color-green");
    const string ClassColorBlue("color-blue");
    const string ClassColorPink("color-pink");
    const string ClassColorYellow("color-yellow");
    const string ClassColorOrange("color-orange");
    const string ClassColorGray("color-gray");
    const string ClassColorBlack("color-black");
    const string ClassColorWhite("color-white");

    class ButtonView: public a {
    public:
        ButtonView(string text): a(text) {
            this->href("#");
            this->classAdd("button button-fill");
        }
    };

    class VerticalSpacer: public div {
    public:
        VerticalSpacer(float height): div() {
            ostringstream oss;
            oss << height;
            oss << "px";
            this->style("height", oss.str());
        }
    };



} // framework7

#endif //FRPCWEBUI_FRAMEWORK7DOCUMENT_H
