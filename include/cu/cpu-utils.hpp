// Copyright (c) 2024-2025, Yakov Usoltsev
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
#include <cstring>

#ifdef CU_ARCH_X86_64

#if  defined(_MSC_VER)
#include <intrin.h>
#endif // MSVC

#if  defined(__GNUC__) || \
     defined(__GNUG__) || \
     defined(__clang__)
#include <cpuid.h>

#undef __cpuid
static inline void __cpuid(int cpuInfo[4], int function_id) {
    uint* _cpu_info = reinterpret_cast<uint*>(cpuInfo);
    __get_cpuid(function_id, &_cpu_info[0], &_cpu_info[1], &_cpu_info[2], &_cpu_info[3]);
}

#if   defined(__clang__)
static inline void __cpuidex(   int cpuInfo[4], int function_id, int subfunction_id) {
    __cpuid_count(function_id, subfunction_id, 
                  reinterpret_cast<unsigned int&>(cpuInfo[0]),
                  reinterpret_cast<unsigned int&>(cpuInfo[1]),
                  reinterpret_cast<unsigned int&>(cpuInfo[2]),
                  reinterpret_cast<unsigned int&>(cpuInfo[3]));
}
#endif // Clang
#endif // GCC & Clang

#endif // CU_ARCH_X86_64

namespace CU {
    typedef int InstructionsFamily;
    typedef int InstructionsSet;
    typedef int SupportInfam;
    typedef int SupportInset;

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
// The user can define a custom function to read the current CPU configuration
// in the define CUSTOM_CPU_CONFIGURATION_READER. A function with the following
// signature is expected: CPUConfiguration read_cpu_configuration().
// In this function, the user can utilize PLATFORM_SPECIFIC_HANDLERS specific
// to the defined INSTRUCTION_SETS.
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
#if defined(_MSC_VER) || \
    defined(__GNUC__) || \
    defined(__GNUG__) || \
    defined(__clang__)

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
        if (cpu_descr[0] < int(0x80000004))
            return "Not defined";

        char brand[49] = "";
        __cpuidex(reinterpret_cast<int*>(brand),                   0x80000002, 0);
        __cpuidex(reinterpret_cast<int*>(brand + sizeof(int) * 4), 0x80000003, 0);
        __cpuidex(reinterpret_cast<int*>(brand + sizeof(int) * 8), 0x80000004, 0);

        return brand;
    }
#endif // MSVC or GCC

#endif // CU_ARCH_X86_64

#endif // !CUSTOM_CPU_CONFIGURATION_READER

// preprocessor magic works here
#include <cu/code-generators/instructions-sets.h>

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
//  8) static inline int get_insets_count_for_infam(InstructionsFamily infam)
//     Description: Function that returns the number of sets in the family
// 
//  9) struct CPUConfiguration {
//          std::string m_vendor = "";
//          std::string m_model = "";
//          SupportInfam m_supported_families = 0;
//          SupportInset m_supported_sets[E_INFAM_COUNT] = {};
//     };
//     Description: Structure describing the configuration of the processor.
//     Fields m_supported_families and m_supported_sets represent the disjunction
//     of support flags for all supported families and sets respectively.
// 
// If CUSTOM_CPU_CONFIGURATION_READER is not defined, also read_cpu_configuration() will be generated:
// 10) static CPUConfiguration read_cpu_configuration()
//     Description: Function that returns configuration of the processor
//     on which the program is currently running.
// 

    static const CPUConfiguration CURRENT_CPU_CONFIGURATION = read_cpu_configuration();

    static inline InstructionsFamily get_inset_family(InstructionsSet inset) {
        return inset >> 16;
    }

    static inline int get_inset_index(InstructionsSet inset) {
        return inset & 0xFFFF;
    }

    static inline SupportInfam get_infam_support_flag(InstructionsFamily infam) {
        return 1 << infam;
    }

    static inline SupportInfam get_inset_support_flag(InstructionsSet inset) {
        return 1 << get_inset_index(inset);
    }

    static inline bool is_infam_supported(const CPUConfiguration& conf, InstructionsFamily infam) {
        return conf.m_supported_families & get_infam_support_flag(infam);
    }

    static inline bool is_inset_supported(const CPUConfiguration& conf, InstructionsSet inset) {
        InstructionsFamily infam = get_inset_family(inset);
        if (!is_infam_supported(conf, infam))
            return false;

        return conf.m_supported_sets[infam] & get_inset_support_flag(inset);
    }

    static inline const char* get_infam_name(InstructionsFamily infam) {
        return STR_INSTRUCTIONS_FAMILY[infam];
    }

    static inline const char* get_inset_name(InstructionsSet inset) {
        return STR_INSTRUCTIONS_SETS[get_inset_family(inset)][get_inset_index(inset)];
    }

    static std::ostream& operator<<(std::ostream& os, const CPUConfiguration& conf) {
        std::string features_list = "";
        for (InstructionsFamily infam = E_INFAM_BEGIN; infam < E_INFAM_END; infam++) {
            if (!is_infam_supported(conf, infam))
                continue;
            for (int inset_index = 0; inset_index < get_insets_count_for_infam(infam); inset_index++) {
                if (conf.m_supported_sets[infam] & (1 << inset_index)) {
                    features_list += STR_INSTRUCTIONS_SETS[infam][inset_index];
                    features_list += " ";
                }
            }
        }

        return os << "CPU Vendor: " << conf.m_vendor << std::endl << \
            "CPU Model: " << conf.m_model << std::endl << \
            "Features: " << features_list << std::endl;
    }

    static inline bool is_inset_supported(InstructionsSet inset) {
        return is_inset_supported(CURRENT_CPU_CONFIGURATION, inset);
    }
}
