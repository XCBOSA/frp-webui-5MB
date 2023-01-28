//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "processor-private.h"
#include "RequestProcessTask.h"

namespace xc {
    namespace processor {
        /*由 ClientConnection 放入请求*/
        void enqueueTask(RequestProcessTask *task);

        /*当该请求超时，则由 ClientConnection 负责调用此方法删除请求*/
        void deleteTask(RequestProcessTask *task);

        /*从处理队列中取出一个，如果没有则等待*/
        RequestProcessTask dequeueTaskSync();
    }
}
