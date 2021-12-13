cmake_minimum_required(VERSION 3.8)

#set (CMAKE_CXX_STANDARD 11)

# Add SDL2
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libraries/SDL2-2.0.18")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add OpenCL
find_package(OpenCL 2.2 EXACT REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

# Add sources to this project
set(SOURCES "main.cpp" "source/Display.cpp" "source/Renderer.cpp")
add_executable (RealtimeFractalRenderer ${SOURCES})

# Includes
target_include_directories(RealtimeFractalRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Linking
target_link_libraries(RealtimeFractalRenderer ${SDL2_LIBRARIES})
target_link_libraries(RealtimeFractalRenderer ${OpenCL_LIBRARY})
