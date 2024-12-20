// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(_MSC_VER)
#include "external/getopt.h"
#else
#include <getopt.h>
#endif // MSVC

#include <string>
#include <cassert>
#include <sstream>
#include <iostream>
#include <concepts>
#include <list>
#include <initializer_list>
#include <filesystem>
#include <algorithm>

namespace CU {
// To define the properties and flags of your command-line interface, you need to define the macro CLI_CONFIGURATION.
// The definition should list the required flags and properties using the following macros:
// - CLI_FLAG( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION )
// - CLI_VALUABLE_FLAG( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, VALIDATOR_OPTIONS )
// - CLI_OPTIONAL_PROPERTY( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, VALIDATOR_OPTIONS )
// - CLI_REQUIRED_PROPERTY( FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, VALIDATOR, VALIDATOR_OPTIONS )
// 
// Macro descriptions:
// - CLI_FLAG - defines a flag, an option without arguments.
//   In the CLIConfig structure, the presence or absence of the flag in the user input is represented by a boolean field
//   with the corresponding IDENTIFIER.
// - CLI_VALUABLE_FLAG - defines a flag with a possible value, an option with an optional argument.
//   In the CLIConfig structure, the presence or absence of the flag is represented by a boolean field prefixed
//   with has_ and the IDENTIFIER. If an argument is provided with the flag, its value will be stored in the field 
//   IDENTIFIER of type TYPE. If no argument is provided, the field IDENTIFIER will be initialized with the DEFAULT value.
// - CLI_OPTIONAL_PROPERTY - defines an optional property, an option with a mandatory argument,
//   the absence of which in the user input is not considered an error. The property's value is stored in the field
//   IDENTIFIER of type TYPE within the CLIConfig structure. If the property is not provided in the input,
//   the field IDENTIFIER will be initialized with the DEFAULT value.
// - CLI_REQUIRED_PROPERTY - defines a required property, an option with a mandatory argument,
//   the absence of which in the user input is considered an error. The property's value is stored in the field IDENTIFIER
//   of type TYPE within the CLIConfig structure.
// 
// Macro arguments:
// - FULL_NAME - the name of the property/flag that can be entered by the user with the -- prefix.
// - SHORT_NAME - an optional single - character abbreviation for the full name that can be entered by the user with the - prefix.
//   Defining the symbol requires the use of the SYMBOL(s) macro.If no symbol is provided, the WO_SYMBOL macro should be used.
// - IDENTIFIER - the identifier used to represent fields in the CLIConfig structure.
//   It must comply with the restrictions for a C++ identifier.
// - DESCRIPTION - a string providing a textual description of the property or flag.
// - TYPE - the type of the property or flag value.
// - DEFAULT - the default value of the property or flag
// - VALIDATOR - the type of the validator used, see code below.
// - VALIDATOR_OPTIONS - the options of the validator, see code below.
// 
// Example:
// #define CLI_CONFIGURATION
//      CLI_FLAG(test-flag1, SYMBOL(f), test_flag1, "first test flag")  \ /* --test-flag1 | -f */
//      CLI_FLAG(test-flag2, WO_SYMBOL, test_flag2, "second test flag") \ /* --test-flag2 */
//      CLI_VALUABLE_FLAG(val-flag, WO_SYMBOL, val_flag, "test flag with optional value", \ /* --val-flag */
//          int, 10, BaseValidator)
//      CLI_OPTIONAL_PROPERTY(optional-prop, SYMBOL(o), optional_prop, "test optional property", \ /* --optional-prop | -o */
//          float, 0.0f, RangeValidator, -1.0f, 1.0f )
//      CLI_REQUIRED_PROPERTY(required-prop, SYMBOL(r), required_prop, "test required property", \ /* --required-prop | -r */
//          std::string, ListValidator, "123", "abc", "qwe" )
// 
// Regardless of the flags/properties specified in CLI_CONFIGURATION, additional handling for the --help or -h flag is generated.
// When this flag is entered, help information is displayed.
// 
// The user can also specify authorship and license information in the CLI_ABOUT macro.
// This information will be displayed when help is requested with the --help or -h flag.
//

#ifndef CLI_CONFIGURATION
#define CLI_CONFIGURATION
#endif // !CLI_CONFIGURATION

    // parsing
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

    // validation
    template <typename OptionType>
    concept StreamInsertable = requires(std::ostream& os, OptionType value) {
        { os << value } -> std::same_as<std::ostream&>;
    };

    // validator without any limitations
    template <StreamInsertable OptionType>
    class BaseValidator {
    public:
        virtual bool CheckValue(const OptionType& value) const { return true; }
        virtual std::string GetDescription(std::string prefix = "") const {
            return "there are no restrictions applied to the value";
        }
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
        std::string GetDescription(std::string prefix = "") const override {
            std::stringstream result;
            result << "value must be within the range [" << m_min << ", " << m_max << "]";
            return result.str();
        }
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
        std::string GetDescription(std::string prefix = "") const override {
            std::stringstream result;
            result << "value must match one of the following:";
            for (auto value : m_valid_options) {
                result << "\n" << prefix << " - '" << value << "';";
            }
            return result.str();
        }
    private:
        const std::list<OptionType> m_valid_options;
    };

#ifdef CUSTOM_VALIDATORS
    // The user can define their own validator class analogously in the CUSTOM_VALIDATORS macro, and its code will be inserted here.
    CUSTOM_VALIDATORS
#endif // CUSTOM_VALIDATORS

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

// for the given CLI_CONFIGURATION the following objects are generated:
//  1) struct CLIConfig {
//         bool FLAG_IDENTIFIER = false;
//         bool has_VAL_FLAG_IDENTIFIER = false;
//         TYPE VAL_FLAG_IDENTIFIER = DEFAULT;
//         TYPE OPTIONAL_IDENTIFIER = DEFAULT;
//         TYPE REQUIRED_IDENTIFIER;
//         ...
//     };
//     Description: A structure containing the values of all options and properties specified in the CLI_CONFIGURATION.
// 
//  2) static bool parse_cli_args(int argc, char* const argv[], CLIConfig* config)
//     Description: A function that parses user command-line arguments into the CLIConfig structure.
//     Arguments:
//     - argc - An integer that contains the count of arguments that follow in argv.
//     - argv - An array of null - terminated strings representing command - line arguments entered by the user of the program.
//     - config - The resulting output config.
//     The function returns true if all arguments are successfully parsed and false if there is an input error or 
//     the user requested help with the --help or -h flag.
//
}
