//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "processor-private.h"

namespace xc {
    namespace processor {

        class RequestProcessTask {
        public:
            RequestProcessTask(RequestData requestData);
            void processFinish(ResponseData *responseData);
            bool isFinish();
            ResponseData *getResponse();
        private:
            bool finish;
            RequestData request;
            ResponseData *response; // 由处理程序创建，由 ClientConnection 在发送后使用 delete 释放
        };

    } // xc
} // processor
