# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(math-test)

add_executable(math-test
    main.cpp
)

target_link_libraries(math-test
    PRIVATE
        GTest::gtest
        common-utils
)

set_property(TARGET math-test PROPERTY FOLDER "tests")
target_interface_group(common-utils)