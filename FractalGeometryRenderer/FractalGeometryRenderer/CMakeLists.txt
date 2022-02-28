include("buildAndLink.cmake" REQUIRED)

build_and_link(${CMAKE_PROJECT_NAME} "main.cpp" ".")
