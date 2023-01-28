//
// Created by xcbosa on 2023/1/28.
//

#include "BinaryResponseData.h"
#include "../webuiconf.h"

namespace xc {
    namespace utils {
        BinaryResponseData::BinaryResponseData(int statusCode, uint8_t *body, int bodySize, string contentType): headers() {
            this->statusCode = statusCode;
            this->body = body;
            this->bodySize = bodySize;
            this->headers["Server"] = "XCHttpServer";
            this->headers["Transfer-Encoding"] = "chunked";
            this->headers["Content-Type"] = contentType;
            this->filePath = nullptr;
        }

        BinaryResponseData::BinaryResponseData(int statusCode, string filePath, string contentType): headers() {
            this->statusCode = statusCode;
            this->body = nullptr;
            this->bodySize = 0;
            this->headers["Server"] = "XCHttpServer";
            this->headers["Transfer-Encoding"] = "chunked";
            this->headers["Content-Type"] = contentType;
            this->filePath = filePath;
        }

        void BinaryResponseData::setHeader(string headerName, string value) {
            this->headers[headerName] = value;
        }

        void BinaryResponseData::writeTo(::FILE *fp) const {
            ::fprintf(fp, "HTTP/1.0 %d FRPCWebUI\r\n", this->statusCode);
            for (auto item : this->headers) {
                ::fprintf(fp, "%s: %s\r\n", item.first.c_str(), item.second.c_str());
            }
            ::fputs("\r\n", fp);
            int mtu = conf::mtu;
            if (this->isWriteFromMemory()) {
                int writeTimes = this->bodySize / mtu;
                if (this->bodySize % mtu) {
                    writeTimes++;
                }
                ::uint8_t *cursor = this->body;
                ::uint8_t *endNextCursor = this->body + this->bodySize;
                for (int i = 0; i < writeTimes; i++) {
                    long writeSize = min((long)mtu, endNextCursor - cursor);
                    ::fwrite(cursor, writeSize, mtu, fp);
                    cursor += writeSize;
                }
            }
            if (this->isWriteFromFile()) {
                string filePath = this->filePath;
                ::FILE *inputFile = ::fopen(filePath.c_str(), "rb");
                if (inputFile) {
                    ::uint8_t buff[mtu];
                    long readPerPack = 0;
                    while ((readPerPack = ::fread(buff, 1, mtu, inputFile)) > 0) {
                        ::fwrite(buff, 1, readPerPack, fp);
                    }
                } else {
                    cerr << "[FileIOError]: " << ::strerror(errno) << endl;
                }
            }
            ::fflush(fp);
        }

        bool BinaryResponseData::isWriteFromFile() const {
            return !this->isWriteFromMemory();
        }

        bool BinaryResponseData::isWriteFromMemory() const {
            return this->bodySize > 0;
        }
    } // xc
} // utils