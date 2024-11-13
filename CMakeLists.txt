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
option(CU_ENABLE_CPU_UTILS  "Enable CPU Utils" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4 /WX               # warnings options
        /DUNICODE /D_UNICODE  # use unicode
        /MP                   # multiple processes build
    )
elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        # warnings options
        -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy
        -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op
        -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual
        -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel
        -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros
        -Wno-parentheses -fdiagnostics-show-option
        -Werror
    )
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(common-utils STATIC)

if (CU_ENABLE_CPU_UTILS)
    target_sources(common-utils
        PUBLIC
            include/cpu-utils.h
        PRIVATE
            src/cpu-utils.cpp
    )
endif(CU_ENABLE_CPU_UTILS)

if (BUILD_CU_APPS)
    add_subdirectory(apps)
endif(BUILD_CU_APPS)

if (BUILD_CU_TEST)
    add_subdirectory(tests)
endif(BUILD_CU_TEST)