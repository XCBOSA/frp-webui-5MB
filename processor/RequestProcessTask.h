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

            /*仅当Controller处理完成后调用，如果HTTPServer已经丢弃了这个任务，则delete自己*/
            void processFinish(ResponseData *responseData);

            bool isFinish();
            RequestData getRequest() const;
            ResponseData *getResponse();
            bool isHttpDiscarded();
            void setHttpDiscarded(bool value);
        private:
            bool finish;
            bool httpDiscarded;
            RequestData request;
            ResponseData *response; // 由处理程序创建，由 ClientConnection 在发送后使用 delete 释放
        };

    } // xc
} // processor
