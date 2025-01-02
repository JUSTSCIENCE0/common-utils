// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include "parsing-utils.hpp"
#include "validation-utils.hpp"

#if defined(_MSC_VER)
#include "external/getopt.h"
#else
#include <getopt.h>
#endif // MSVC

#include <initializer_list>
#include <filesystem>

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
