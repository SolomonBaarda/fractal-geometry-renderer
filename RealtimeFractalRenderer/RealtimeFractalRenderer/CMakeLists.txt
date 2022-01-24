cmake_minimum_required(VERSION 3.8)

# Find packages
find_package(OpenCL 2.2 REQUIRED)
find_package(OpenMP 2.0 REQUIRED)
find_package(SDL2 2.0 REQUIRED)
find_package(imgui REQUIRED) # TODO force version 1.85

# Add sources to this project
set(SOURCES "main.cpp" "source/Window.cpp" "source/Renderer.cpp" )
add_executable(RealtimeFractalRenderer ${SOURCES})

# Enforce use of C++17 features
target_compile_features(RealtimeFractalRenderer PRIVATE cxx_std_17)

# Includes
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/source/include" "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
target_include_directories(RealtimeFractalRenderer PRIVATE ${INCLUDE_DIRS})

# Linking
target_link_libraries(RealtimeFractalRenderer PRIVATE OpenCL::OpenCL)
target_link_libraries(RealtimeFractalRenderer PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(RealtimeFractalRenderer PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(RealtimeFractalRenderer PRIVATE imgui::imgui)