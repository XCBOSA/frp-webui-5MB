#!/usr/bin/python3

# 使用此脚本将data/html下的代码转换成写死的Controller

import os
import sys
import datetime

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: Generate-Assets <AssetFilePath>")
        exit(-1)
        pass
    assetPath = sys.argv[1]
    with open("html/" + assetPath, "r", encoding="utf-8") as f:
        fileTexts = []
        for it in f.readlines():
            lineText = it.replace("\\", "\\\\").replace("\n", "\\n").replace("\r", "").replace("\'", "\\'").replace("\"", "\\\"")
            lineText = "            string(\"" + lineText + "\")"
            fileTexts.append(lineText)
        fileText = " + \n".join(fileTexts)
        if len(fileTexts) == 0:
            fileText = "\"\""
        writeToFilePath = "../controller/" + assetPath.replace("/", "-") + ".cpp"
        print("Writing to " + writeToFilePath)
        with open(writeToFilePath, "w+", encoding="utf-8") as w:
            w.write("//\n")
            w.write("// Created by xcbosa on " + str(datetime.date.today()) + "\n")
            w.write("//\n")
            w.write("\n")
            w.write("#include \"../processor/processor.h\"\n")
            w.write("#include \"../utils/utils.h\"\n")
            w.write("#include \"../webuiconf.h\"\n")
            w.write("\n")
            w.write("using namespace std;\n")
            w.write("using namespace xc::processor;\n")
            w.write("using namespace xc::utils;\n")
            w.write("using namespace xc::processor::templates;\n")
            w.write("\n")
            w.write("namespace xc::controller {\n")
            w.write("\n")
            w.write("    static string ControllerPath = \"/" + assetPath + "\";\n\n")
            w.write("    static ResponseData *controllerResponse(RequestData request) {\n")
            w.write("        return new TextResponseData(200,\n")
            w.write(fileText + "\n")
            w.write("        , mimeTypeOfFile(ControllerPath));\n")
            w.write("    }\n\n")
            w.write("    ContentGeneratorDefineS(request.getURL() == ControllerPath, controllerResponse(request))\n")
            w.write("\n")
            w.write("}\n")

