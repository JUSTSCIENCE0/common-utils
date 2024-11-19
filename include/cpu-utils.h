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

#ifndef INSTRUCTIONS_SETS
#error "Instruction sets are not defined for this platform"
#endif // !INSTRUCTIONS_SETS

// preprocessor magic starts here

// generate instructions family enum
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    enum E_INSTRUCTIONS_FAMILY {
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
            E_INFAM_ ##FAM_NAME,
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME)
#define END_INSTRUCTIONS_FAMILIES_LIST \
        E_INFAM_COUNT, \
        E_INFAM_END = E_INFAM_COUNT, \
        E_INFAM_BEGIN = 0 \
    };
    INSTRUCTIONS_SETS
    static_assert(E_INFAM_COUNT < sizeof(int) * 8, "Too many instructions families defined");

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// generate instructions family support flags
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    enum E_INSTRUCTIONS_FAMILY_SUPPORT_FLAG {
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
            E_INFAM_SUPPORT_FLAG_ ##FAM_NAME = 1 << E_INFAM_ ##FAM_NAME,
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME)
#define END_INSTRUCTIONS_FAMILIES_LIST \
    };
    INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

        // generate instructions family string names
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static const char* STR_INSTRUCTIONS_FAMILY[E_INFAM_COUNT] = {
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
            #FAM_NAME,
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME)
#define END_INSTRUCTIONS_FAMILIES_LIST \
    };
        INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

        // generate instructions sets enums
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST
#define BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
    enum E_INSTRUCTIONS_SET_ ##FAM_NAME { \
        E_INSET_ ##FAM_NAME ##_PREFIX = (E_INFAM_ ##FAM_NAME << 16) - 1,
#define     ADD_INSTRACTIONS_SET(SET_NAME, ...) \
        E_INSET_ ##SET_NAME,
#define END_INSTRUCTIONS_FAMILY(FAM_NAME) \
        E_INSET_ ##FAM_NAME ##_END, \
        E_INSET_ ##FAM_NAME ##_BEGIN = E_INSET_ ##FAM_NAME ##_PREFIX + 1, \
        E_INSET_ ##FAM_NAME ##_COUNT = E_INSET_ ##FAM_NAME ##_END - E_INSET_ ##FAM_NAME ##_BEGIN \
    }; \
    static_assert(E_INSET_ ##FAM_NAME ##_COUNT < sizeof(int) * 8, "Too many instructions sets defined for family " #FAM_NAME);
#define END_INSTRUCTIONS_FAMILIES_LIST
        INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// generate instructions sets string names
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static const char* STR_INSTRUCTIONS_SETS[][sizeof(int) * 8 - 1] = {
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
        {
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...) \
            #SET_NAME,
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME) \
        },
#define END_INSTRUCTIONS_FAMILIES_LIST \
    };
        INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST
}
