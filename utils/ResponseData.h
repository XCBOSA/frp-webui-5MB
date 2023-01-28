//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "utils-private.h"

namespace xc {
    namespace utils {

        class ResponseData {
        public:
            virtual void writeTo(::FILE *fp) const = 0;
        };

    } // xc
} // utils
