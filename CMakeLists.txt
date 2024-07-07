cmake_minimum_required(VERSION 3.10)
project(app VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Enable clang-tidy
set(CMAKE_CXX_CLANG_TIDY clang-tidy;--fix-errors)
include_directories("src/" "external/") 

configure_file(
  "${PROJECT_SOURCE_DIR}/src/global.h.in"
  "${PROJECT_BINARY_DIR}/src/global.h"
)

include_directories("${PROJECT_BINARY_DIR}")

file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Custom target for Clang-Format
add_custom_target(
    clang-format
    COMMAND clang-format
    -i
    -style=file
    ${SOURCES}
)