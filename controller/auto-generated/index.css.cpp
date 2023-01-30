//
// Created by xcbosa on 2023-01-30
//

#include "../../processor/processor.h"
#include "../../utils/utils.h"
#include "../../webuiconf.h"

using namespace std;
using namespace xc::processor;
using namespace xc::utils;
using namespace xc::processor::templates;

namespace xc::controller {

    static string ControllerPath = "/index.css";

    static ResponseData *controllerResponse(RequestData request) {
        return new TextResponseData(200,
""
        , mimeTypeOfFile(ControllerPath));
    }

    ContentGeneratorDefineS(request.getURL() == ControllerPath, controllerResponse(request))

}
