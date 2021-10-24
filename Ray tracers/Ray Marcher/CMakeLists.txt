cmake_minimum_required (VERSION 3.16)
project (RealtimeFractalRenderer)

include_directories ("${PROJECT_SOURCE_DIR}/source")

add_executable (RealtimeFractalRenderer "source/main.cpp" "source/shapes.h"  "source/Camera.h" "source/Renderer.h" "source/Utils.h" "source/Vector3.h" "source/Window.h")

find_package(OpenMP REQUIRED)
target_link_libraries(RealtimeFractalRenderer PUBLIC OpenMP::OpenMP_CXX)

find_package(SDL2 REQUIRED)
target_link_libraries(RealtimeFractalRenderer PRIVATE SDL2::SDL2 SDL2::SDL2main)

add_definitions(${COMPILE_FLAGS})