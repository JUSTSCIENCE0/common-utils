# Copyright (c) 2024, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(benchmark-demo)

add_executable(benchmark-demo
    main.cpp
    some_class.h
    some_class.cpp
)

target_link_libraries(benchmark-demo
    PRIVATE
        common-utils
)

set_property(TARGET benchmark-demo PROPERTY FOLDER "apps")

string(LENGTH "${CMAKE_CURRENT_SOURCE_DIR}" SOURCE_DIR_LENGTH)
target_compile_definitions(common-utils PUBLIC CU_PREFIX_LENGTH=${SOURCE_DIR_LENGTH})
