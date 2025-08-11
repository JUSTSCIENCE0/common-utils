// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#define CU_ENUMS_DESCRIPTION \
    CU_BEGIN_ENUM(SimpleEnum) \
        CU_ENUM_UNIT(E_SIMPLE_OPTION1) \
        CU_ENUM_UNIT(E_SIMPLE_OPTION2) \
        CU_ENUM_UNIT(E_SIMPLE_OPTION3) \
        CU_ENUM_UNIT(E_SIMPLE_OPTION4) \
        CU_ENUM_ANCILLARY_UNITS(E_SIMPLE) \
    CU_END_ENUM(SimpleEnum) \
    CU_BEGIN_ENUM(ValuedEnum) \
        CU_VALUED_ENUM_UNIT(E_VAL_OPTION1, 0xE3) \
        CU_VALUED_ENUM_UNIT(E_VAL_OPTION2, 0xFF) \
        CU_VALUED_ENUM_UNIT(E_VAL_OPTION3, 0x12) \
    CU_END_ENUM(ValuedEnum)

#include <cu/enum-utils.hpp>

#include <iostream>

int main() {
    std::cout << "SimpleEnum:" << std::endl;
    for (int e = SimpleEnum::E_SIMPLE_BEGIN; e < SimpleEnum::E_SIMPLE_END; e++) {
        std::cout << "\t" << to_string(SimpleEnum(e)) << " = " << e << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ValuedEnum:" << std::endl;
    std::cout << "\t" << to_string(E_VAL_OPTION1) << " = " << E_VAL_OPTION1 << std::endl;
    std::cout << "\t" << to_string(E_VAL_OPTION2) << " = " << E_VAL_OPTION2 << std::endl;
    std::cout << "\t" << to_string(E_VAL_OPTION3) << " = " << E_VAL_OPTION3 << std::endl;
    std::cout << std::endl;

    std::string unit_name;
    std::cout << "enter SimpleEnum unit name, to get value:" << std::endl;
    std::cin >> unit_name;
    std::cout << "result: " << SimpleEnum_from_string(unit_name.c_str()) << std::endl;
    std::cout << std::endl;

    std::cout << "enter ValuedEnum unit name, to get value:" << std::endl;
    std::cin >> unit_name;
    std::cout << "result: " << ValuedEnum_from_string(unit_name.c_str()) << std::endl;

    return 0;
}