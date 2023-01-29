set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRC)
aux_source_directory(templates DIR_Templates)

add_library(Processor ${DIR_SRC} ${DIR_Templates} templates/TemplateResponseData.cpp)
