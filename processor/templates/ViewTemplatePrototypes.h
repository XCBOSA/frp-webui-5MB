//
// Created by xcbosa on 2023/1/29.
//

#pragma once

#include "../processor-private.h"

using namespace configor;

namespace xc {
    namespace processor {
        namespace templates {

            class ViewTemplatePrototype {
            public:
                ViewTemplatePrototype(string html);
                ViewTemplatePrototype(const char *html);
                ViewTemplatePrototype(string keyName, string innerText);
                ViewTemplatePrototype(string keyName, vector<ViewTemplatePrototype> inners);
                virtual void generateHTML(ostream &writeTo);
                ViewTemplatePrototype& prop(string key, string value);
                ViewTemplatePrototype& style(string key, string value);
                ViewTemplatePrototype& inner(vector<ViewTemplatePrototype> inner);
                ViewTemplatePrototype& inner(ViewTemplatePrototype inner);
                ViewTemplatePrototype& inner(string inner);

                ViewTemplatePrototype& klass(string value);
                ViewTemplatePrototype& classAdd(string klass);
                ViewTemplatePrototype& contenteditable(bool value);
                ViewTemplatePrototype& dir(string value);
                ViewTemplatePrototype& draggable(bool value);
                ViewTemplatePrototype& hidden(bool value);
                ViewTemplatePrototype& disabled(bool value);
                ViewTemplatePrototype& id(string value);
                ViewTemplatePrototype& lang(string value);
                ViewTemplatePrototype& spellcheck(bool value);
                ViewTemplatePrototype& title(string value);
                ViewTemplatePrototype& href(string value);
                ViewTemplatePrototype& onclick(string value);
                ViewTemplatePrototype& name(string value);
                ViewTemplatePrototype& placeholder(string value);
                ViewTemplatePrototype& content(string value);
                ViewTemplatePrototype& rel(string value);
                ViewTemplatePrototype& type(string value);
                ViewTemplatePrototype& src(string value);
                ViewTemplatePrototype& charset(string value);

                ViewTemplatePrototype& pointer(ViewTemplatePrototype **ptr);
            protected:
                string directHTML;
                string innerHTML;
                vector<ViewTemplatePrototype> innerViews;
                bool useDirectHTML;
                string keyName;
                map<string, string> properties;
                map<string, string> styles;
                void makeSureNotUseDirectHTML();
            };

            typedef ViewTemplatePrototype View;
            typedef vector<ViewTemplatePrototype> ViewCollection;
            typedef json::value JsonModel;
            typedef JsonModel JsonArrayObject;

            View ContentGeneratorReference(string name, RequestData request);

            typedef json::object JsonDictDef;
            typedef json::array JsonArrDef;

            class If: public View {
            public:
                If(bool conditions, ViewCollection then);
                If(bool conditions, ViewCollection then, ViewCollection else_);
            };

            class Foreach: public View {
            public:
                Foreach(vector<JsonModel> model, function<ViewTemplatePrototype (JsonModel)> generateBlock);
                Foreach(vector<JsonModel> model, function<ViewCollection (JsonModel)> generateBlock);
                Foreach(JsonArrayObject arrayModel, function<ViewTemplatePrototype (JsonModel)> generateBlock);
                Foreach(JsonArrayObject arrayModel, function<ViewCollection (JsonModel)> generateBlock);
            };

            class ViewTemplateComponent: public ViewTemplatePrototype {
            public:
                ViewTemplateComponent(vector<ViewTemplatePrototype> inners);
            };

#define __GenerateElemDefs(tagName)                       \
            class tagName: public ViewTemplatePrototype { \
            public:                                       \
                tagName();                                \
                tagName(string text);                     \
                tagName(const char *text);                \
                tagName(ViewCollection inner);            \
                tagName(ViewTemplatePrototype view);      \
            };

