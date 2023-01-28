//
// Created by xcbosa on 2023/1/28.
//

#pragma once

#include "processor-private.h"
#include "../utils/utils.h"

using namespace std;

namespace xc {
    namespace processor {

        /*输入一个请求，判断此生成器是否响应此请求*/
        typedef function<bool (RequestData)> RequestCheckBlock;

        /*为此请求生成响应，响应需要使用new，在发送完成或HTTP服务器不再需要时自动调用delete销毁*/
        typedef function<ResponseData* (RequestData)> ContentGenerateBlock;

        /*请定义为static*/
        class ContentGenerator {
        public:
            ContentGenerator(RequestCheckBlock checker, ContentGenerateBlock generator);
            bool matchRequest(RequestData request);
            ResponseData *generateResponse(RequestData request);
        private:
            RequestCheckBlock checker;
            ContentGenerateBlock generator;
        };

    }
}
