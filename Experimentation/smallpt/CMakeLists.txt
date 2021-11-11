cmake_minimum_required (VERSION 3.16)
project (smallpt)

include_directories ("${PROJECT_SOURCE_DIR}/source")

add_executable (smallpt "source/smallpt.cpp")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(smallpt PUBLIC OpenMP::OpenMP_CXX)
endif()

SET(COMPILE_FLAGS "-O3 -fopenmp")
add_definitions(${COMPILE_FLAGS})