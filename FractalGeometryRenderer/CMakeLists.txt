# Flags used:
# -DBUILD_TESTS true/false
# -DTEST_PATH_TO_KERNELS "path"

# -DNO_GUI_BUILD true/false

cmake_minimum_required (VERSION 3.8)

# Create project
project("FractalGeometryRenderer")

# Copy scenes to the build directory
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/FractalGeometryRenderer/kernels" DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

# Enable this using CMake command line argument -RBUILD_TESTS=true
if(${BUILD_TESTS})
	# Build tests
	enable_testing()
	add_subdirectory("FractalGeometryRenderer/test")
else()
	# Build application
	add_subdirectory("FractalGeometryRenderer")
endif()
