#project("Testing")

message("\nBuilding test directory\n")

add_subdirectory("unittest")

message("\nRunning command line interface tests: ${r}, ${o}")

#execute_process(COMMAND "${RUNTIME_OUTPUT_DIRECTORY}/RealtimeFractalRenderer.exe" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE r OUTPUT_VARIABLE o)
#message("stat: ${RUNTIME_OUTPUT_DIRECTORY}")
#message("stat: ${r}, ${o}")
