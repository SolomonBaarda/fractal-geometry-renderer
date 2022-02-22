# Flags used:
# -DBUILD_TESTS true/false
# -DNO_GUI_BUILD true/false

cmake_minimum_required (VERSION 3.8)

# Create project
project("FractalGeometryRenderer")

# Enable this using CMake command line argument -RBUILD_TESTS=true
if(${BUILD_TESTS})
	# Build tests
	enable_testing()
	add_subdirectory("FractalGeometryRenderer/test")
else()
	# Build application
	add_subdirectory("FractalGeometryRenderer")

	# Enforce use of C++17 features
	target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
endif()
