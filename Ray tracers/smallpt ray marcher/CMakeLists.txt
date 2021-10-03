cmake_minimum_required (VERSION 3.16)
project (rayMarcher)

include_directories ("${PROJECT_SOURCE_DIR}/source")

add_executable (rayMarcher "source/smallpt.cpp" "source/utils.h" "source/signedDistanceFunctions.h" "source/shapes.h")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(rayMarcher PUBLIC OpenMP::OpenMP_CXX)
endif()

SET(COMPILE_FLAGS "-O3 -fopenmp")
add_definitions(${COMPILE_FLAGS})