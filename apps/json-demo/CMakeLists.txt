# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(json-demo)

macrojson_generate_header_cu_ex(
    example_descr.h
    example.hpp
    TRUE
)

add_executable(json-demo
    generated/example.hpp

    example_descr.h
    main.cpp
)

target_include_directories(json-demo
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/generated
)

target_link_libraries(json-demo
    PRIVATE
        common-utils
)

set_property(TARGET json-demo PROPERTY FOLDER "apps")
target_interface_group(common-utils)
