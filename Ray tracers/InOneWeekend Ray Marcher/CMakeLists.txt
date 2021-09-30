cmake_minimum_required (VERSION 3.16)
project (rayMarcher)

include_directories ("${PROJECT_SOURCE_DIR}/source")
include_directories ("${PROJECT_SOURCE_DIR}/source/common")

add_executable (rayMarcher "source/main.cpp")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(rayMarcher PUBLIC OpenMP::OpenMP_CXX)
endif()