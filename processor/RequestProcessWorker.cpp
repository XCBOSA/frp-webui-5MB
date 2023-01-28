//
// Created by xcbosa on 2023/1/28.
//

#include "../webuiconf.h"
#include "RequestProcessWorker.h"
#include "processor.h"

namespace xc {
    namespace processor {

        vector<ContentGenerator *> generators;

        void RequestProcessWorker::workerLoop() {
            while (true) {
                RequestProcessTask task = processor::dequeueTaskSync();
                ContentGenerator *generator = nullptr;
                for (auto it : generators) {
                    if (it->matchRequest(task.getRequest())) {
                        generator = it;
                    }
                }
                ResponseData *resp = nullptr;
                if (generator == nullptr) {
                    resp = new FileResponseData(conf::errorPage404);
                } else {
                    resp = generator->generateResponse(task.getRequest());
                }
                task.processFinish(resp);
            }
        }

    } // xc
} // processor