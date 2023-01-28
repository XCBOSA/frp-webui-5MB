//
// Created by xcbosa on 2023/1/28.
//

#include "ResponseData.h"

using namespace std;

namespace xc {
    namespace utils {
        ResponseData::ResponseData(int statusCode, string body): headers() {
            this->statusCode = statusCode;
            this->body = body;
            this->headers["Server"] = "XCHttpServer";
            this->headers["Transfer-Encoding"] = "chunked";
            this->headers["Content-Type"] = "text/html";
        }

        ResponseData::ResponseData(int statusCode, string body, string contentType):
                ResponseData(statusCode, body) {
            this->headers["Content-Type"] = contentType;
        }

        void ResponseData::setHeader(string headerName, string value) {
            this->headers[headerName] = value;
        }

        void ResponseData::removeHeader(string headerName) {
            this->headers[headerName] = nullptr;
        }

        string ResponseData::getHeader(string headerName) {
            return this->headers[headerName];
        }

        void ResponseData::setStatusCode(int statusCode) {
            this->statusCode = statusCode;
        }

        void ResponseData::setContentType(string mimeType) {
            this->setHeader("Content-Type", mimeType);
        }

        int ResponseData::getStatusCode() const {
            return this->statusCode;
        }

        void ResponseData::setBody(string body) {
            this->body = body;
        }

        string ResponseData::getBody() {
            return this->body;
        }

        void ResponseData::writeTo(::FILE *fp) const {
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