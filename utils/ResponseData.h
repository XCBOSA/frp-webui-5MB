//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_RESPONSEDATA_H
#define FRPCWEBUI_RESPONSEDATA_H

#include "../webui.h"

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
            int getStatusCode();
            void setBody(string body);
            string getBody();
            void writeTo(::FILE *fp);
        private:
            int statusCode;
            map<string, string> headers;
            string body;
        };

    } // xc
} // utils

#endif //FRPCWEBUI_RESPONSEDATA_H
