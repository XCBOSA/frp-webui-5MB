//
// Created by xcbosa on 2023/1/29.
//

#pragma once

#include "../processor-private.h"
#include "ViewTemplatePrototypes.h"

namespace xc {
    namespace processor {
        namespace templates {

            class TemplateResponseData: public TextResponseData {
            public:
                TemplateResponseData(vector<ViewTemplatePrototype> prototypes);
                TemplateResponseData(int statusCode, vector<ViewTemplatePrototype> prototypes);
                void generateBody(const vector<ViewTemplatePrototype> &prototypes);
            };

        } // xc
    } // processor
} // templates
