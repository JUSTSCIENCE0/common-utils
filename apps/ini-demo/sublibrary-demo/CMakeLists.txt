# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(sublibrary-demo)

add_library(sublibrary-demo SHARED)
target_sources(sublibrary-demo
    PUBLIC
        sublibrary-demo.h
    PRIVATE
        sublibrary-demo.cpp
)

target_link_libraries(sublibrary-demo
    PRIVATE
        common-utils
)

set_property(TARGET sublibrary-demo PROPERTY FOLDER "apps/libs")
target_interface_group(common-utils)
