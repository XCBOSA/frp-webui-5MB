//
// Created by xcbosa on 2023/1/29.
//

#include "TemplateResponseData.h"

namespace xc {
    namespace processor {
        namespace templates {

            TemplateResponseData::TemplateResponseData(vector<ViewTemplatePrototype> prototypes): TextResponseData(200, "") {
                this->generateBody(prototypes);
            }

            void TemplateResponseData::generateBody(const vector<ViewTemplatePrototype> &prototypes) {
                ostringstream oss;
                for (auto it : prototypes) {
                    it.generateHTML(oss);
                }
                this->setBody(oss.str());
            }

        } // xc
    } // processor
} // templates