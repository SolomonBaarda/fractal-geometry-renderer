find_package(Boost REQUIRED COMPONENTS unit_test_framework)

enable_testing()
include(CTest)

# Creates a new test executable from the file specified
function(create_test_executable EXECUTABLE_NAME TEST_FILE)
    add_executable(${EXECUTABLE_NAME} ${TEST_FILE})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Boost::boost Boost::unit_test_framework)
    add_test(NAME ${EXECUTABLE_NAME} COMMAND "${EXECUTABLE_NAME}.exe")
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND "ctest" --output-on-failure)
    message("Built ${TEST_FILE} into executable ${EXECUTABLE_NAME}.exe")
endfunction()

message("\nBuilding unit tests:")
create_test_executable("test1" "test1.cpp")
create_test_executable("test2" "test2.cpp")
