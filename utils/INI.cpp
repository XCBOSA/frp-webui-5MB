//
// Created by xcbosa on 2023/1/30.
//

#include "INI.h"
#include "utils.h"

namespace xc {
    namespace utils {

        vector<string> split(const string& str, const string& delim) {
            vector<string> res;
            if ("" == str) return res;
            char * strs = new char[str.length() + 1];
            strcpy(strs, str.c_str());
            char * d = new char[delim.length() + 1];
            strcpy(d, delim.c_str());
            char *p = strtok(strs, d);
            while(p) {
                string s = p;
                res.push_back(s);
                p = strtok(NULL, d);
            }
            return res;
        }

        string& replace_all(string& src, const string& old_value, const string& new_value) {
            for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
                if ((pos = src.find(old_value, pos)) != string::npos) {
                    src.replace(pos, old_value.length(), new_value);
                }
                else break;
            }
            return src;
        }

        std::string& trim(std::string &s) {
            if (s.empty()) { return s; }
            s.erase(0,s.find_first_not_of(" "));
            s.erase(s.find_last_not_of(" ") + 1);
            return s;
        }

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