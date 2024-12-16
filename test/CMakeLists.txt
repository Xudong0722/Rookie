file(GLOB EAST_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")
######################################################################################################################
# DEPENDENCIES
######################################################################################################################

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# "make server client ..."
##########################################
foreach (east_test_source ${EAST_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(east_test_filename ${east_test_source} NAME)
    string(REPLACE ".cpp" "" east_test_name ${east_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${east_test_name} EXCLUDE_FROM_ALL ${east_test_source})
    add_dependencies(build-tests ${east_test_name})
    add_dependencies(check-tests ${east_test_name})

    target_link_libraries(${east_test_name} east_shared)

    # Set test target properties and dependencies.
    set_target_properties(${east_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        COMMAND ${east_test_name}
    )

endforeach(east_test_source ${EAST_TEST_SOURCES})