//
// Created by xcbosa on 2023/1/28.
//

#include "RequestData.h"

namespace xc {
    namespace utils {
        RequestData::RequestData(string url, string method, map<string, string> headers, string body) {
            this->url = url;
            this->method = method;
            this->headers = headers;
            this->body = body;
        }

        string RequestData::getURL() const {
            return this->url;
        }

        string RequestData::getMethod() const {
            return this->method;
        }

        string RequestData::getHeader(string name) {
            return this->headers[name];
        }

        string RequestData::getBody() const {
            return this->body;
        }
    } // xc
} // utils