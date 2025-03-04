add_subdirectory(thirdparty)

add_library(ZipMaker SHARED "main.cpp")
target_include_directories(ZipMaker PRIVATE thirdparty/zip/src)

target_link_libraries(ZipMaker zip)