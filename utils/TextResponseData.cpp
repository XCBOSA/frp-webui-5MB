//
// Created by xcbosa on 2023/1/28.
//

#include "TextResponseData.h"

using namespace std;

namespace xc {
    namespace utils {
        TextResponseData::TextResponseData(int statusCode, string body): headers() {
            this->statusCode = statusCode;
            this->body = body;
            this->headers["Server"] = "XCHttpServer";
            this->headers["Transfer-Encoding"] = "chunked";
            this->headers["Content-Type"] = "text/html";
        }

        TextResponseData::TextResponseData(int statusCode, string body, string contentType):
                TextResponseData(statusCode, body) {
            this->headers["Content-Type"] = contentType;
        }

        void TextResponseData::setHeader(string headerName, string value) {
            this->headers[headerName] = value;
        }

        void TextResponseData::removeHeader(string headerName) {
            this->headers[headerName] = nullptr;
        }

        string TextResponseData::getHeader(string headerName) {
            return this->headers[headerName];
        }

        void TextResponseData::setStatusCode(int statusCode) {
            this->statusCode = statusCode;
        }

        void TextResponseData::setContentType(string mimeType) {
            this->setHeader("Content-Type", mimeType);
        }

        int TextResponseData::getStatusCode() const {
            return this->statusCode;
        }

        void TextResponseData::setBody(string body) {
            this->body = body;
        }

        string TextResponseData::getBody() {
            return this->body;
        }

        void TextResponseData::writeTo(::FILE *fp) const {
            ::fprintf(fp, "HTTP/1.0 %d FRPCWebUI\r\n", this->statusCode);
            for (auto item : this->headers) {
                ::fprintf(fp, "%s: %s\r\n", item.first.c_str(), item.second.c_str());
            }
            ::fputs("\r\n", fp);
            ::fputs(this->body.c_str(), fp);
            ::fflush(fp);
        }
    } // xc
} // utils