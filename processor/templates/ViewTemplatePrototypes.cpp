//
// Created by xcbosa on 2023/1/29.
//

#include "ViewTemplatePrototypes.h"

namespace xc {
    namespace processor {
        namespace templates {

            string& replace_all(string& src, const string& old_value, const string& new_value) {
                for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
                    if ((pos = src.find(old_value, pos)) != string::npos) {
                        src.replace(pos, old_value.length(), new_value);
                    }
                    else break;
                }
                return src;
            }

            string fixStringFormat(string strToFix) {
                replace_all(strToFix, "\"", "\\\"");
                replace_all(strToFix, "\'", "\\\'");
                replace_all(strToFix, "\r", "\\\r");
                replace_all(strToFix, "\n", "\\\n");
                return strToFix;
            }

            string generateInner(ViewTemplatePrototype inner) {
                ostringstream oss;
                inner.generateHTML(oss);
                return oss.str();
            }

            string generateInner(vector<ViewTemplatePrototype> inners) {
                ostringstream oss;
                for (auto it : inners) {
                    it.generateHTML(oss);
                }
                return oss.str();
            }

            ViewTemplatePrototype::ViewTemplatePrototype(string directHTML): directHTML(directHTML), useDirectHTML(true), keyName(), innerHTML(), properties(), styles() { }

            ViewTemplatePrototype::ViewTemplatePrototype(const char *directHTML): directHTML(directHTML), useDirectHTML(true), keyName(), innerHTML(), properties(), styles() { }

            ViewTemplatePrototype::ViewTemplatePrototype(string keyName, string innerText): directHTML(), useDirectHTML(false), keyName(keyName), innerHTML(innerText), properties(), styles() { }

            ViewTemplatePrototype::ViewTemplatePrototype(string keyName, vector<ViewTemplatePrototype> inners): directHTML(), useDirectHTML(false), keyName(keyName), innerHTML(generateInner(inners)), properties(), styles() { }

            void ViewTemplatePrototype::generateHTML(ostream &writeTo) {
                if (this->useDirectHTML) {
                    writeTo << this->directHTML;
                } else {
                    if (!this->keyName.empty()) {
                        writeTo << '<';
                        writeTo << this->keyName;
                        writeTo << ' ';
                        for (auto it: this->properties) {
                            writeTo << it.first;
                            writeTo << "=\"";
                            writeTo << fixStringFormat(it.second);
                            writeTo << "\" ";
                        }

                        writeTo << "style=\"";
                        for (auto it: this->styles) {
                            writeTo << it.first;
                            writeTo << ": ";
                            writeTo << it.second;
                            writeTo << "; ";
                        }
                        writeTo << '\"';

                        writeTo << '>';
                    }
                    writeTo << this->innerHTML;
                    if (!this->keyName.empty()) {
                        writeTo << "</";
                        writeTo << this->keyName;
                        writeTo << '>';
                    }
                }
            }

            ViewTemplatePrototype& ViewTemplatePrototype::prop(string key, string value) {
                this->makeSureNotUseDirectHTML();
                this->properties[key] = value;
                return *this;
            }

            ViewTemplatePrototype& ViewTemplatePrototype::style(string key, string value) {
                this->makeSureNotUseDirectHTML();
                this->styles[key] = value;
                return *this;
            }

            ViewTemplatePrototype& ViewTemplatePrototype::inner(vector<ViewTemplatePrototype> inner) {
                this->makeSureNotUseDirectHTML();
                this->innerHTML = generateInner(inner);
                return *this;
            }

            ViewTemplatePrototype& ViewTemplatePrototype::inner(string inner) {
                this->makeSureNotUseDirectHTML();
                this->innerHTML = inner;
                return *this;
            }

            void ViewTemplatePrototype::makeSureNotUseDirectHTML() {
                if (this->useDirectHTML) {
                    this->useDirectHTML = false;
                    this->keyName = this->directHTML;
                    this->directHTML = "";
                }
            }

            ViewTemplatePrototype& ViewTemplatePrototype::klass(string value) { return this->prop("class", value); }
            ViewTemplatePrototype& ViewTemplatePrototype::classAdd(string klass) {
                if (this->properties.find("class") == this->properties.end()) {
                    this->properties["class"] = klass;
                } else {
                    this->properties["class"] += klass + " ";
                }
                return *this;
            }
            ViewTemplatePrototype& ViewTemplatePrototype::contenteditable(bool value) { return this->prop("contenteditable", value ? "true" : "false"); }
            ViewTemplatePrototype& ViewTemplatePrototype::dir(string value) { return this->prop("dir", value); }
            ViewTemplatePrototype& ViewTemplatePrototype::draggable(bool value) { return this->prop("draggable", value ? "true" : "false"); }
            ViewTemplatePrototype& ViewTemplatePrototype::hidden(bool value) { return this->prop("hidden", value ? "true" : "false"); }
            ViewTemplatePrototype& ViewTemplatePrototype::id(string value) { return this->prop("id", value); }
            ViewTemplatePrototype& ViewTemplatePrototype::lang(string value) { return this->prop("lang", value); }
            ViewTemplatePrototype& ViewTemplatePrototype::spellcheck(bool value) { return this->prop("spellcheck", value ? "true" : "false"); }
            ViewTemplatePrototype& ViewTemplatePrototype::title(string value) { return this->prop("title", value); }

