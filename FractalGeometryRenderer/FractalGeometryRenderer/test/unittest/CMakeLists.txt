include("../../buildAndLink.cmake" REQUIRED)

# Creates a new test executable from the file specified
function(create_test_executable EXECUTABLE_NAME TEST_FILE)
    build_and_link(${EXECUTABLE_NAME} ${TEST_FILE} "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    add_test(${EXECUTABLE_NAME} ${EXECUTABLE_NAME})
endfunction()

message("\nBuilding unit tests:")

create_test_executable("TestBenchmark" "testBenchmark.cpp")
create_test_executable("TestScene" "testScene.cpp")
create_test_executable("TestTimer" "testTimer.cpp")
create_test_executable("TestCommonScenes" "testCommonScenes.cpp")
create_test_executable("TestResolutions" "testResolutions.cpp")
