//
// Created by xcbosa on 2023/1/28.
//

#include "RequestProcessorManager.h"

namespace xc {
    namespace processor {
        auto taskQueue = deque<RequestProcessTask *>();
        auto taskQueueMutex = mutex();

        void enqueueTask(RequestProcessTask *task) {
            taskQueueMutex.lock();
            taskQueue.push_back(task);
            taskQueueMutex.unlock();
        }

        void deleteTask(RequestProcessTask *task) {
            taskQueueMutex.lock();
            std::remove_if(taskQueue.begin(), taskQueue.end(), [task](RequestProcessTask *t) { return task == t; });
            taskQueueMutex.unlock();
        }

        RequestProcessTask dequeueTaskSync() {
            while (true) {
                taskQueueMutex.lock();
                if (!taskQueue.empty()) {
                    RequestProcessTask task = *taskQueue.front();
                    taskQueue.pop_front();
                    taskQueueMutex.unlock();
                    return task;
                }
                taskQueueMutex.unlock();
                usleep(1000 * 10);
            }
        }
    }
}
