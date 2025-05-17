# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(cli-demo)

add_executable(cli-demo
    main.cpp
)

target_link_libraries(cli-demo
    PRIVATE
        common-utils
)

set_property(TARGET cli-demo PROPERTY FOLDER "apps")
target_interface_group(common-utils)
