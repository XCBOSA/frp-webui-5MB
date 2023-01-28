//
// Created by xcbosa on 2023/1/28.
//

#include "ContentGenerator.h"

namespace xc {
    namespace processor {
        extern vector<ContentGenerator *> generators;

        ContentGenerator::ContentGenerator(RequestCheckBlock checker, ContentGenerateBlock generator) {
            this->checker = checker;
            this->generator = generator;
            generators.push_back(this);
        }

        bool ContentGenerator::matchRequest(RequestData request) {
            return this->checker(request);
        }

        ResponseData *ContentGenerator::generateResponse(RequestData request) {
            return this->generator(request);
        }
    }
}
