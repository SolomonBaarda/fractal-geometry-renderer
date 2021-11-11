cmake_minimum_required (VERSION 3.16)
project (InOneWeekend)

include_directories ("${PROJECT_SOURCE_DIR}/source")
include_directories ("${PROJECT_SOURCE_DIR}/source/common")

add_executable (InOneWeekend "source/main.cc")
