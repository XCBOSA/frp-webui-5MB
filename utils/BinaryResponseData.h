//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "utils-private.h"
#include "ResponseData.h"

namespace xc {
    namespace utils {

        class BinaryResponseData: public ResponseData {
        public:
            BinaryResponseData(int statusCode, uint8_t *body, int bodySize, string contentType);
            BinaryResponseData(int statusCode, string filePath, string contentType);
            void writeTo(::FILE *fp) const;
            void writeResponseBodyTo(ostream &fp) const;
            void setHeader(string headerName, string value);
            bool isWriteFromFile() const;
            bool isWriteFromMemory() const;
        private:
            int statusCode;
            map<string, string> headers;
            uint8_t *body;
            string filePath;
            int bodySize;
        };

    } // xc
} // utils
