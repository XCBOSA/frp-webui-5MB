//
// Created by xcbosa on 2023/1/31.
//

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace xc {
    namespace utils {

        vector<string> split(const string& str, const string& delim);
        string replace_all(string& src, const string& old_value, const string& new_value);
        string fixStringTransfer(string& src);
        string& trim(string &s);
        string uppercase(string s);

    } // xc
} // utils