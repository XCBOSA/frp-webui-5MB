//
// Created by xcbosa on 2023/1/28.
//

#include "utils-private.h"

using namespace std;

namespace xc::utils {
    string contentsOfTextFile(string filePath) {
        ifstream fin(filePath);
        stringstream buffer;
        buffer << fin.rdbuf();
        string str(buffer.str());
        fin.close();
        return str;
    }
}
