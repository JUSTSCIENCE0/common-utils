// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <concepts>
#include <iostream>

namespace CU {
    template <typename OptionType>
    concept Parsable = requires(std::istream & is, OptionType value) {
        { is >> value } -> std::same_as<std::istream&>;
    };
    template <Parsable OptionType>
    bool parse_option(const char* in_option, OptionType* out_value) {
        std::stringstream parser(in_option);
        parser >> *out_value;

        if (!parser.eof() || parser.fail())
            return false;
        else
            return true;
    }

    // HEX parsing
    struct HEX {
        uint64_t value;
        HEX(uint64_t val = 0) : value(val) {}
        operator uint64_t() const { return value; }
    };
    std::istream& operator>>(std::istream& is, HEX& hex) {
        is >> std::hex >> hex.value;
        return is;
    }
}