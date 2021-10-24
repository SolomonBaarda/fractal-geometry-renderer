cmake_minimum_required (VERSION 3.16)
project (rayMarcher)

include_directories ("${PROJECT_SOURCE_DIR}/source")

add_executable (rayMarcher "source/main.cpp" "source/shapes.h"  "source/Camera.h" "source/Renderer.h" "source/Utils.h" "source/Vector3.h")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(rayMarcher PUBLIC OpenMP::OpenMP_CXX)
endif()

add_definitions(${COMPILE_FLAGS})