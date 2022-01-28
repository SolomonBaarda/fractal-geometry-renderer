cmake_minimum_required (VERSION 3.8)

# Create project
project ("RealtimeFractalRenderer")

# Add directories
add_subdirectory ("RealtimeFractalRenderer")

# Enable this using CMake command line argument -RUN_TESTS_DURING_BUILD=true
if(${RUN_TESTS_DURING_BUILD})
	add_subdirectory("RealtimeFractalRenderer/test")	
endif()

# Enforce use of C++17 features
target_compile_features(RealtimeFractalRenderer PRIVATE cxx_std_17)
