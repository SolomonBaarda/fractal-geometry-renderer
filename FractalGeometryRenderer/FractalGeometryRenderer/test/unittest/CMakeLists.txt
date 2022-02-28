include(CTest)
find_package(GTest REQUIRED)
find_package(OpenCL 2.2 REQUIRED)
find_package(OpenMP 2.0 REQUIRED)
find_package(SDL2 2.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

# Includes
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../source" "${CMAKE_CURRENT_SOURCE_DIR}/../../source/include" "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries")


# Creates a new test executable from the file specified
function(create_test_executable EXECUTABLE_NAME TEST_FILE)
    add_executable(${EXECUTABLE_NAME} ${TEST_FILE})

    # Link libraries and includes
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenCL::OpenCL)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Eigen3::Eigen)

    # Enforce use of C++17 features
	target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${INCLUDE_DIRS})
    include_directories(${EXECUTABLE_NAME} PRIVATE ${GTEST_INCLUDE_DIRS})

    add_test(${EXECUTABLE_NAME} ${EXECUTABLE_NAME})
endfunction()

message("\nBuilding unit tests:")

create_test_executable("TestBenchmark" "testBenchmark.cpp")
create_test_executable("TestScene" "testScene.cpp")
create_test_executable("TestTimer" "testTimer.cpp")
create_test_executable("TestCommonScenes" "testCommonScenes.cpp")
