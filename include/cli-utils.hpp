// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <getopt.h>

#include <string>
#include <cassert>
#include <sstream>
#include <iostream>
#include <concepts>
#include <list>
#include <initializer_list>
#include <optional>

namespace CU {
// TODO: doc - CLI_CONFIGURATION interface
//
//  CLI_FLAG( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION )
//  CLI_VALUABLE_FLAG( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR )
//  CLI_OPTIONAL_PROPERTY( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR )
//  CLI_REQUIRED_PROPERTY( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, VALIDATOR )
// 
//  SYMBOL(s)
//  WO_SYMBOL
//
#ifndef CLI_CONFIGURATION
// TODO: default CLI_CONFIGURATION
#define CLI_CONFIGURATION
#endif // !CLI_CONFIGURATION

    // parsing
    template <typename OptionType>
    bool parse_option(const char* in_option, OptionType* out_value) {
        std::stringstream parser(in_option);
        parser >> *out_value;

        if (!parser.eof() || parser.fail())
            return false;
        else
            return true;
    }

    // validation

    // validator without any limitations
    template <typename OptionType>
    concept StreamInsertable = requires(std::ostream& os, OptionType value) {
        { os << value } -> std::same_as<std::ostream&>;
    };
    template <StreamInsertable OptionType>
    class BaseValidator {
    public:
        virtual bool CheckValue(const OptionType& value) const { return true; }
        // TODO: add validator description
    };

    // validator that checks if a value is within the range [min, max]
    template <std::three_way_comparable OptionType>
    class RangeValidator final :
        public BaseValidator<OptionType> {
    public:
        RangeValidator(
            const OptionType& min,
            const OptionType& max) :
            m_min(min),
            m_max(max) {
            assert(min < max);
        }

        bool CheckValue(const OptionType& value) const override {
            return (m_min <= value) && (value <= m_max);
        }
        // TODO: add validator description
    private:
        const OptionType m_min;
        const OptionType m_max;
    };

    // validator that checks if a value is present in a predefined list
    template <std::equality_comparable OptionType>
    class ListValidator final :
        public BaseValidator<OptionType> {
    public:
        ListValidator(std::initializer_list<OptionType> valid_options) :
            m_valid_options(valid_options) {}

        bool CheckValue(const OptionType& value) const override {
            return m_valid_options.end() !=
                std::find(m_valid_options.begin(), m_valid_options.end(), value);
        }
        // TODO: add validator description
    private:
        const std::list<OptionType> m_valid_options;
    };

#ifdef CUSTOM_VALIDATORS
    // TODO: doc - CUSTOM_VALIDATORS
    CUSTOM_VALIDATORS
#endif CUSTOM_VALIDATORS

    template <typename OptionType>
    bool validate_option(const OptionType& value, const BaseValidator<OptionType>& validator) {
        return validator.CheckValue(value);
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

// preprocessor magic works here
#include "code-generators/cli-parsers.h"

// TODO: doc - generation
}
