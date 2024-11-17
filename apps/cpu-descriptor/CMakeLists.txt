# Copyright (c) 2024, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(cpu-descriptor)

add_executable(cpu-descriptor
    main.cpp)

target_link_libraries(cpu-descriptor
    PRIVATE
        common-utils)

set_property(TARGET cpu-descriptor PROPERTY FOLDER "apps")