// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <cu/macro-utils.hpp>

#if defined(CU_COMPILE_UNIT_SSE)
#  define FUNC_POSTFIX sse
#elif defined(CU_COMPILE_UNIT_SSE2)
#  define FUNC_POSTFIX sse2
#elif defined(CU_COMPILE_UNIT_SSE3)
#  define FUNC_POSTFIX sse3
#elif defined(CU_COMPILE_UNIT_SSSE3)
#  define FUNC_POSTFIX ssse3
#elif defined(CU_COMPILE_UNIT_SSE4_1)
#  define FUNC_POSTFIX sse4_1
#elif defined(CU_COMPILE_UNIT_SSE4_2)
#  define FUNC_POSTFIX sse4_2
#elif defined(CU_COMPILE_UNIT_AVX)
#  define FUNC_POSTFIX avx
#elif defined(CU_COMPILE_UNIT_AVX2)
#  define FUNC_POSTFIX avx2
#elif defined(CU_COMPILE_UNIT_AVX512)
#  define FUNC_POSTFIX avx512
//TODO: we need specific compile_unit for some avx512 instructions set
//TODO: ARM
#else // CU_COMPILE_UNIT_DEF or just default implementation
#  define FUNC_POSTFIX def
#endif

// macro CU_SIMD(name)
// Description: Generates an identifier with a postfix corresponding to the current compilation unit.
//              The format of the compilation unit is defined by the macro CU_COMPILE_UNIT_<current instruction set>.
#ifndef CU_SIMD
#  define CU_SIMD(name) CU_EXPAND_CONCAT(name, FUNC_POSTFIX)
#endif

#ifdef CU_IMPLEMENTATION_INCLUDE
#  ifndef CU_IMPLEMENTATION_INCLUDE_EXT
#   define CU_IMPLEMENTATION_INCLUDE_EXT .h
#  endif

#define CU_INCLUDE_FILE \
    CU_EXPAND_STR( \
        CU_EXPAND_CONCAT(CU_IMPLEMENTATION_INCLUDE, FUNC_POSTFIX).CU_IMPLEMENTATION_INCLUDE_EXT)
#include CU_INCLUDE_FILE
#endif // CU_IMPLEMENTATION_INCLUDE

