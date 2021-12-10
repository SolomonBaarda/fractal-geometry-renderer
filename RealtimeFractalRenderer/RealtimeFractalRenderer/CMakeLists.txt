cmake_minimum_required (VERSION 3.8)

# Add SDL2
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libraries/SDL2-2.0.18")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add sources to this project
set(SOURCES "main.cpp" "source/Display.cpp")
add_executable (RealtimeFractalRenderer ${SOURCES})

# Includes
target_include_directories(RealtimeFractalRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Linking
target_link_libraries(RealtimeFractalRenderer ${SDL2_LIBRARIES})
