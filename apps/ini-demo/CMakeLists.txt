# Copyright (c) 2024-2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

add_subdirectory(sublibrary-demo)

project(ini-demo)

set(CONFIG_INI ${CMAKE_CURRENT_SOURCE_DIR}/config.ini)

add_executable(ini-demo
    main.cpp
)

target_link_libraries(ini-demo
    PRIVATE
        common-utils
        sublibrary-demo
)

set_property(TARGET ini-demo PROPERTY FOLDER "apps")
target_interface_group(common-utils)

add_custom_command(
    TARGET ini-demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CONFIG_INI}
            $<TARGET_FILE_DIR:ini-demo>
    COMMENT "Copying config.ini to build directory"
)

set_target_properties(sublibrary-demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:ini-demo>
)
