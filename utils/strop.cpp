//
// Created by xcbosa on 2023/1/31.
//

#include "strop.h"

namespace xc {
    namespace utils {

        vector<string> split(const string& str, const string& delim) {
            vector<string> res;
            if ("" == str) return res;
            char *strs = new char[str.length() + 1];
            strcpy(strs, str.c_str());
            char *d = new char[delim.length() + 1];
            strcpy(d, delim.c_str());
            char *p = strtok(strs, d);
            while (p) {
                string s = p;
                res.push_back(s);
                p = strtok(NULL, d);
            }
            delete[] strs;
            delete[] d;
            return res;
        }

        string replace_all(string& src, const string& old_value, const string& new_value) {
            for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
                if ((pos = src.find(old_value, pos)) != string::npos) {
                    src.replace(pos, old_value.length(), new_value);
                }
                else break;
            }
            return src;
        }

        string fixStringTransfer(string& src) {
            string modify = src;
            replace_all(modify, "\"", "\\\"");
            replace_all(modify, "\r", "\\\r");
            replace_all(modify, "\n", "\\\n");
            return modify;
        }

        string& trim(string &s) {
            if (s.empty()) { return s; }
            s.erase(0,s.find_first_not_of(" "));
            s.erase(s.find_last_not_of(" ") + 1);
            return s;
        }

        string uppercase(string s) {
            ostringstream oss;
            for (char ch : s) {
                oss << (char)::toupper(ch);
            }
            return oss.str();
        }

    } // xc
} // utils