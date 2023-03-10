//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "utils-private.h"

namespace xc {
    namespace utils {

        class RequestData {
        public:
            RequestData(string url, string method, map<string, string> headers, string body);
            string getURL() const;
            string getURLPath() const;
            string getURLArgument(string key) const;
            string getMethod() const;
            string getHeader(string name);
            string getBody() const;
            string getCookie(string key);
        private:
            string url;
            string method;
            map<string, string> headers;
            string body;
        };

    } // xc
} // utils