            Foreach::Foreach(vector<JsonModel> model, function<ViewTemplatePrototype(JsonModel)> generateBlock): ViewTemplatePrototype("") {
                ostringstream oss;
                for (JsonModel t : model) {
                    generateBlock(t).generateHTML(oss);
                }
                this->inner(oss.str());
            }

            Foreach::Foreach(vector<JsonModel> model, function<ViewCollection (JsonModel)> generateBlock): ViewTemplatePrototype("") {
                ostringstream oss;
                for (JsonModel t : model) {
                    oss << generateInner(generateBlock(t));
                }
                this->inner(oss.str());
            }

            Foreach::Foreach(JsonArrayObject model, function<ViewTemplatePrototype(JsonModel)> generateBlock): ViewTemplatePrototype("") {
                ostringstream oss;
                for (JsonModel t : model) {
                    generateBlock(t).generateHTML(oss);
                }
                this->inner(oss.str());
            }

            Foreach::Foreach(JsonArrayObject model, function<ViewCollection (JsonModel)> generateBlock): ViewTemplatePrototype("") {
                ostringstream oss;
                for (JsonModel t : model) {
                    oss << generateInner(generateBlock(t));
                }
                this->inner(oss.str());
            }

#define __GenerateElemImpl(tagName, keyString)                                                            \
            tagName::tagName(string text): ViewTemplatePrototype(keyString, text) { }                     \
            tagName::tagName(const char *text): ViewTemplatePrototype(keyString, string(text)) { }        \
            tagName::tagName(ViewCollection inner): ViewTemplatePrototype(keyString, inner) { }           \
            tagName::tagName(ViewTemplatePrototype inner): ViewTemplatePrototype(keyString, generateInner(inner)) { }
#define __GenerateElemImpl_KeySameAsTagName(tagName) __GenerateElemImpl(tagName, #tagName)

