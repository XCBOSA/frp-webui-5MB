//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "utils-private.h"

using namespace std;

namespace xc {
    namespace utils {

        class ResponseData {
        public:
            ResponseData(int statusCode, string body);
            ResponseData(int statusCode, string body, string contentType);
            void setHeader(string headerName, string value);
            void removeHeader(string headerName);
            string getHeader(string headerName);
            void setContentType(string mimeType);
            void setStatusCode(int statusCode);
            int getStatusCode() const;
            void setBody(string body);
            string getBody();
            void writeTo(::FILE *fp) const;
        private:
            int statusCode;
            map<string, string> headers;
            string body;
        };

    } // xc
} // utils
