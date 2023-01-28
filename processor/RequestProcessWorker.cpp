//
// Created by xcbosa on 2023/1/28.
//

#include "../webuiconf.h"
#include "RequestProcessWorker.h"
#include "processor.h"

namespace xc {
    namespace processor {

        void RequestProcessWorker::workerLoop() {
            while (true) {
                RequestProcessTask *task = processor::dequeueTaskSync();
                ContentGenerator *generator = nullptr;
                for (int i = 0; i < generatorsCnt; i++) {
                    auto it = generators[i];
                    if (it->matchRequest(task->getRequest())) {
                        generator = it;
                    }
                }
                ResponseData *resp = nullptr;
                if (generator == nullptr) {
                    resp = new FileResponseData(conf::errorPage404);
                } else {
                    resp = generator->generateResponse(task->getRequest());
                }
                task->processFinish(resp);
            }
        }

    } // xc
} // processor