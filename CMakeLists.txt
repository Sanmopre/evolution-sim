cmake_minimum_required(VERSION 3.10)
project(raygates VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

option(USE_CLANG_TIDY "Use clang-tidy for static code analysis" ON)
option(USE_CLANG_FORMAT "Enable custom target for clang-format" ON)
option(BUILD_TESTS "Build the unit tests" ON)

if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy;--fix-errors)
endif()

include_directories("src/" "external/")

add_subdirectory("external/raylib-5.5")

configure_file(
  "${PROJECT_SOURCE_DIR}/src/global.h.in"
  "${PROJECT_BINARY_DIR}/src/global.h"
)

include_directories("${PROJECT_BINARY_DIR}/src/" "external/raylib-5.5/src")

file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(USE_CLANG_FORMAT)
    # Custom target for Clang-Format
    add_custom_target(
        clang-format
        COMMAND clang-format
        -i
        -style=file
        ${SOURCES}
    )
endif()

if(BUILD_TESTS)
    # Enable testing with CTest
    enable_testing()

    # Find Google Test package
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Define a test executable
    add_executable(test_app test/test.cpp)

    # Link Google Test to the test executable
    target_link_libraries(test_app ${GTEST_LIBRARIES} pthread)

    # Add the test executable to CTest
    add_test(NAME test_app COMMAND test_app)
endif()