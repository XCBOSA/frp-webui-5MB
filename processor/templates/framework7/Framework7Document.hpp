//
// Created by xcbosa on 2023/1/29.
//

#ifndef FRPCWEBUI_FRAMEWORK7DOCUMENT_H
#define FRPCWEBUI_FRAMEWORK7DOCUMENT_H

#include "../../processor.h"

namespace xc::processor::templates::framework7 {
    class Framework7Document: public ViewTemplateComponent {
    public:
        Framework7Document(): ViewTemplateComponent({ }) {
            html html({
                head({
                    meta().prop("charset", "utf-8"),
                    meta().prop("name", "viewport").prop("content", "width=device-width, initial-scale=1, maximum-scale=1, minimum-scale=1, user-scalable=no, viewport-fit=cover"),
                    meta().prop("name", "apple-mobile-web-app-capable").prop("content", "yes"),
                    meta().prop("name", "theme-color").prop("content", "#2196f3"),
                    templates::title("My App"),
                    link().prop("rel", "stylesheet").href("framework7/framework7-bundle.min.css"),
                    link().prop("rel", "stylesheet").href("index.css")
                }),
                body({
                    div({
                        div({
                            div({
                                div().classAdd("navbar-bg"),
                                div({
                                    div("App").classAdd("title")
                                }).classAdd("navbar-inner")
                            }).classAdd("navbar"),
                            div({
                                div({
                                    a("Link 1").href("#").classAdd("link"),
                                    a("Link 2").href("#").classAdd("link")
                                }).classAdd("toolbar-inner")
                            }).classAdd("toolbar toolbar-bottom"),
                            div({
                                p("Page content goes here"),
                                a("About app").href("/about/")
                            }).classAdd("page-content")

                        }).prop("data-name", "home").classAdd("page")
                    }).classAdd("view view-main")
                }).id("app"),
                script().prop("type", "text/javascript").prop("src", "framework7/framework7-bundle.min.js"),
                script().prop("type", "text/javascript").prop("src", "index.js"),
            });
            this->inner(html);
        }
    };
} // framework7

#endif //FRPCWEBUI_FRAMEWORK7DOCUMENT_H