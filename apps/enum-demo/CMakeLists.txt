# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(enum-demo)

add_executable(enum-demo
    main.cpp
)

target_link_libraries(enum-demo
    PRIVATE
        common-utils
)

set_property(TARGET enum-demo PROPERTY FOLDER "apps")
target_interface_group(common-utils)
