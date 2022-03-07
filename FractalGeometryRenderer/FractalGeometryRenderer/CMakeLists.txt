include("buildAndLink.cmake" REQUIRED)

build_and_link(${CMAKE_PROJECT_NAME} "main.cpp" ".")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/runBenchmarks.bat" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}/runBenchmarks.bat")