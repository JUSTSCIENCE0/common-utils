# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(common-utils LANGUAGES C CXX)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "common_utils: applications and tests are disabled in submodule")
    set(BUILD_CU_APPS OFF)
    set(BUILD_CU_TEST OFF)
endif()

option(BUILD_CU_APPS        "Build example applications" ON)
option(BUILD_CU_TEST        "Build tests" ON)

option(ENABLE_CU_PROFILE    "Enable profile utils" OFF)
option(ENABLE_CU_TEST_UTILS "Enable test utils"    OFF)

if (PROJECT_IS_TOP_LEVEL)
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(
            # warnings options
            /W4 /WX
            /wd4505 /wd4100
            /DUNICODE /D_UNICODE  # use unicode
            /MP                   # multiple processes build
            /Zc:preprocessor      # __VA_ARG__ support
        )
    elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        add_compile_options(
            # warnings options
            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
            -Wdisabled-optimization -Wformat=2 -Winit-self 
            -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual
            -Wredundant-decls -Wshadow -Wsign-promo 
            -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros
            -Wno-parentheses -Wno-unused-parameter -fdiagnostics-show-option
            -Werror
        )
    endif() # GCC & Clang

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(INTERFACE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/parsing-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/validation-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/cpu-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/cli-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/ini-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/file-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/simd-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/macro-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/profile-utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/cu/math-utils.hpp
)

if (ENABLE_CU_TEST_UTILS)
    set(INTERFACE_HEADERS
        ${INTERFACE_HEADERS}
        ${CMAKE_CURRENT_LIST_DIR}/include/cu/test-utils.hpp)
endif(ENABLE_CU_TEST_UTILS)

set(IMPLEMENTATION
    include/cu/code-generators/instructions-sets.h
    include/cu/code-generators/cli-parsers.h
    include/cu/code-generators/macro-helpers.h
    src/log-utils.cpp
    src/profile-utils.cpp
)

include(FetchContent)
FetchContent_Declare(
    mINI
    GIT_REPOSITORY https://github.com/metayeti/mINI.git
    GIT_TAG        52b66e987cb56171dc91d96115cdf094b6e4d7a0 # 0.9.18
)
FetchContent_MakeAvailable(mINI)

# googletest
if (ENABLE_CU_TEST_UTILS OR BUILD_CU_TEST)
    if (NOT TARGET gtest)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
        )
        FetchContent_MakeAvailable(googletest)
        set_property(TARGET gmock gmock_main gtest gtest_main PROPERTY FOLDER "externals")
    endif()
endif()

add_library(common-utils STATIC)
target_sources(common-utils
    PUBLIC
        ${INTERFACE_HEADERS}
    PRIVATE
        ${IMPLEMENTATION}
)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(EXTERNAL
        include/cu/external/getopt.h
        src/external/getopt_long.c
    )

    target_sources(common-utils
        PRIVATE
            ${EXTERNAL}
    )

    source_group("External" FILES ${EXTERNAL})
endif() # MSVC

source_group("Interface" FILES ${INTERFACE_HEADERS})
source_group("Implementation" FILES ${IMPLEMENTATION})

target_include_directories(common-utils
    PUBLIC
        include/
)

# CPU identifiers
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
    message(STATUS "CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
    target_compile_definitions(common-utils PUBLIC CU_ARCH_X86_64)
#elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")  TODO
#elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")      TODO
else()
    message(FATAL_ERROR "Unsupported CPU Architecture - ${CMAKE_SYSTEM_PROCESSOR}")
endif()

string(LENGTH "${CMAKE_SOURCE_DIR}" SOURCE_DIR_LENGTH)
target_compile_definitions(common-utils PUBLIC CU_PREFIX_LENGTH=${SOURCE_DIR_LENGTH})

# Embellishments
set_property(TARGET common-utils PROPERTY INCLUDE_DIR_POSTFIX "cu")
function(target_interface_group target_name)
    if (NOT target_name)
        message(FATAL_ERROR "You must provide a target name")
    endif()

    get_target_property(TARGET_INTERFACE_HEADERS ${target_name} INTERFACE_SOURCES)
    get_target_property(TARGET_INTERFACE_INCLUDE_DIR ${target_name} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(TARGET_INCLUDE_DIR_POSTFIX ${target_name} INCLUDE_DIR_POSTFIX)

    if (TARGET_INTERFACE_HEADERS AND TARGET_INTERFACE_INCLUDE_DIR)
        if (TARGET_INCLUDE_DIR_POSTFIX)
            source_group(
                TREE ${TARGET_INTERFACE_INCLUDE_DIR}/${TARGET_INCLUDE_DIR_POSTFIX}
                PREFIX ${target_name}
                FILES ${TARGET_INTERFACE_HEADERS})
        else(TARGET_INCLUDE_DIR_POSTFIX)
            source_group(
                TREE ${TARGET_INTERFACE_INCLUDE_DIR}
                FILES ${TARGET_INTERFACE_HEADERS})
        endif(TARGET_INCLUDE_DIR_POSTFIX)
    endif()
endfunction()

if (BUILD_CU_APPS)
    add_subdirectory(apps)
endif(BUILD_CU_APPS)

if (BUILD_CU_TEST)
    add_subdirectory(tests)
endif(BUILD_CU_TEST)

if (ENABLE_CU_PROFILE)
    target_compile_definitions(common-utils PUBLIC ENABLE_CU_PROFILE=1)
endif(ENABLE_CU_PROFILE)

if (ENABLE_CU_TEST_UTILS)
    target_compile_definitions(common-utils PUBLIC ENABLE_CU_TEST_UTILS=1)
    target_link_libraries(common-utils
        PUBLIC
            GTest::gtest
    )
endif(ENABLE_CU_TEST_UTILS)
