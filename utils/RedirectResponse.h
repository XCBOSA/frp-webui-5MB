//
// Created by xcbosa on 2023/1/30.
//

#pragma once

#include "utils-private.h"
#include "TextResponseData.h"

namespace xc {
    namespace utils {

        class RedirectResponse: public TextResponseData {
        public:
            RedirectResponse(string location);
        };

    } // xc
} // utils
