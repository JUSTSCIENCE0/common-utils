# Copyright (c) 2024, Yakov Usoltsev
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

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        # warnings options
        /W4 /WX
        /wd4505 /wd4100
        /DUNICODE /D_UNICODE  # use unicode
        /MP                   # multiple processes build
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

set(INTERFACE_HEADERS
    include/parsing-utils.hpp
    include/validation-utils.hpp
    include/cpu-utils.hpp
    include/cli-utils.hpp
    include/profile-utils.hpp
    include/ini-utils.hpp
    include/os-utils.hpp
)

set(IMPLEMENTATION
    include/code-generators/instructions-sets.h
    include/code-generators/cli-parsers.h
    src/log-utils.cpp
    src/profile-utils.cpp
)

add_subdirectory(externals/googletest)
add_subdirectory(externals/mINI)
set_property(TARGET gmock gmock_main gtest gtest_main PROPERTY FOLDER "externals")

add_library(common-utils STATIC)
target_sources(common-utils
    PUBLIC
        ${INTERFACE_HEADERS}
    PRIVATE
        ${IMPLEMENTATION}
)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(EXTERNAL
        include/external/getopt.h
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

if (BUILD_CU_APPS)
    add_subdirectory(apps)
endif(BUILD_CU_APPS)

if (BUILD_CU_TEST)
    add_subdirectory(tests)
endif(BUILD_CU_TEST)

if (ENABLE_CU_PROFILE)
    target_compile_definitions(common-utils PUBLIC ENABLE_CU_PROFILE=1)
endif(ENABLE_CU_PROFILE)
