// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

// do not use separately from cpu-utils.hpp

#ifndef INSTRUCTIONS_SETS
#error "Instruction sets are not defined for this platform"
#endif // !INSTRUCTIONS_SETS

static constexpr size_t MAX_ELEMENTS_COUNT = 31;

// ---------------------------------------------------------------------------
// begin of auxiliary functions

// get_max_instructions_family_name_length
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static constexpr size_t get_max_instructions_family_name_length() { \
        size_t result = 0;
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
        if (result < std::char_traits<char>::length(#FAM_NAME)) result = std::char_traits<char>::length(#FAM_NAME);
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME) 
#define END_INSTRUCTIONS_FAMILIES_LIST \
        return result; \
    }
INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// get_max_instructions_set_name_length
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static constexpr size_t get_max_instructions_set_name_length() { \
        size_t result = 0;
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME)
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...) \
        if (result < std::char_traits<char>::length(#SET_NAME)) result = std::char_traits<char>::length(#SET_NAME);
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME) 
#define END_INSTRUCTIONS_FAMILIES_LIST \
        return result; \
    }
    INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// end of auxiliary functions
// ---------------------------------------------------------------------------

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
static_assert(E_INFAM_COUNT <= MAX_ELEMENTS_COUNT, "Too many instructions families defined");

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
    static constexpr const char* STR_INSTRUCTIONS_FAMILY[E_INFAM_COUNT] = {
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
    static_assert(E_INSET_ ##FAM_NAME ##_COUNT <= MAX_ELEMENTS_COUNT, "Too many instructions sets defined for family " #FAM_NAME);
#define END_INSTRUCTIONS_FAMILIES_LIST
INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// generate instructions sets support flags
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST
#define BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
    enum E_INSTRUCTIONS_SET_ ##FAM_NAME ##_SUPPORT_FLAG {
#define     ADD_INSTRACTIONS_SET(SET_NAME, ...) \
        E_INSET_SUPPORT_FLAG_ ##SET_NAME = 1 << (E_INSET_ ##SET_NAME & 0xFFFF),
#define END_INSTRUCTIONS_FAMILY(FAM_NAME) \
    };
#define END_INSTRUCTIONS_FAMILIES_LIST
INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// generate instructions sets string names
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static constexpr const char* STR_INSTRUCTIONS_SETS[][MAX_ELEMENTS_COUNT] = {
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

// generate current configuration description
#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static std::string get_current_configuration_description() { \
        std::stringstream res; \
        constexpr size_t fam_width = get_max_instructions_family_name_length(); \
        constexpr size_t set_width = get_max_instructions_set_name_length(); \
        constexpr size_t width = std::max(fam_width, set_width); \
        E_INSTRUCTIONS_FAMILY current_family; \
        res << std::hex; \
        res << std::setfill('-') << std::setw(width + MAX_ELEMENTS_COUNT + 60) << "" << std::setfill(' ') << "\n";
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME) \
        current_family = E_INFAM_ ##FAM_NAME; \
        res << "Instructions family ID: 0x" << \
            std::right << std::setfill('0') << std::setw(8) << current_family << std::setfill(' ') << \
            " NAME: " << std::left << std::setw(width + 1) << STR_INSTRUCTIONS_FAMILY[current_family] << \
            " SUPPORT_FLAG: " << std::bitset<MAX_ELEMENTS_COUNT>(E_INFAM_SUPPORT_FLAG_ ##FAM_NAME) << "\n";
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...) \
        res << "Instructions set    ID: 0x" << \
            std::right << std::setfill('0') << std::setw(8) << E_INSET_ ##SET_NAME << std::setfill(' ') << \
            " NAME: " << std::left << std::setw(width + 1) << STR_INSTRUCTIONS_SETS[current_family][E_INSET_ ##SET_NAME & 0xFFFF] << \
            " SUPPORT_FLAG: " << std::bitset<MAX_ELEMENTS_COUNT>(E_INSET_SUPPORT_FLAG_ ##SET_NAME) << "\n";
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME) \
        res << std::setfill('-') << std::setw(width + MAX_ELEMENTS_COUNT + 60) << "" << std::setfill(' ') << "\n";
#define END_INSTRUCTIONS_FAMILIES_LIST \
        return res.str(); \
    }
INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

struct CPUConfiguration {
    std::string m_vendor = "";
    std::string m_model  = "";

    SupportInsFams m_supported_families = 0;
    SupportInsSets m_supported_sets[E_INFAM_COUNT] = {};
};

#ifndef CUSTOM_CPU_CONFIGURATION_READER

#ifdef  CU_ARCH_X86_64

namespace X86_64_IMPL {
    static std::bitset<32> ECX1 = 0;
    static std::bitset<32> EDX1 = 0;
    static std::bitset<32> EBX7 = 0;
    static std::bitset<32> ECX7 = 0;
    static std::bitset<32> ECX81 = 0;
    static std::bitset<32> EDX81 = 0;
}

#ifdef   _MSC_VER
// see https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex

#define READ_REGISTERS \
    std::array<int, 4> cpu_descr;  \
    __cpuid(cpu_descr.data(), 0);  \
    int fID_number = cpu_descr[0]; \
    
#endif // _MSC_VER
#endif // CU_ARCH_X86_64

#define BEGIN_INSTRUCTIONS_FAMILIES_LIST \
    static CPUConfiguration read_cpu_configuration() { \
        CPUConfiguration result = { \
            .m_vendor = get_cpu_vendor(),\
            .m_model = get_cpu_model() \
        };
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME)
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME)
#define END_INSTRUCTIONS_FAMILIES_LIST \
        return result; \
    }
INSTRUCTIONS_SETS

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

#else
CUSTOM_CPU_CONFIGURATION_READER
#endif // !CUSTOM_CPU_CONFIGURATION_READER

