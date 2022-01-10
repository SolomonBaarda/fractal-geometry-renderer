cmake_minimum_required(VERSION 3.8)

# Add SDL2
find_package(SDL2 2.0.16 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add OpenCL
find_package(OpenCL 2.2 REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

find_package(OpenMP REQUIRED)

# Add sources to this project
set(SOURCES "main.cpp" "source/Window.cpp" "source/Renderer.cpp")
add_executable (RealtimeFractalRenderer ${SOURCES})

# Enforce use of C++17 features
target_compile_features(RealtimeFractalRenderer PRIVATE cxx_std_17)

# Includes
target_include_directories(RealtimeFractalRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Linking
target_link_libraries(RealtimeFractalRenderer ${SDL2_LIBRARIES})
target_link_libraries(RealtimeFractalRenderer ${OpenCL_LIBRARY})
target_link_libraries(RealtimeFractalRenderer OpenMP::OpenMP_CXX)

