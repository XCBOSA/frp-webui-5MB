//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "utils-private.h"
#include "TextResponseData.h"

using namespace std;

namespace xc {
    namespace utils {

        struct Replacement {
        public:
            string replace;
            string with;
            Replacement(string replace, string with);
        };

        class FileResponseData: public TextResponseData {
        public:
            FileResponseData(int statusCode, string filePath, string contentType);
            FileResponseData(int statusCode, string filePath, string contentType, vector<Replacement> replacements);
            string getFilePath();
            void setFilePath(string filePath);
            void setFilePath(string filePath, vector<Replacement> replacements);
        private:
            string filePath;
        };

        class IncompleteFileResponseData {
        public:
            IncompleteFileResponseData(FileResponseData holdData);
            FileResponseData applyReplacements(vector<Replacement> replacements) const;
            FileResponseData applyReplacements(int statusCode, vector<Replacement> replacements) const;
        private:
            FileResponseData holdData;
        };

    } // xc
} // utils
