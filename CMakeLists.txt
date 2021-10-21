cmake_minimum_required(VERSION 3.21)

function(run_tests name executable)
    set(_test_state "${PROJECT_BINARY_DIR}/run_tests/${executable}.passed")
    add_custom_command(
        OUTPUT ${_test_state}
        COMMAND $<TARGET_FILE:${executable}>
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/run_tests"
        COMMAND ${CMAKE_COMMAND} -E touch ${_test_state}
        DEPENDS ${executable}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        )
    add_custom_target(
        ${name}
        ALL
        DEPENDS ${_test_state}
        )
endfunction()

project(libQOTForest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
link_directories(${PROJECT_BINARY_DIR})

set(HEADERS
	include/QOTF/morton_code2.h
	include/QOTF/morton_code3.h
	include/QOTF/node2_index.h
	include/QOTF/node3_index.h
	include/QOTF/node_state.h
	include/QOTF/utility.h
	include/QOTF/ntree.h
	include/QOTF/labeled_ntree.h
	)

set(ALL_TESTS
	tests/all_tests.cpp
	tests/tests_morton_code.h
	)

find_package(Catch2 REQUIRED)

add_library(QOTForest ${SRCS} ${HEADERS} ${TEMPLATES})
target_link_libraries(QOTForest Catch2::Catch2)

add_executable(QOTForestTests ${ALL_TESTS})
target_link_libraries(QOTForestTests libQOTForest.a)

if(RUN_TESTS)
    run_tests(runQOTForestTests QOTForestTests)
endif()