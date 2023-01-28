cmake_minimum_required(VERSION 3.24)
project(FRPCWebUI)

set(CMAKE_CXX_STANDARD 17)

add_executable(FRPCWebUI main.cpp httpserver/HTTPServer.cpp httpserver/HTTPServer.h webui.h httpserver/ClientConnection.cpp httpserver/ClientConnection.h utils/ResponseData.cpp utils/ResponseData.h utils/FileResponseData.cpp utils/FileResponseData.h utils/FileReader.cpp webuiconf.h)
