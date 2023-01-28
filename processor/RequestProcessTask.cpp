//
// Created by xcbosa on 2023/1/28.
//

#include "RequestProcessTask.h"

namespace xc {
    namespace processor {
        RequestProcessTask::RequestProcessTask(RequestData requestData): request(requestData) {
            this->response = nullptr;
            this->finish = false;
        }

        void RequestProcessTask::processFinish(ResponseData *responseData) {
            this->response = responseData;
            this->finish = true;
            // Todo-Fix: 当Task已脱离
        }

        bool RequestProcessTask::isFinish() {
            return this->isFinish();
        }

        ResponseData *RequestProcessTask::getResponse() {
            return this->response;
        }
    } // xc
} // processor