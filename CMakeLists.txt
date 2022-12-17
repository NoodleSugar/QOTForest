cmake_minimum_required(VERSION 3.22.0)
project(
	QOTForest
		VERSION 0.1.0
		DESCRIPTION "Quadtree / Octree library for C++"
		LANGUAGES CXX
)

include(CTest)

# QOTForest Library

add_library(${PROJECT_NAME} STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(${PROJECT_NAME}
	PRIVATE
		src/internal/BitVector.cpp
)

# AFECS Tests

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(tests)
endif ()

enable_testing()

if(RUN_TESTS)
    run_tests(runQOTForestTests QOTForestTests)
endif()