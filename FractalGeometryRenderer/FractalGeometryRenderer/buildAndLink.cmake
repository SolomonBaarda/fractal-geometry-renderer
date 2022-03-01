# Find packages
find_package(OpenCL 2.2 REQUIRED)
find_package(OpenMP 2.0 REQUIRED)
find_package(SDL2 2.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

# Find test build packages
if(${BUILD_TESTS})
	include(CTest REQUIRED)
	find_package(GTest REQUIRED)
endif()

# Add compiler flag if we don't want a GUI
if(${NO_GUI_BUILD})
	add_compile_definitions(NO_GUI_BUILD)
endif()

function (build_and_link EXECUTABLE_NAME ADDITIONAL_SOURCES PATH_TO_THIS_DIRECTORY)
	# Set source and include variables
	set(SOURCES ${SOURCES} ${ADDITIONAL_SOURCES} "${PATH_TO_THIS_DIRECTORY}/source/Window.cpp" "${PATH_TO_THIS_DIRECTORY}/source/Renderer.cpp")
	set(INCLUDE_DIRS "${PATH_TO_THIS_DIRECTORY}/source" "${PATH_TO_THIS_DIRECTORY}/source/include" "${PATH_TO_THIS_DIRECTORY}/libraries")

	message ("\nBuilding executable ${EXECUTABLE_NAME} with sources ${SOURCES} and includes ${INCLUDE_DIRS}")

	# Add sources to this project
	add_executable(${EXECUTABLE_NAME} ${SOURCES})

	# Enforce use of C++17 features
	target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

	# Add includes
	target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_DIRS})
	# Add test includes
	if(${BUILD_TESTS})
	    include_directories(${EXECUTABLE_NAME} PRIVATE ${GTEST_INCLUDE_DIRS})
	endif()

	# Linking
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenCL::OpenCL)
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenMP::OpenMP_CXX)
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL2::SDL2)
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE Eigen3::Eigen)
	# Link test libraries
	if(${BUILD_TESTS})
	    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${GTEST_BOTH_LIBRARIES})
	endif()

	add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/kernels" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/kernels")

	#add_library(${EXECUTABLE_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/kernels")

	#add_custom_target(copy-scenes ALL
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/kernels" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/kernels"
	#	DEPENDS ${EXECUTABLE_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/kernels")

	#add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/kernels"
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/kernels" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/kernels"
	#	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/kernels")

	#ADD_CUSTOM_TARGET(CopyScenes ALL 
	#	DEPENDS "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/kernels"

	#ADD_LIBRARY(${EXECUTABLE_NAME} ${LIBRARY_TYPE} ${TARGET_SOURCES} ${TARGET_OPTIONS})
	#ADD_DEPENDENCIES(${EXECUTABLE_NAME} CopyScenes)
#)

endfunction()
