cmake_minimum_required (VERSION 3.8)

# Source files
set(SOURCES "main.cpp" "source/Display.cpp")

# Add sources to this project
add_executable (RealtimeFractalRenderer ${SOURCES})

# Includes
# Main files
target_include_directories(RealtimeFractalRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
# SDL2
target_include_directories(RealtimeFractalRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/SDL2-2.0.16/include")
