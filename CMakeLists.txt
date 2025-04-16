cmake_minimum_required(VERSION 3.10)
project(raygates VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

option(USE_CLANG_TIDY "Use clang-tidy for static code analysis" OFF)

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

file(GLOB SOURCES "src/*.cpp" "src/*.hpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES}
        src/raygates.cpp
        src/raygates.h
        external/fastNoiseLite/fastNoiseLite.h
        src/animals/animal.cpp
        src/animals/animal.h
        src/terrain_generator.cpp
        src/terrain_generator.h
        src/animals/rabbit.cpp
        src/animals/rabbit.h
        src/animals/wolf.cpp
        src/animals/wolf.h
        src/plants/plant.cpp
        src/plants/plant.h)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)