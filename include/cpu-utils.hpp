// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once
#include <assert.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>

namespace CU {
// The supported instruction sets are defined using the INSTRUCTIONS_SETS macro.
// Custom INSTRUCTIONS_SETS can be used. Due to the limited number of bits in one-hot encoding,
// the supported instruction sets are divided into families. It is necessary to specify a 
// list of instruction families and a set of instructions in each family.
// 
// Expected structure of the INSTRUCTIONS_SETS macro:
//  BEGIN_INSTRUCTIONS_FAMILIES_LIST
//      BEGIN_INSTRUCTIONS_FAMILY(FAMILY_1)
//          ADD_INSTRACTIONS_SET(SET_1_1, PLATFORM_SPECIFIC_HANDLERS)
//          ADD_INSTRACTIONS_SET(SET_1_2, PLATFORM_SPECIFIC_HANDLERS)
//          ...
//          ADD_INSTRACTIONS_SET(SET_1_A, PLATFORM_SPECIFIC_HANDLERS)
//      END_INSTRUCTIONS_FAMILY(FAMILY_1)
//      ...
//      BEGIN_INSTRUCTIONS_FAMILY(FAMILY_N)
//          ADD_INSTRACTIONS_SET(SET_N_1, PLATFORM_SPECIFIC_HANDLERS)
//          ADD_INSTRACTIONS_SET(SET_N_2, PLATFORM_SPECIFIC_HANDLERS)
//          ...
//          ADD_INSTRACTIONS_SET(SET_N_C, PLATFORM_SPECIFIC_HANDLERS)
//      END_INSTRUCTIONS_FAMILY(FAMILY_1)
//  END_INSTRUCTIONS_FAMILIES_LIST
// 
// TODO: PLATFORM_SPECIFIC_HANDLERS
//

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

// for the given INSTRUCTIONS_SETS the following objects are generated:
//  1) enum E_INSTRUCTIONS_FAMILY {
//          E_INFAM_BEGIN,
//          E_INFAM_<FAMILY_1> = E_INFAM_BEGIN,
//          E_INFAM_<FAMILY_2>,
//          ...
//          E_INFAM_<FAMILY_N>,
//          E_INFAM_COUNT,
//          E_INFAM_END = E_INFAM_COUNT
//     };
//     Description: List of all instructions families. This object is generated once.
// 
//  2) enum E_INSTRUCTIONS_FAMILY_SUPPORT_FLAG {
//          E_INFAM_SUPPORT_FLAG_<FAMILY_1> = 1 << E_INFAM_<FAMILY_1>,
//          E_INFAM_SUPPORT_FLAG_<FAMILY_2> = 1 << E_INFAM_<FAMILY_2>,
//          ...
//          E_INFAM_SUPPORT_FLAG_<FAMILY_2> = 1 << E_INFAM_<FAMILY_N>,
//     };
//     Description: One-hot encoded list of all instructions families. This object is generated once.
// 
//  3) static const char* STR_INSTRUCTIONS_FAMILY[] = {
//          "FAMILY_1", "FAMILY_2", ... "FAMILY_N",
//     };
//     Description: Array of instructions family names. This object is generated once.
// 
//  4) enum E_INSTRUCTIONS_SET_<FAMILY_K> {
//          E_INSET_<FAMILY_K>_BEGIN = E_INFAM_<FAMILY_K> << 16,
//          E_INSET_<SET_K_1> = E_INSET_<FAMILY_K>_BEGIN,
//          E_INSET_<SET_K_2>,
//          ...
//          E_INSET_<SET_K_M>,
//          E_INSET_<FAMILY_K>_END,
//          E_INSET_<FAMILY_K>_COUNT = E_INSET_<FAMILY_K>_END - E_INSET_<FAMILY_K>_BEGIN
//     };
//     Description: List of instructions sets included in the family.
//                  A similar object is generated for each instructions family.
// 
//  5) enum E_INSTRUCTIONS_SET_<FAMILY_K>_SUPPORT_FLAG {
//          E_INSET_SUPPORT_FLAG_<SET_K_1> = 1 << (E_INSET_<SET_K_1> & 0xFFFF),
//          E_INSET_SUPPORT_FLAG_<SET_K_2> = 1 << (E_INSET_<SET_K_2> & 0xFFFF),
//          ...
//          E_INSET_SUPPORT_FLAG_<SET_K_M> = 1 << (E_INSET_<SET_K_M> & 0xFFFF),
//     };
//     Description: One-hot encoded list of instructions sets included in the family.
//                  A similar object is generated for each instructions family.
// 
//  6) static const char* STR_INSTRUCTIONS_SETS[][] = {
//          { "SET_1_1", "SET_1_2", ... "SET_1_A" },
//          { "SET_2_1", "SET_2_2", ... "SET_2_B" },
//          ...
//          { "SET_N_1", "SET_N_2", ... "SET_N_C" },
//     };
//     Description: Array of instructions set names.
//                  A similar object is generated for each instructions family.
// 
//  7) static std::string get_current_configuration_description()
//     Description: Function that returns a string description of 
//     the current configuration generated according to INSTRUCTIONS_SETS
//
}
