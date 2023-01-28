//
// Created by xcbosa on 2023/1/28.
//

#include "RequestProcessTask.h"

namespace xc {
    namespace processor {
        RequestProcessTask::RequestProcessTask(RequestData requestData): request(requestData) {
            this->response = nullptr;
            this->finish = false;
            this->httpDiscarded = false;
        }

        void RequestProcessTask::processFinish(ResponseData *responseData) {
            this->response = responseData;
            this->finish = true;
            if (this->httpDiscarded) {
                delete responseData;
                this->response = nullptr;
                //delete this; // Todo: Memory Lake
            }
        }

        bool RequestProcessTask::isHttpDiscarded() {
            return this->httpDiscarded;
        }

        void RequestProcessTask::setHttpDiscarded(bool value) {
            this->httpDiscarded = value;
        }

        bool RequestProcessTask::isFinish() {
            return this->finish;
        }

        ResponseData *RequestProcessTask::getResponse() {
            return this->response;
        }

        RequestData RequestProcessTask::getRequest() const {
            return this->request;
        }
    } // xc
} // processor