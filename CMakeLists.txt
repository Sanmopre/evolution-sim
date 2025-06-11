cmake_minimum_required(VERSION 3.10)
project(evolution-sim VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# where to find our cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(RUN_CONAN_INSTALL "Use Conan to install dependencies" ON)
option(USE_CLANG_TIDY "Use clang-tidy for static code analysis" OFF)

include(conan)

find_package(raylib REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(EnTT REQUIRED)

if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy;--fix-errors)
endif()

configure_file(
        "${PROJECT_SOURCE_DIR}/src/global.h.in"
        "${PROJECT_BINARY_DIR}/src/global.h"
)

include_directories(
        "${PROJECT_SOURCE_DIR}/src/"
        "${PROJECT_BINARY_DIR}/src/"
        "${PROJECT_SOURCE_DIR}/external/fastNoiseLite/" # For manual FastNoiseLite
)

file(GLOB SOURCES
        "src/*.cpp" "src/*.hpp" "src/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES}
        src/evolution-sim.cpp
        src/evolution-sim.h
        external/fastNoiseLite/fastNoiseLite.h
        src/terrain_generator.cpp
        src/terrain_generator.h
        src/entities/entity_manager.h
        src/entities/entity_manager.cpp
        src/entities/stats.h
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        raylib
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        EnTT::EnTT
)
