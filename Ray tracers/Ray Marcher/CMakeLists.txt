cmake_minimum_required (VERSION 3.16)
project (rayMarcher)

include_directories ("${PROJECT_SOURCE_DIR}/source")

add_executable (rayMarcher "source/smallpt.cpp" "source/shapes.h" "source/render.h" "source/camera.h")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(rayMarcher PUBLIC OpenMP::OpenMP_CXX)
endif()

add_definitions(${COMPILE_FLAGS})