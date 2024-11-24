// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once
#include <assert.h>
#include <stdlib.h>

#include <array>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>

#ifdef CU_ARCH_X86_64
#ifdef  _MSC_VER
#include <intrin.h>
#endif // _MSC_VER
#endif // CU_ARCH_X86_64

namespace CU {
    typedef int InstructionsFamily;
    typedef int InstructionsSet;
    typedef int SupportInsFams;
    typedef int SupportInsSets;

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

#define INSTRUCTIONS_SETS                                                 \
    BEGIN_INSTRUCTIONS_FAMILIES_LIST                                      \
        BEGIN_INSTRUCTIONS_FAMILY(MMX)                                    \
            ADD_INSTRACTIONS_SET( MMX,                 REG_1_0, EDX, 23 ) \
            ADD_INSTRACTIONS_SET( MMX_EXT_AMD,         EXT_1_0, EDX, 22 ) \
        END_INSTRUCTIONS_FAMILY(MMX)                                      \
        BEGIN_INSTRUCTIONS_FAMILY(SSE)                                    \
            ADD_INSTRACTIONS_SET( SSE,                 REG_1_0, EDX, 25 ) \
            ADD_INSTRACTIONS_SET( SSE2,                REG_1_0, EDX, 26 ) \
            ADD_INSTRACTIONS_SET( SSE3,                REG_1_0, ECX, 0  ) \
            ADD_INSTRACTIONS_SET( SSSE3,               REG_1_0, ECX, 9  ) \
            ADD_INSTRACTIONS_SET( SSE4_1,              REG_1_0, ECX, 19 ) \
            ADD_INSTRACTIONS_SET( SSE4_2,              REG_1_0, ECX, 20 ) \
        END_INSTRUCTIONS_FAMILY(SSE)                                      \
        BEGIN_INSTRUCTIONS_FAMILY(AVX)                                    \
            ADD_INSTRACTIONS_SET( AVX,                 REG_1_0, ECX, 28 ) \
            ADD_INSTRACTIONS_SET( AVX2,                REG_7_0, EBX, 5  ) \
        END_INSTRUCTIONS_FAMILY(AVX)                                      \
        BEGIN_INSTRUCTIONS_FAMILY(AVX_512)                                \
            ADD_INSTRACTIONS_SET( AVX_512F,            REG_7_0, EBX, 16 ) \
            ADD_INSTRACTIONS_SET( AVX_512PF,           REG_7_0, EBX, 26 ) \
            ADD_INSTRACTIONS_SET( AVX_512ER,           REG_7_0, EBX, 27 ) \
            ADD_INSTRACTIONS_SET( AVX_512CD,           REG_7_0, EBX, 28 ) \
            ADD_INSTRACTIONS_SET( AVX_512BW,           REG_7_0, EBX, 30 ) \
            ADD_INSTRACTIONS_SET( AVX_512DQ,           REG_7_0, EBX, 17 ) \
            ADD_INSTRACTIONS_SET( AVX_512VL,           REG_7_0, EBX, 31 ) \
            ADD_INSTRACTIONS_SET( AVX_512IFMA,         REG_7_0, EBX, 21 ) \
            ADD_INSTRACTIONS_SET( AVX_512VBMI,         REG_7_0, ECX, 1  ) \
            ADD_INSTRACTIONS_SET( AVX_512VBMI2,        REG_7_0, ECX, 6  ) \
            ADD_INSTRACTIONS_SET( AVX_512VNNI,         REG_7_0, ECX, 11 ) \
            ADD_INSTRACTIONS_SET( AVX_512BITALG,       REG_7_0, ECX, 12 ) \
            ADD_INSTRACTIONS_SET( AVX_512BF16,         REG_7_1, EAX, 5  ) \
            ADD_INSTRACTIONS_SET( AVX_512VPOPCNTDQ,    REG_7_0, ECX, 14 ) \
            ADD_INSTRACTIONS_SET( AVX_512VP2INTERSECT, REG_7_0, EDX, 8  ) \
            ADD_INSTRACTIONS_SET( AVX_5124FMAPS,       REG_7_0, EDX, 3  ) \
            ADD_INSTRACTIONS_SET( AVX_5124VNNIW,       REG_7_0, EDX, 2  ) \
            ADD_INSTRACTIONS_SET( AVX_512FP16,         REG_7_0, EDX, 23 ) \
        END_INSTRUCTIONS_FAMILY(AVX_512)                                  \
    END_INSTRUCTIONS_FAMILIES_LIST

#endif //  CU_ARCH_X86_64
#endif // !INSTRUCTIONS_SETS

#ifndef CUSTOM_CPU_CONFIGURATION_READER

#ifdef  CU_ARCH_X86_64
#ifdef   _MSC_VER
// see https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex

    static inline std::string get_cpu_vendor() {
        std::array<int, 4> cpu_descr;
        __cpuid(cpu_descr.data(), 0);

        char vendor[13] = "";

        std::memcpy(vendor,                   &cpu_descr[1], sizeof(int));
        std::memcpy(vendor + sizeof(int),     &cpu_descr[3], sizeof(int));
        std::memcpy(vendor + 2 * sizeof(int), &cpu_descr[2], sizeof(int));

        // short name of popular brands
        if (!strcmp(vendor, "GenuineIntel"))
            return "Intel";
        if (!strcmp(vendor, "AuthenticAMD"))
            return "AMD";

        return vendor;
    }

    static inline std::string get_cpu_model() {
        std::array<int, 4> cpu_descr;
        __cpuid(cpu_descr.data(), 0x80000000);
        if (cpu_descr[0] < 0x80000004)
            return "Not defined";

        char brand[49] = "";
        __cpuidex(reinterpret_cast<int*>(brand),                   0x80000002, 0);
        __cpuidex(reinterpret_cast<int*>(brand + sizeof(int) * 4), 0x80000003, 0);
        __cpuidex(reinterpret_cast<int*>(brand + sizeof(int) * 8), 0x80000004, 0);

        return brand;
    }
#endif // _MSC_VER
#endif // CU_ARCH_X86_64

#endif // !CUSTOM_CPU_CONFIGURATION_READER

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
//  8) struct CPUConfiguration {
//          std::string m_vendor = "";
//          std::string m_model = "";
//          SupportInsFams m_supported_families = 0;
//          SupportInsSets m_supported_sets[E_INFAM_COUNT] = {};
//     };
//     Description: TODO
// 


std::ostream& operator<<(std::ostream& os, const CPUConfiguration& cpu_conf) {
    return os << "CPU Vendor: " << cpu_conf.m_vendor << std::endl << \
        "CPU Model: " << cpu_conf.m_model << std::endl;

    // TODO: supported features
}
}
