//
// Created by xcbosa on 2023/1/28.
//

#include "RequestData.h"
#include "regex"

using namespace std;

namespace xc {
    namespace utils {
        RequestData::RequestData(string url, string method, map<string, string> headers, string body) {
            this->url = url;
            this->method = method;
            this->headers = headers;
            this->body = body;
            if (this->url.length() == 0) {
                this->url = "/";
            } else {
                if (this->url[0] != '/') {
                    this->url = '/' + this->url;
                }
            }
        }

        string RequestData::getURL() const {
            return this->url;
        }

        string RequestData::getMethod() const {
            return this->method;
        }

        string RequestData::getHeader(string name) {
            return this->headers[name];
        }

        string RequestData::getBody() const {
            return this->body;
        }

        string RequestData::getURLPath() const {
            return this->url.substr(0, this->url.find_first_of('?'));
        }

        static unsigned char dec_tab[256] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
                0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        };

        static char *urlDecode(const char *str){
            int len = (int) strlen(str);
            char *tmp = (char *)malloc(len + 1);
            int i = 0, pos = 0;
            for (i = 0; i < len; i++) {
                if (str[i] != '%')
                    tmp[pos] = str[i];
                else if (i + 2 >= len) {
                    tmp[pos++] = '%';
                    if (++i >= len)
                        break;
                    tmp[pos] = str[i];
                    break;
                } else if (isalnum(str[i + 1]) && isalnum(str[i + 2])) {
                    tmp[pos] = (dec_tab[(unsigned char) str[i + 1]] << 4)
                               + dec_tab[(unsigned char) str[i + 2]];
                    i += 2;
                } else
                    tmp[pos] = str[i];
                pos++;
            }
            tmp[pos] = '\0';
            return tmp;
        }

        string RequestData::getURLArgument(string key) const {
            smatch result;
            string res;
            if (regex_search(this->url.cbegin(), this->url.cend(), result, regex(key + "=(.*?)&"))) {
                res = result[1];
            } else if (regex_search(this->url.cbegin(), this->url.cend(), result, regex(key + "=(.*)"))) {
                res = result[1];
            } else {
                return string();
            }
            char *newStr = urlDecode(res.c_str());
            res = string(newStr);
            ::free(newStr);
            return res;
        }

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

        std::string& trim(std::string &s) {
            if (s.empty()) { return s; }
            s.erase(0,s.find_first_not_of(" "));
            s.erase(s.find_last_not_of(" ") + 1);
            return s;
        }

        string RequestData::getCookie(string key) {
            string cookies = this->getHeader("Cookie");
            if (cookies.empty()) { return ""; }
            vector<string> cookieList = split(cookies, ";");
            for (string cookie : cookieList) {
                auto items = split(cookie, "=");
                if (items.size() == 2) {
                    string name = items[0];
                    string value = items[1];
                    trim(name);
                    if (name == key) {
                        return value;
                    }
                }
            }
            return "";
        }
    } // xc
} // utils