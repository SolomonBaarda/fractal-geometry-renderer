message("\nBuilding test directory\n")

add_compile_definitions(TEST_PATH_TO_KERNELS=\"${TEST_PATH_TO_KERNELS}\")

add_subdirectory("unittest")

# TODO command line interface tests?
