set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRC)

add_library(HttpServer ${DIR_SRC})
