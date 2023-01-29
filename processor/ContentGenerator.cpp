//
// Created by xcbosa on 2023/1/28.
//

#include "../webui.h"
#include "ContentGenerator.h"

namespace xc {
    namespace processor {
        ContentGenerator *generators[1024];
        int generatorsCnt = 0;

        ContentGenerator::ContentGenerator(string name, RequestCheckBlock checker, ContentGenerateBlock generator) {
            this->checker = checker;
            this->generator = generator;
            this->name = name;
            int maxCnt = sizeof(generators) / sizeof(ContentGenerator *);
            ostringstream oss;
            oss << "ContentGenerator count must be less than ";
            oss << maxCnt;
            assertx(generatorsCnt < maxCnt, oss.str());
            generators[generatorsCnt++] = this;
            cout << "[ContentGenerator] Registered " << name << endl;
        }

        bool ContentGenerator::matchRequest(RequestData request) {
            return this->checker(request);
        }

        ResponseData *ContentGenerator::generateResponse(RequestData request) {
            return this->generator(request);
        }

        string ContentGenerator::getName() {
            return this->name;
        }
    }
}
