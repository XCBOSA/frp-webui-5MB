cmake_minimum_required(VERSION 3.24)
project(FRPCWebUI)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRCS)

add_subdirectory(utils)
add_subdirectory(httpserver)

add_executable(FRPCWebUI ${DIR_SRCS})

find_package(Threads)
target_link_libraries(FRPCWebUI HttpServerUtils HttpServer ${CMAKE_THREAD_LIBS_INIT})
