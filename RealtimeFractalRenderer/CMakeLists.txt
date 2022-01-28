cmake_minimum_required (VERSION 3.8)

# Create project
project ("RealtimeFractalRenderer")

# Add directories
add_subdirectory ("RealtimeFractalRenderer")

# Run the unit tests instead
# Enable this using CMake command line argument -DRUN_UNIT_TESTS=true
if(${RUN_UNIT_TESTS})
	enable_testing()
	add_subdirectory ("RealtimeFractalRenderer/test")

	# Change the executable name for clarity
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "TestRealtimeFractalRenderer")
endif()

# Enforce use of C++17 features
target_compile_features(RealtimeFractalRenderer PRIVATE cxx_std_17)
