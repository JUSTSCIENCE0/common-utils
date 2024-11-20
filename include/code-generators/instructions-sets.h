// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#ifndef INSTRUCTIONS_SETS
#error "Instruction sets are not defined for this platform"
#endif // !INSTRUCTIONS_SETS

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
    static_assert(E_INSET_ ##FAM_NAME ##_COUNT < sizeof(int) * 8, "Too many instructions sets defined for family " #FAM_NAME);
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
    static constexpr const char* STR_INSTRUCTIONS_SETS[][sizeof(int) * 8 - 1] = {
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
    static constexpr size_t get_max_instructions_family_name_length(); \
    static constexpr size_t get_max_instructions_set_name_length(); \
    static constexpr std::string get_current_configuration_description() {
#define     BEGIN_INSTRUCTIONS_FAMILY(FAM_NAME)
#define         ADD_INSTRACTIONS_SET(SET_NAME, ...)
// TODO
#define     END_INSTRUCTIONS_FAMILY(FAM_NAME) 
#define END_INSTRUCTIONS_FAMILIES_LIST \
    }

#undef BEGIN_INSTRUCTIONS_FAMILIES_LIST
#undef   BEGIN_INSTRUCTIONS_FAMILY
#undef     ADD_INSTRACTIONS_SET
#undef   END_INSTRUCTIONS_FAMILY
#undef END_INSTRUCTIONS_FAMILIES_LIST

// ---------------------------------------------------------------------------
// auxiliary functions

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
