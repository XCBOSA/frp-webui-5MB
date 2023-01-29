cmake_minimum_required(VERSION 3.24)
project(FRPCWebUI)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRCS)
aux_source_directory(controller DIR_CONTROLLERS)

add_subdirectory(utils)
add_subdirectory(httpserver)
add_subdirectory(processor)

add_executable(FRPCWebUI ${DIR_SRCS} ${DIR_CONTROLLERS})

find_package(Threads)
target_link_libraries(FRPCWebUI PRIVATE HttpServerUtils HttpServer Processor ${CMAKE_THREAD_LIBS_INIT})
