if(RUN_CONAN_INSTALL)

    set(CONANFILE ${PROJECT_SOURCE_DIR}/conanfile.py)

    # Handle build type detection robustly (multi-config generators like Visual Studio)
    if(CMAKE_CONFIGURATION_TYPES)
        if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
            set(CONAN_BUILD_TYPE "Debug") # default for CLion/MSVC
        else()
            set(CONAN_BUILD_TYPE ${CMAKE_INSTALL_CONFIG_NAME})
        endif()
    elseif(CMAKE_BUILD_TYPE)
        set(CONAN_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    else()
        set(CONAN_BUILD_TYPE "RelWithDebInfo")
    endif()

    message(STATUS "Resolved CONAN_BUILD_TYPE: ${CONAN_BUILD_TYPE}")

    # Conan output directory
    set(CONAN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CONAN_BUILD_TYPE})
    set(CONAN_TOOLCHAIN_FILE ${CONAN_OUTPUT_DIR}/conan_toolchain.cmake)

    if(NOT EXISTS ${CONAN_TOOLCHAIN_FILE})

        find_program(_conan_exec conan REQUIRED)

        # Ensure reconfigure if conanfile.py changes
        set_property(
                DIRECTORY ${PROJECT_SOURCE_DIR}
                APPEND
                PROPERTY CMAKE_CONFIGURE_DEPENDS ${CONANFILE}
        )

        message(STATUS "Running Conan install...")
        execute_process(
                COMMAND ${_conan_exec} install ${CONANFILE}
                --output-folder=${CONAN_OUTPUT_DIR}
                --build=missing
                --profile=default
                --settings=build_type=${CONAN_BUILD_TYPE}
                --settings=compiler.cppstd=17
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    # Load Conan's generated toolchain + targets
    include(${CONAN_TOOLCHAIN_FILE})
    list(PREPEND CMAKE_PREFIX_PATH "${CONAN_OUTPUT_DIR}/generators")

else()
    include(${PROJECT_BINARY_DIR}/conan_toolchain.cmake)
endif()