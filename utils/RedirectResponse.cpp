//
// Created by xcbosa on 2023/1/30.
//

#include "RedirectResponse.h"

namespace xc {
    namespace utils {
        RedirectResponse::RedirectResponse(string location): TextResponseData(302, "") {
            this->setHeader("Location", location);
        }
    } // xc
} // utils