//
// Created by xcbosa on 2023/1/30.
//

#pragma once

#include "utils-private.h"

namespace xc {
    namespace utils {

        class INI;

        class INISection {
            friend class INI;
        public:
            INISection();
            INISection(string title, map<string, string> data);

            bool has(string key);
            string get(string key);
            void set(string key, string value);
            string getTitle();
            void setTitle(string title);
        private:
            string title;
            map<string, string> data;
        };

        class INI {
        public:
            INI();
            INI(string string);
            INI(vector<INISection> data);
            friend INI readFromFile(string filePath);

            bool has(string header);
            INISection* get(string header);
            INISection* getMust(string header);
            void remove(string header);

            string getINIString();
            vector<INISection> data;
        };

        class INIFile: public INI {
        public:
            INIFile(string filePath);
            void save();
        private:
            string filePath;
        };

    } // xc
} // utils
