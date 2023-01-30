cmake_minimum_required(VERSION 3.24)
project(FRPCWebUI)

set(CMAKE_CXX_STANDARD 17)

execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/data/Generate-Assets.py index.js
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
)

execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/data/Generate-Assets.py index.css
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
)

execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/data/Generate-Assets.py framework7/framework7-bundle.min.js
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
)

execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/data/Generate-Assets.py framework7/framework7-bundle.min.css
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
)

aux_source_directory(. DIR_SRCS)
aux_source_directory(controller DIR_CONTROLLERS)
aux_source_directory(view DIR_VIEW)

add_subdirectory(utils)
add_subdirectory(httpserver)
add_subdirectory(processor)

add_executable(FRPCWebUI ${DIR_SRCS} ${DIR_CONTROLLERS} ${DIR_VIEW})

find_package(Threads)
target_link_libraries(FRPCWebUI PRIVATE HttpServerUtils HttpServer Processor ${CMAKE_THREAD_LIBS_INIT})
