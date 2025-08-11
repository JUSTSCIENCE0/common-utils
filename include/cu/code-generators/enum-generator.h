// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

// do not use separately from enum-utils.hpp

#ifndef CU_ENUMS_DESCRIPTION
#error "Enums description is not defined"
#endif // !INSTRUCTIONS_SETS

// enums definition
#define CU_BEGIN_ENUM(NAME) enum NAME { \
    E_ ## NAME ## _UNKNOWN = -1,
#define CU_ENUM_UNIT(NAME) NAME,
#define CU_VALUED_ENUM_UNIT(NAME, VALUE) NAME = VALUE,
#define CU_ENUM_ANCILLARY_UNITS(PREFIX) \
    PREFIX ## _COUNT, \
    PREFIX ## _END = PREFIX ## _COUNT, \
    PREFIX ## _BEGIN = 0,
#define CU_END_ENUM(NAME) };

CU_ENUMS_DESCRIPTION

#undef CU_BEGIN_ENUM
#undef CU_ENUM_UNIT
#undef CU_VALUED_ENUM_UNIT
#undef CU_ENUM_ANCILLARY_UNITS
#undef CU_END_ENUM

// enum to string
#define CU_BEGIN_ENUM(NAME) \
    static inline const char* to_string(NAME value) { \
        switch (value) {
#define CU_ENUM_UNIT(NAME) \
        case NAME : \
            return #NAME ;
#define CU_VALUED_ENUM_UNIT(NAME, VALUE) \
        case NAME : \
            return #NAME ;
#define CU_ENUM_ANCILLARY_UNITS(PREFIX)
#define CU_END_ENUM(NAME) \
        default: \
            return nullptr; \
        } \
    }

CU_ENUMS_DESCRIPTION

#undef CU_BEGIN_ENUM
#undef CU_ENUM_UNIT
#undef CU_VALUED_ENUM_UNIT
#undef CU_ENUM_ANCILLARY_UNITS
#undef CU_END_ENUM

// enum from string
#define CU_BEGIN_ENUM(NAME) \
    static inline NAME NAME ## _from_string(const char* str) {
#define CU_ENUM_UNIT(NAME) \
        if (!std::strcmp(str, #NAME)) return NAME;
#define CU_VALUED_ENUM_UNIT(NAME, VALUE) \
        if (!std::strcmp(str, #NAME)) return NAME;
#define CU_ENUM_ANCILLARY_UNITS(PREFIX)
#define CU_END_ENUM(NAME) \
        return E_ ## NAME ## _UNKNOWN; \
    }

CU_ENUMS_DESCRIPTION

#undef CU_BEGIN_ENUM
#undef CU_ENUM_UNIT
#undef CU_VALUED_ENUM_UNIT
#undef CU_ENUM_ANCILLARY_UNITS
#undef CU_END_ENUM
