# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(id-test)

add_executable(id-test
    main.cpp
)

target_link_libraries(id-test
    PRIVATE
        GTest::gtest
        common-utils
)

set_property(TARGET id-test PROPERTY FOLDER "tests")
target_interface_group(common-utils)