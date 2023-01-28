//
// Created by xcbosa on 2023/1/28.
//

#include "FileResponseData.h"

namespace xc {
    namespace utils {
        Replacement::Replacement(string replace, string with) {
            this->replace = replace;
            this->with = with;
        }

        FileResponseData::FileResponseData(int statusCode, string filePath, string contentType):
                ResponseData(statusCode, "", contentType),
                filePath() {
            this->setFilePath(filePath);
        }

        FileResponseData::FileResponseData(int statusCode, string filePath, string contentType, vector<Replacement> replacements):
                ResponseData(statusCode, "", contentType) {
            this->setFilePath(filePath, replacements);
        }

        string FileResponseData::getFilePath() {
            return this->filePath;
        }

        void FileResponseData::setFilePath(string filePath) {
            this->filePath = filePath;
            this->setBody(contentsOfTextFile(filePath));
        }

        void FileResponseData::setFilePath(string filePath, vector<Replacement> replacements) {
            this->filePath = filePath;
            string str = contentsOfTextFile(filePath);
            for (auto replacement : replacements) {
                string fullReplaceText = "{{" + replacement.replace + "}}";
                string::size_type pos = 0;
                while ((pos = str.find(fullReplaceText)) != string::npos) {
                    str.replace(pos, fullReplaceText.length(), replacement.with);
                }
            }
            this->setBody(str);
        }


    } // xc
} // utils