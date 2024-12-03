// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <getopt.h>

#include <cassert>
#include <sstream>
#include <iostream>
#include <concepts>

namespace CU {
// TODO: doc - CLI_CONFIGURATION interface
#ifndef CLI_CONFIGURATION
// TODO: default CLI_CONFIGURATION
#define CLI_CONFIGURATION
#endif // !CLI_CONFIGURATION

    // parsing
    template <typename OptionType>
    bool parse_option(const char* in_option, OptionType& out_value) {
        std::stringstream parser(in_option);
        parser >> out_value;

        if (!parser.eof() || parser.fail())
            return false;
        else
            return true;
    }

    // validation
    template <typename OptionType>
    class BaseValidator {
    public:
        virtual bool CheckValue(const OptionType& value) const { return true; }
    };

    // validator that checks if a value is within the range [min, max]
    template <std::three_way_comparable OptionType>
    class RangeValidator final :
        public BaseValidator<OptionType> {
    public:
        RangeValidator(
            OptionType min,
            OptionType max) :
            m_min(min),
            m_max(max) {
            assert(min < max);
        }

        bool CheckValue(const OptionType& value) const override {
            return (m_min <= value) && (value <= m_max);
        }

    private:
        const OptionType m_min;
        const OptionType m_max;
    };

#ifdef CUSTOM_VALIDATORS
    // TODO: doc - CUSTOM_VALIDATORS
    CUSTOM_VALIDATORS
#endif CUSTOM_VALIDATORS

    template <typename OptionType>
    bool validate_option(const OptionType& value, const BaseValidator<OptionType>& validator) {
        return validator.CheckValue(value);
    }

// TODO: template specializations for additional functional

// preprocessor magic works here
#include "code-generators/cli-parsers.h"

// TODO: doc - generation
}
