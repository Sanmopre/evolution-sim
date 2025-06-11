if(RUN_CONAN_INSTALL)

    set(CONANFILE ${PROJECT_SOURCE_DIR}/conanfile.py)
    set(CONAN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set(CONAN_TOOLCHAIN_FILE ${CONAN_OUTPUT_DIR}/conan_toolchain.cmake)

    if(NOT EXISTS ${CONAN_TOOLCHAIN_FILE})

        find_program(_conan_exec conan REQUIRED)

        set_property(
                DIRECTORY ${PROJECT_SOURCE_DIR}
                APPEND
                PROPERTY CMAKE_CONFIGURE_DEPENDS ${CONANFILE}
        )

        execute_process(
                COMMAND ${_conan_exec} install ${CONANFILE}
                --output-folder=${CONAN_OUTPUT_DIR}
                --build=missing
                --profile=default
                --settings=build_type=${CMAKE_BUILD_TYPE}
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    include(${CONAN_TOOLCHAIN_FILE})
    list(PREPEND CMAKE_PREFIX_PATH "${CONAN_OUTPUT_DIR}/generators")

else()
    include(${PROJECT_BINARY_DIR}/conan_toolchain.cmake)
endif()