# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

function(capitalize_first_letter
    INPUT
    OUTPUT
)
    if(NOT INPUT)
        set(${OUTPUT} "" PARENT_SCOPE)
        return()
    endif()

    string(SUBSTRING "${INPUT}" 0 1 first_char)
    string(SUBSTRING "${INPUT}" 1 -1 rest)
    string(TOUPPER "${first_char}" first_upper)
    set(result "${first_upper}${rest}")
    set(${OUTPUT} "${result}" PARENT_SCOPE)
endfunction()