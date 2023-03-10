//
// Created by xcbosa on 2023/1/30.
//

#include "INI.h"
#include "utils.h"
#include "strop.h"

namespace xc {
    namespace utils {

        INISection::INISection(): title(), data() { }

        INISection::INISection(string title, map<string, string> data): title(title), data(data) { }

        string INISection::get(string key) {
            return this->data[key];
        }

        bool INISection::has(string key) {
            return this->data.count(key) == 1;
        }

        void INISection::set(string key, string value) {
            this->data[key] = value;
        }

        string INISection::getTitle() {
            return this->title;
        }

        void INISection::setTitle(string title) {
            this->title = title;
        }

        INI::INI(): data() { }

        INI::INI(string str) {
            vector<string> lines = split(str, "\n");
            string sectionName = "";
            map<string, string> currentSection;
            for (string it : lines) {
                string line = it;
                replace_all(line, "\r", "");
                if (line.length() == 0) { continue; }
                if (line[0] == '[') {
                    if (sectionName.length() > 0 || !currentSection.empty()) {
                        this->data.push_back(INISection(sectionName, currentSection));
                        sectionName.clear();
                        currentSection.clear();
                    }
                    ostringstream oss;
                    for (string::iterator it = line.begin() + 1; it != line.end() && *it != ']'; it++) {
                        oss << *it;
                    }
                    sectionName = oss.str();
                    continue;
                }
                ostringstream left, right;
                bool isRight = false;
                for (int i = 0; i < line.length(); i++) {
                    if (isRight) {
                        right << line[i];
                        continue;
                    }
                    if (line[i] == '=') {
                        isRight = true;
                        continue;
                    }
                    left << line[i];
                }
                string leftStr = left.str();
                string rightStr = right.str();
                trim(leftStr);
                trim(rightStr);
                currentSection[leftStr] = rightStr;
            }
            if (sectionName.length() > 0 || !currentSection.empty()) {
                this->data.push_back(INISection(sectionName, currentSection));
                sectionName.clear();
                currentSection.clear();
            }
        }

        INI::INI(vector<INISection> data): data(data) { }

        INI readFromFile(string filePath) {
            return INI(contentsOfTextFile(filePath));
        }

        bool INI::has(string header) {
            return this->get(header) != nullptr;
        }

        INISection* INI::get(string header) {
            for (auto it = this->data.begin(); it != this->data.end(); it++) {
                if (it->title == header) {
                    return &*it;
                }
            }
            return nullptr;
        }

        INISection *INI::getMust(string header) {
            INISection *section = this->get(header);
            if (section == nullptr) {
                this->data.push_back(INISection(header, map<string, string>()));
                return this->getMust(header);
            }
            return section;
        }

        void INI::remove(string header) {
            std::remove_if(this->data.begin(), this->data.end(), [&header](auto it) {
                return it.title == header;
            });
        }

        string INI::getINIString() {
            ostringstream oss;
            for (INISection section : this->data) {
                oss << '[';
                oss << section.title;
                oss << "]\n";
                for (auto it : section.data) {
                    oss << it.first;
                    oss << " = ";
                    oss << it.second;
                    oss << '\n';
                }
                oss << '\n';
            }
            return oss.str();
        }

        INIFile::INIFile(string filePath): INI(contentsOfTextFile(filePath)) {
            this->filePath = filePath;
        }

        void INIFile::save() {
            saveTextFile(this->filePath, this->getINIString());
        }

    } // xc
} // utils