            __GenerateElemDefs(a)
            __GenerateElemDefs(abbr)
            __GenerateElemDefs(acronym)
            __GenerateElemDefs(address)
            __GenerateElemDefs(applet)
            __GenerateElemDefs(area)
            __GenerateElemDefs(article)
            __GenerateElemDefs(aside)
            __GenerateElemDefs(audio)
            __GenerateElemDefs(b)
            __GenerateElemDefs(base)
            __GenerateElemDefs(bdi)
            __GenerateElemDefs(bdo)
            __GenerateElemDefs(big)
            __GenerateElemDefs(blockquote)
            __GenerateElemDefs(body)
            __GenerateElemDefs(br)
            __GenerateElemDefs(button)
            __GenerateElemDefs(canvas)
            __GenerateElemDefs(caption)
            __GenerateElemDefs(center)
            __GenerateElemDefs(cite)
            __GenerateElemDefs(code)
            __GenerateElemDefs(col)
            __GenerateElemDefs(colgroup)
            __GenerateElemDefs(command)
            __GenerateElemDefs(data)
            __GenerateElemDefs(datalist)
            __GenerateElemDefs(do_)
            __GenerateElemDefs(del)
            __GenerateElemDefs(details)
            __GenerateElemDefs(dir)
            __GenerateElemDefs(div)
            __GenerateElemDefs(dfn)
            __GenerateElemDefs(dialog)
            __GenerateElemDefs(dl)
            __GenerateElemDefs(dt)
            __GenerateElemDefs(em)
            __GenerateElemDefs(embed)
            __GenerateElemDefs(fieldset)
            __GenerateElemDefs(figcaption)
            __GenerateElemDefs(figure)
            __GenerateElemDefs(font)
            __GenerateElemDefs(footer)
            __GenerateElemDefs(form)
            __GenerateElemDefs(frame)
            __GenerateElemDefs(frameset)
            __GenerateElemDefs(h1)
            __GenerateElemDefs(h2)
            __GenerateElemDefs(h3)
            __GenerateElemDefs(h4)
            __GenerateElemDefs(h5)
            __GenerateElemDefs(h6)
            __GenerateElemDefs(head)
            __GenerateElemDefs(header)
            __GenerateElemDefs(hr)
            __GenerateElemDefs(html)
            __GenerateElemDefs(i)
            __GenerateElemDefs(iframe)
            __GenerateElemDefs(img)
            __GenerateElemDefs(input)
            __GenerateElemDefs(ins)
            __GenerateElemDefs(isindex)
            __GenerateElemDefs(kbd)
            __GenerateElemDefs(keygen)
            __GenerateElemDefs(label)
            __GenerateElemDefs(legend)
            __GenerateElemDefs(li)
            __GenerateElemDefs(link)
            __GenerateElemDefs(main)
            __GenerateElemDefs(map_)
            __GenerateElemDefs(mark)
            __GenerateElemDefs(menu)
            __GenerateElemDefs(menuitem)
            __GenerateElemDefs(meta)
            __GenerateElemDefs(meter)
            __GenerateElemDefs(nav)
            __GenerateElemDefs(noframes)
            __GenerateElemDefs(noscript)
            __GenerateElemDefs(object)
            __GenerateElemDefs(ol)
            __GenerateElemDefs(optgroup)
            __GenerateElemDefs(option)
            __GenerateElemDefs(output)
            __GenerateElemDefs(p)
            __GenerateElemDefs(param)
            __GenerateElemDefs(pre)
            __GenerateElemDefs(progress)
            __GenerateElemDefs(q)
            __GenerateElemDefs(rp)
            __GenerateElemDefs(ruby)
            __GenerateElemDefs(s)
            __GenerateElemDefs(samp)
            __GenerateElemDefs(script)
            __GenerateElemDefs(section)
            __GenerateElemDefs(select)
            __GenerateElemDefs(small)
            __GenerateElemDefs(source)
            __GenerateElemDefs(span)
            __GenerateElemDefs(strike)
            __GenerateElemDefs(strong)
            __GenerateElemDefs(style)
            __GenerateElemDefs(sub)
            __GenerateElemDefs(summary)
            __GenerateElemDefs(sup)
            __GenerateElemDefs(svg)
            __GenerateElemDefs(table)
            __GenerateElemDefs(tbody)
            __GenerateElemDefs(td)
            __GenerateElemDefs(template_)
            __GenerateElemDefs(textarea)
            __GenerateElemDefs(tfoot)
            __GenerateElemDefs(th)
            __GenerateElemDefs(thead)
            __GenerateElemDefs(time)
            __GenerateElemDefs(title)
            __GenerateElemDefs(tr)
            __GenerateElemDefs(track)
            __GenerateElemDefs(tt)
            __GenerateElemDefs(u)
            __GenerateElemDefs(ul)
            __GenerateElemDefs(var)
            __GenerateElemDefs(video)
            __GenerateElemDefs(wbr)
            __GenerateElemDefs(xmp)


        } // xc
    } // processor
} // templates
