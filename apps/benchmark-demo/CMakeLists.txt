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

string(LENGTH "${CMAKE_SOURCE_DIR}" ROOT_SOURCE_DIR_LENGTH)
string(LENGTH "${CMAKE_CURRENT_SOURCE_DIR}" SOURCE_DIR_LENGTH)

get_target_property(existing_defs common-utils COMPILE_DEFINITIONS)
list(REMOVE_ITEM existing_defs CU_PREFIX_LENGTH=${ROOT_SOURCE_DIR_LENGTH})
list(APPEND existing_defs CU_PREFIX_LENGTH=${SOURCE_DIR_LENGTH})
set_target_properties(common-utils PROPERTIES COMPILE_DEFINITIONS "${existing_defs}")
