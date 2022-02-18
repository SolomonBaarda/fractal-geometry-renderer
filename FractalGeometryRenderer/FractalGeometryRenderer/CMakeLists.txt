# Find packages
find_package(OpenCL 2.2 REQUIRED)
find_package(OpenMP 2.0 REQUIRED)
find_package(SDL2 2.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

# Add sources to this project
set(SOURCES "main.cpp" "source/Window.cpp" "source/Renderer.cpp" )
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Includes
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/source/include" "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Linking
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenCL::OpenCL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Eigen3::Eigen)
