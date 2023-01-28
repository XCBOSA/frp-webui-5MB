//
// Created by xcbosa on 2023/1/28.
//

#ifndef FRPCWEBUI_FILERESPONSEDATA_H
#define FRPCWEBUI_FILERESPONSEDATA_H

#include "ResponseData.h"

namespace xc {
    namespace utils {

        struct Replacement {
        public:
            string replace;
            string with;
            Replacement(string replace, string with);
        };

        class FileResponseData: public ResponseData {
        public:
            FileResponseData(int statusCode, string filePath, string contentType);
            FileResponseData(int statusCode, string filePath, string contentType, vector<Replacement> replacements);
            string getFilePath();
            void setFilePath(string filePath);
            void setFilePath(string filePath, vector<Replacement> replacements);
        private:
            string filePath;
        };

    } // xc
} // utils

#endif //FRPCWEBUI_FILERESPONSEDATA_H
