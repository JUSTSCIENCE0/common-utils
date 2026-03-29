# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

function(macrojson_generate_header_cu_ex
    descr_file
    output_file
    create_objects
)
    if (create_objects)
        set(MJSON_DEFINE_OBJECT 1)
    else()
        set(MJSON_DEFINE_OBJECT 0)
    endif()

    set(MJSON_HEADER_TEMPLATE
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/mjson-enum-extention.template)
    set(MJSON_OUTPUT_FILE
        ${CMAKE_CURRENT_LIST_DIR}/generated/${output_file})
    set(MJSON_DESCR_FILE ${descr_file})
    configure_file(
        ${MJSON_HEADER_TEMPLATE}
        ${MJSON_OUTPUT_FILE}
        @ONLY
    )
endfunction()