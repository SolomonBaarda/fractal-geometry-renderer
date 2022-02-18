include(CTest)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Creates a new test executable from the file specified
function(create_test_executable EXECUTABLE_NAME TEST_FILE)
    add_executable(${EXECUTABLE_NAME} ${TEST_FILE})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${GTEST_BOTH_LIBRARIES})
    add_test(${EXECUTABLE_NAME} ${EXECUTABLE_NAME})
endfunction()

message("\nBuilding unit tests:")

create_test_executable("test1" "test1.cpp")
create_test_executable("test2" "test2.cpp")