            __GenerateElemImpl_KeySameAsTagName(a)
            __GenerateElemImpl_KeySameAsTagName(abbr)
            __GenerateElemImpl_KeySameAsTagName(acronym)
            __GenerateElemImpl_KeySameAsTagName(address)
            __GenerateElemImpl_KeySameAsTagName(applet)
            __GenerateElemImpl_KeySameAsTagName(area)
            __GenerateElemImpl_KeySameAsTagName(article)
            __GenerateElemImpl_KeySameAsTagName(aside)
            __GenerateElemImpl_KeySameAsTagName(audio)
            __GenerateElemImpl_KeySameAsTagName(b)
            __GenerateElemImpl_KeySameAsTagName(base)
            __GenerateElemImpl_KeySameAsTagName(bdi)
            __GenerateElemImpl_KeySameAsTagName(bdo)
            __GenerateElemImpl_KeySameAsTagName(big)
            __GenerateElemImpl_KeySameAsTagName(blockquote)
            __GenerateElemImpl_KeySameAsTagName(body)
            __GenerateElemImpl_KeySameAsTagName(br)
            __GenerateElemImpl_KeySameAsTagName(button)
            __GenerateElemImpl_KeySameAsTagName(canvas)
            __GenerateElemImpl_KeySameAsTagName(caption)
            __GenerateElemImpl_KeySameAsTagName(center)
            __GenerateElemImpl_KeySameAsTagName(cite)
            __GenerateElemImpl_KeySameAsTagName(code)
            __GenerateElemImpl_KeySameAsTagName(col)
            __GenerateElemImpl_KeySameAsTagName(colgroup)
            __GenerateElemImpl_KeySameAsTagName(command)
            __GenerateElemImpl_KeySameAsTagName(data)
            __GenerateElemImpl_KeySameAsTagName(datalist)
            __GenerateElemImpl(do_, "do")
            __GenerateElemImpl_KeySameAsTagName(del)
            __GenerateElemImpl_KeySameAsTagName(details)
            __GenerateElemImpl_KeySameAsTagName(dir)
            __GenerateElemImpl_KeySameAsTagName(div)
            __GenerateElemImpl_KeySameAsTagName(dfn)
            __GenerateElemImpl_KeySameAsTagName(dialog)
            __GenerateElemImpl_KeySameAsTagName(dl)
            __GenerateElemImpl_KeySameAsTagName(dt)
            __GenerateElemImpl_KeySameAsTagName(em)
            __GenerateElemImpl_KeySameAsTagName(embed)
            __GenerateElemImpl_KeySameAsTagName(fieldset)
            __GenerateElemImpl_KeySameAsTagName(figcaption)
            __GenerateElemImpl_KeySameAsTagName(figure)
            __GenerateElemImpl_KeySameAsTagName(font)
            __GenerateElemImpl_KeySameAsTagName(footer)
            __GenerateElemImpl_KeySameAsTagName(form)
            __GenerateElemImpl_KeySameAsTagName(frame)
            __GenerateElemImpl_KeySameAsTagName(frameset)
            __GenerateElemImpl_KeySameAsTagName(h1)
            __GenerateElemImpl_KeySameAsTagName(h2)
            __GenerateElemImpl_KeySameAsTagName(h3)
            __GenerateElemImpl_KeySameAsTagName(h4)
            __GenerateElemImpl_KeySameAsTagName(h5)
            __GenerateElemImpl_KeySameAsTagName(h6)
            __GenerateElemImpl_KeySameAsTagName(head)
            __GenerateElemImpl_KeySameAsTagName(header)
            __GenerateElemImpl_KeySameAsTagName(hr)
            __GenerateElemImpl_KeySameAsTagName(html)
            __GenerateElemImpl_KeySameAsTagName(i)
            __GenerateElemImpl_KeySameAsTagName(iframe)
            __GenerateElemImpl_KeySameAsTagName(img)
            __GenerateElemImpl_KeySameAsTagName(input)
            __GenerateElemImpl_KeySameAsTagName(ins)
            __GenerateElemImpl_KeySameAsTagName(isindex)
            __GenerateElemImpl_KeySameAsTagName(kbd)
            __GenerateElemImpl_KeySameAsTagName(keygen)
            __GenerateElemImpl_KeySameAsTagName(label)
            __GenerateElemImpl_KeySameAsTagName(legend)
            __GenerateElemImpl_KeySameAsTagName(li)
            __GenerateElemImpl_KeySameAsTagName(link)
            __GenerateElemImpl_KeySameAsTagName(main)
            __GenerateElemImpl_KeySameAsTagName(map_)
            __GenerateElemImpl_KeySameAsTagName(mark)
            __GenerateElemImpl_KeySameAsTagName(menu)
            __GenerateElemImpl_KeySameAsTagName(menuitem)
            __GenerateElemImpl_KeySameAsTagName(meta)
            __GenerateElemImpl_KeySameAsTagName(meter)
            __GenerateElemImpl_KeySameAsTagName(nav)
            __GenerateElemImpl_KeySameAsTagName(noframes)
            __GenerateElemImpl_KeySameAsTagName(noscript)
            __GenerateElemImpl_KeySameAsTagName(object)
            __GenerateElemImpl_KeySameAsTagName(ol)
            __GenerateElemImpl_KeySameAsTagName(optgroup)
            __GenerateElemImpl_KeySameAsTagName(option)
            __GenerateElemImpl_KeySameAsTagName(output)
            __GenerateElemImpl_KeySameAsTagName(p)
            __GenerateElemImpl_KeySameAsTagName(param)
            __GenerateElemImpl_KeySameAsTagName(pre)
            __GenerateElemImpl_KeySameAsTagName(progress)
            __GenerateElemImpl_KeySameAsTagName(q)
            __GenerateElemImpl_KeySameAsTagName(rp)
            __GenerateElemImpl_KeySameAsTagName(ruby)
            __GenerateElemImpl_KeySameAsTagName(s)
            __GenerateElemImpl_KeySameAsTagName(samp)
            __GenerateElemImpl_KeySameAsTagName(script)
            __GenerateElemImpl_KeySameAsTagName(section)
            __GenerateElemImpl_KeySameAsTagName(select)
            __GenerateElemImpl_KeySameAsTagName(small)
            __GenerateElemImpl_KeySameAsTagName(source)
            __GenerateElemImpl_KeySameAsTagName(span)
            __GenerateElemImpl_KeySameAsTagName(strike)
            __GenerateElemImpl_KeySameAsTagName(strong)
            __GenerateElemImpl_KeySameAsTagName(style)
            __GenerateElemImpl_KeySameAsTagName(sub)
            __GenerateElemImpl_KeySameAsTagName(summary)
            __GenerateElemImpl_KeySameAsTagName(sup)
            __GenerateElemImpl_KeySameAsTagName(svg)
            __GenerateElemImpl_KeySameAsTagName(table)
            __GenerateElemImpl_KeySameAsTagName(tbody)
            __GenerateElemImpl_KeySameAsTagName(td)
            __GenerateElemImpl(template_, "template_")
            __GenerateElemImpl_KeySameAsTagName(textarea)
            __GenerateElemImpl_KeySameAsTagName(tfoot)
            __GenerateElemImpl_KeySameAsTagName(th)
            __GenerateElemImpl_KeySameAsTagName(thead)
            __GenerateElemImpl_KeySameAsTagName(time)
            __GenerateElemImpl_KeySameAsTagName(title)
            __GenerateElemImpl_KeySameAsTagName(tr)
            __GenerateElemImpl_KeySameAsTagName(track)
            __GenerateElemImpl_KeySameAsTagName(tt)
            __GenerateElemImpl_KeySameAsTagName(u)
            __GenerateElemImpl_KeySameAsTagName(ul)
            __GenerateElemImpl_KeySameAsTagName(var)
            __GenerateElemImpl_KeySameAsTagName(video)
            __GenerateElemImpl_KeySameAsTagName(wbr)
            __GenerateElemImpl_KeySameAsTagName(xmp)

        } // xc
    } // processor
} // templates
