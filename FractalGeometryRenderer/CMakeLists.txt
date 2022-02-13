cmake_minimum_required (VERSION 3.8)

# Create project
project("FractalGeometryRenderer")

# Add directories
add_subdirectory("FractalGeometryRenderer")

# Enable this using CMake command line argument -RUN_TESTS_DURING_BUILD=true
if(${RUN_TESTS_DURING_BUILD})
	add_subdirectory("FractalGeometryRenderer/test")	
endif()

# Enforce use of C++17 features
target_compile_features(FractalGeometryRenderer PRIVATE cxx_std_17)
