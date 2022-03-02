# Flags used:
# -DBUILD_TESTS true/false
# -DTEST_PATH_TO_KERNELS "path"

# -DNO_GUI_BUILD true/false

# -DGENERATE_DOXYGEN true/false

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
endif()
