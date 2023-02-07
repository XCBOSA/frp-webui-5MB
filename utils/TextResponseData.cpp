//
// Created by xcbosa on 2023/1/28.
//

#include "TextResponseData.h"
#include "../webuiconf.h"

using namespace std;

namespace xc {
    namespace utils {
        TextResponseData::TextResponseData(int statusCode, string body): headers() {
            this->statusCode = statusCode;
            this->body = body;
            this->headers["Server"] = "XCHttpServer";
            this->headers["Transfer-Encoding"] = "chunked";
            this->headers["Content-Type"] = "text/html";
            this->headers["keepalive"] = "false";
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
            ::fprintf(fp, "HTTP/1.1 %d XCHttpServer\r\n", this->statusCode);
            if (!this->cookies.empty()) {
                for (auto it : this->cookies) {
                    ostringstream oss;
                    oss << it.first;
                    oss << "=";
                    oss << it.second;
                    oss << "; Path=/";
                    string str = oss.str();
                    ::fprintf(fp, "%s: %s\r\n", "Set-Cookie", str.c_str());
                }
            }
            for (auto item : this->headers) {
                ::fprintf(fp, "%s: %s\r\n", item.first.c_str(), item.second.c_str());
            }
            ::fputs("\r\n", fp);

            long mtu = conf::mtu;
            const char *data = this->body.c_str();
            const char *cursor = data;
            long dataSize = ::strlen(data);
            const char *endNextCursor = data + ::strlen(data);
            int writeTimes = dataSize / mtu;
            if (dataSize % mtu) {
                writeTimes++;
            }
            for (int i = 0; i < writeTimes; i++) {
                long writeSize = min((long)mtu, endNextCursor - cursor);
                ::fprintf(fp, "%x\r\n", writeSize);
                ::fwrite(cursor, 1, writeSize, fp);
                ::fprintf(fp, "\r\n");
                cursor += writeSize;
            }
            ::fprintf(fp, "0\r\n\r\n");
            ::fflush(fp);
        }

        void TextResponseData::writeResponseBodyTo(ostream &fp) const {
            fp << this->body;
        }

        void TextResponseData::addCookie(string key, string value) {
            this->cookies[key] = value;
        }
    } // xc
} // utils