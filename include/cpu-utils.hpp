// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once
#include <assert.h>

namespace CU {
    // instructions family
    // instructions sets

#ifndef INSTRUCTIONS_SETS
#ifdef  CU_ARCH_X86_64
// see https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html

#define INSTRUCTIONS_SETS                               \
    BEGIN_INSTRUCTIONS_FAMILIES_LIST                    \
        BEGIN_INSTRUCTIONS_FAMILY(MMX)                  \
            ADD_INSTRACTIONS_SET(MMX)                   \
        END_INSTRUCTIONS_FAMILY(MMX)                    \
        BEGIN_INSTRUCTIONS_FAMILY(SSE)                  \
            ADD_INSTRACTIONS_SET(SSE)                   \
            ADD_INSTRACTIONS_SET(SSE2)                  \
            ADD_INSTRACTIONS_SET(SSE3)                  \
            ADD_INSTRACTIONS_SET(SSSE3)                 \
            ADD_INSTRACTIONS_SET(SSE4_1)                \
            ADD_INSTRACTIONS_SET(SSE4_2)                \
        END_INSTRUCTIONS_FAMILY(SSE)                    \
        BEGIN_INSTRUCTIONS_FAMILY(AVX)                  \
            ADD_INSTRACTIONS_SET(AVX)                   \
            ADD_INSTRACTIONS_SET(AVX2)                  \
        END_INSTRUCTIONS_FAMILY(AVX)                    \
        BEGIN_INSTRUCTIONS_FAMILY(AVX_512)              \
            ADD_INSTRACTIONS_SET(AVX_512F)              \
            ADD_INSTRACTIONS_SET(AVX_512PF)             \
            ADD_INSTRACTIONS_SET(AVX_512ER)             \
            ADD_INSTRACTIONS_SET(AVX_512CD)             \
            ADD_INSTRACTIONS_SET(AVX_512BW)             \
            ADD_INSTRACTIONS_SET(AVX_512DQ)             \
            ADD_INSTRACTIONS_SET(AVX_512VL)             \
            ADD_INSTRACTIONS_SET(AVX_512IFMA)           \
            ADD_INSTRACTIONS_SET(AVX_512VBMI)           \
            ADD_INSTRACTIONS_SET(AVX_512VBMI2)          \
            ADD_INSTRACTIONS_SET(AVX_512VNNI)           \
            ADD_INSTRACTIONS_SET(AVX_512BITALG)         \
            ADD_INSTRACTIONS_SET(AVX_512BF16)           \
            ADD_INSTRACTIONS_SET(AVX_512VPOPCNTDQ)      \
            ADD_INSTRACTIONS_SET(AVX_512VP2INTERSECT)   \
        END_INSTRUCTIONS_FAMILY(AVX_512)  \
    END_INSTRUCTIONS_FAMILIES_LIST

#endif //  CU_ARCH_X86_64
#endif // !INSTRUCTIONS_SETS

// preprocessor magic works here
#include "code-generators/instructions-sets.h"

// for INSTRUCTIONS_SETS the following objects are generated:
//  - single enum E_INSTRUCTIONS_FAMILY - list of all instructions families
//  - single enum E_INSTRUCTIONS_FAMILY_SUPPORT_FLAG - one-hot encoded list of all instructions families
//  - single static const char* STR_INSTRUCTIONS_FAMILY[] - array of instructions family names
//  - set of enum E_INSTRUCTIONS_SET_<INSTRUCTIONS_FAMILY_NAME> - list of instructions sets included in the family
//  - set of enum E_INSTRUCTIONS_SET_<INSTRUCTIONS_FAMILY_NAME>_SUPPORT_FLAG - one-hot encoded list of instructions sets included in the family
//  - single static const char* STR_INSTRUCTIONS_SETS[][] - array of instructions set names

}
