//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "ResponseData.h"
#include "utils-private.h"

using namespace std;

namespace xc {
    namespace utils {

        class TextResponseData: public ResponseData {
        public:
            TextResponseData(int statusCode, string body);
            TextResponseData(int statusCode, string body, string contentType);
            void setHeader(string headerName, string value);
            void removeHeader(string headerName);
            string getHeader(string headerName);
            void setContentType(string mimeType);
            void setStatusCode(int statusCode);
            int getStatusCode() const;
            void setBody(string body);
            string getBody();
            void writeTo(::FILE *fp) const;
            void writeResponseBodyTo(ostream &fp) const;
            void addCookie(string key, string value);
        private:
            int statusCode;
            map<string, string> headers;
            map<string, string> cookies;
            string body;
        };

    } // xc
} // utils
