# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

# helpers

function(set_simd_compile_unit_flag
    file_path
    instructions_set
)
    if (instructions_set STREQUAL "DEF")
        return()
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(ARCH_FLAG "/arch:${instructions_set}")
    elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        string(TOLOWER ${instructions_set} instructions_set_lower)
        string(REPLACE "_" "." instructions_set_name "${instructions_set_lower}")
        set(ARCH_FLAG "-m${instructions_set_name}")
    endif() # GCC & Clang

    set_source_files_properties(${file_path}
        PROPERTIES
            COMPILE_OPTIONS ${ARCH_FLAG}
    )
endfunction()

function(parse_simd_sets_group
    sets_group
    is_single
    sets_list
)
    string(REGEX MATCH "[()]" HAS_PAREN "${${sets_group}}")
    if (NOT HAS_PAREN)
        set(${is_single} TRUE PARENT_SCOPE)
        return()
    endif()

    set(${is_single} FALSE PARENT_SCOPE)
    string(REGEX MATCH "^[^(]+" SET_NAME "${${sets_group}}")
    string(REGEX REPLACE "^[^(]*\\(([^)]*)\\).*" "\\1" ARGS "${${sets_group}}")
    string(REPLACE "," ";" ARGS "${ARGS}")

    set(${sets_group} ${SET_NAME} PARENT_SCOPE)
    set(${sets_list} ${ARGS} PARENT_SCOPE)
endfunction()

# interface

# For the given implementation of a function or class,
# generates an interface header file (.hpp) and a compilation unit file (.cpp)
# for each supported instruction set.
#
# Arguments:
#  target — the project to which the generated source files will be added
#  implementation — the name of the implementation for which the files are generated
#  logic_unit - The type used for template generation.
#               Supported values:
#               function – supports standalone C-style functions
#               class – supports class-based implementation with inheritance and polymorphism
#  supported_sets — the list of supported instruction sets
#
# Preconditions:
# Files <implementation>_iface.hpp and <implementation>_impl.hpp
# must exist, defining the interface and implementation of the generated
# executable files.
function(generate_simd_compile_units
    target
    implementation
    logic_unit
    #LIST supported_sets 
)
    set(CU_SIMD_SUPPORT_DEF    0)
    set(CU_SIMD_SUPPORT_SSE    0)
    set(CU_SIMD_SUPPORT_SSE2   0)
    set(CU_SIMD_SUPPORT_SSE3   0)
    set(CU_SIMD_SUPPORT_SSSE3  0)
    set(CU_SIMD_SUPPORT_SSE4_1 0)
    set(CU_SIMD_SUPPORT_SSE4_2 0)
    set(CU_SIMD_SUPPORT_AVX    0)
    set(CU_SIMD_SUPPORT_AVX2   0)
    set(CU_SIMD_SUPPORT_AVX512 0)

    set(IMPLEMENTATION_NAME ${implementation})
    if (${logic_unit} MATCHES "function")
        set(COMPILE_UNIT_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-compile-unit.template)
        set(INTERFACE_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-interface.template)
    elseif(${logic_unit} MATCHES "class")
        set(COMPILE_UNIT_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-compile-unit-class.template)
        set(INTERFACE_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-interface-class.template)

        set(CLASS_TYPE "BASE")
        set(CURRENT_SUPPORTED_SET "DEF")
        set(GENERATED_FILE ${CMAKE_CURRENT_LIST_DIR}/generated/${IMPLEMENTATION_NAME}_base.cpp)
        configure_file(${COMPILE_UNIT_TEMPLATE} ${GENERATED_FILE} @ONLY)
        set(GENERATED_FILES ${GENERATED_FILE})
    else()
        message(FATAL_ERROR "unknown logic unit")
    endif()
    set(CLASS_TYPE "DERIVED")

    foreach(supported_set IN LISTS ARGN)
        parse_simd_sets_group(supported_set is_single sets_list)

        set(SUPPORT_FLAG_NAME "CU_SIMD_SUPPORT_${supported_set}")
        set(${SUPPORT_FLAG_NAME} 1)
        set(CURRENT_SUPPORTED_SET ${supported_set})

        # generate compile unit
        string(TOLOWER ${supported_set} supported_set_lower)
        set(GENERATED_FILE ${CMAKE_CURRENT_LIST_DIR}/generated/${IMPLEMENTATION_NAME}_${supported_set_lower}.cpp)
        configure_file(${COMPILE_UNIT_TEMPLATE} ${GENERATED_FILE} @ONLY)
        set(GENERATED_FILES ${GENERATED_FILES} ${GENERATED_FILE})

        if (is_single OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set_simd_compile_unit_flag(${GENERATED_FILE} ${supported_set})
        else()
            foreach(set_postfix IN LISTS sets_list)
                set(full_set_name "${supported_set}${set_postfix}")
                set_simd_compile_unit_flag(${GENERATED_FILE} ${full_set_name})
            endforeach()
        endif()
    endforeach()

    # generate interface header
    set(GENERATED_FILE ${CMAKE_CURRENT_LIST_DIR}/generated/${IMPLEMENTATION_NAME}.hpp)
    configure_file(${INTERFACE_TEMPLATE} ${GENERATED_FILE} @ONLY)
    set(GENERATED_FILES ${GENERATED_FILES} ${GENERATED_FILE})

    # add to target
    target_sources(${target} PRIVATE ${GENERATED_FILES})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/generated)
    source_group("Generated" FILES ${GENERATED_FILES})
endfunction()