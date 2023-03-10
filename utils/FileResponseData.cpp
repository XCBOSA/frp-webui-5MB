//
// Created by xcbosa on 2023/1/28.
//

#include "FileResponseData.h"
#include "strop.h"

using namespace std;

namespace xc {
    namespace utils {
        Replacement::Replacement(string replace, string with) {
            this->replace = replace;
            this->with = with;
        }

        FileResponseData::FileResponseData(int statusCode, string filePath, string contentType):
                TextResponseData(statusCode, "", contentType),
                filePath() {
            this->setFilePath(filePath);
        }

        FileResponseData::FileResponseData(int statusCode, string filePath, string contentType, vector<Replacement> replacements):
                TextResponseData(statusCode, "", contentType) {
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
                replace_all(str, fullReplaceText, replacement.with);
            }
            this->setBody(str);
        }

        IncompleteFileResponseData::IncompleteFileResponseData(FileResponseData holdData): holdData(holdData) { }

        FileResponseData IncompleteFileResponseData::applyReplacements(vector<Replacement> replacements) const {
            return applyReplacements(this->holdData.getStatusCode(), replacements);
        }

        FileResponseData IncompleteFileResponseData::applyReplacements(int statusCode, vector<Replacement> replacements) const {
            FileResponseData responseData(this->holdData);
            responseData.setStatusCode(statusCode);
            string oriBody = responseData.getBody();
            for (auto replacement : replacements) {
                string fullReplaceText = "{{" + replacement.replace + "}}";
                replace_all(oriBody, fullReplaceText, replacement.with);
            }
            responseData.setBody(oriBody);
            return responseData;
        }

    } // xc
} // utils