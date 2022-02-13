cmake_minimum_required (VERSION 3.9)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Create project
project("FractalGeometryRenderer")

if(${DEBUG})
	add_compile_definitions(DEBUG)
endif()

# Add directories
add_subdirectory("FractalGeometryRenderer")

# Enable this using CMake command line argument -RUN_TESTS_DURING_BUILD=true
if(${RUN_TESTS_DURING_BUILD})
	add_subdirectory("FractalGeometryRenderer/test")	
endif()

# Enforce use of C++17 features
target_compile_features(FractalGeometryRenderer PRIVATE cxx_std_17)
