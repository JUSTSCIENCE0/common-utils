# Copyright (c) 2025, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

# For the given implementation of a function or class,
# generates an interface header file (.hpp) and a compilation unit file (.cpp)
# for each supported instruction set.
#
# Arguments:
#  target — the project to which the generated source files will be added
#  implementation — the name of the implementation for which the files are generated
#  supported_sets — the list of supported instruction sets
#
# Preconditions:
# Files <implementation>_iface.hpp and <implementation>_impl.hpp
# must exist, defining the interface and implementation of the generated
# executable files.
function(generate_simd_compile_units
    target
    implementation
    #LIST supported_sets 
)
    set(IMPLEMENTATION_NAME   ${implementation})
    set(COMPILE_UNIT_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-compile-unit.template)
    set(INTERFACE_TEMPLATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/simd-interface.template)
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

    foreach(supported_set IN LISTS ARGN)
        set(SUPPORT_FLAG_NAME "CU_SIMD_SUPPORT_${supported_set}")
        set(${SUPPORT_FLAG_NAME} 1)
        set(CURRENT_SUPPORTED_SET ${supported_set})

        # generate compile unit
        string(TOLOWER ${supported_set} supported_set_lower)
        set(GENERATED_FILE ${CMAKE_CURRENT_LIST_DIR}/generated/${IMPLEMENTATION_NAME}_${supported_set_lower}.cpp)
        configure_file(${COMPILE_UNIT_TEMPLATE} ${GENERATED_FILE} @ONLY)
        set(GENERATED_FILES ${GENERATED_FILES} ${GENERATED_FILE})
    endforeach()

    # generate interface header
    set(GENERATED_FILE ${CMAKE_CURRENT_LIST_DIR}/generated/${IMPLEMENTATION_NAME}.hpp)
    configure_file(${INTERFACE_TEMPLATE} ${GENERATED_FILE} @ONLY)
    set(GENERATED_FILES ${GENERATED_FILES} ${GENERATED_FILE})
    message("GENERATED_FILES = ${GENERATED_FILES}")

    # TODO:
    # source_group
    # add to target
endfunction()