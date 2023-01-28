//
// Created by xcbosa on 2023/1/28.
//

#include "utils-private.h"
#include "../webuiconf.h"

using namespace std;

namespace xc::utils {
    string contentsOfTextFile(string filePath) {
        ifstream fin(filePath);
        if (fin.fail()) {
            cerr << "[FileIOError]: " << ::strerror(errno) << endl;
            return "404";
        }
        stringstream buffer;
        buffer << fin.rdbuf();
        string str(buffer.str());
        fin.close();
        return str;
    }

    string mimeTypeOfFile(string filePath) {
        std::filesystem::path p(filePath);
        string ext = p.extension();
        auto cit = conf::fileExtensionToMimeTypes.find(ext);
        if (cit != conf::fileExtensionToMimeTypes.end()) {
            return cit->second;
        }
        cit = conf::fileExtensionToMimeTypes.find("default");
        if (cit != conf::fileExtensionToMimeTypes.end()) {
            return cit->second;
        }
        return "data";
    }
}
