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
