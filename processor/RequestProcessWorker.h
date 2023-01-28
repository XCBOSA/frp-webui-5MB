//
// Created by xcbosa on 2023/1/28.
//

#pragma once
#include "processor-private.h"
#include "ContentGenerator.h"

namespace xc {
    namespace processor {

        class RequestProcessWorker {
        public:
            void workerLoop();
        };

    } // xc
} // processor
