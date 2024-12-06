// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

// do not use separately from cli-utils.hpp

#ifndef CLI_CONFIGURATION
#error "CLI configuration is not defined"
#endif

namespace PrivateImplementation {

    enum E_OPTIONS {
#define SYMBOL(s) = #s[0]
#define WO_SYMBOL
#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        E_ ##IDENTIFIER SHORT_NAME,
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        E_ ##IDENTIFIER SHORT_NAME,
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        E_ ##IDENTIFIER SHORT_NAME,
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        E_ ##IDENTIFIER SHORT_NAME,

        CLI_CONFIGURATION
        E_HELP = 'h'
    };

#undef SYMBOL
#undef WO_SYMBOL
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

    static std::string generate_optstring() {
        std::string optstring = "h";

#define SYMBOL(s) #s
#define WO_SYMBOL nullptr
#define CLI_FLAG(FULL_NAME, SHORT_NAME, ...) \
        if constexpr (SHORT_NAME) { optstring += SHORT_NAME; }
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, ...) \
        if constexpr (SHORT_NAME) { optstring += SHORT_NAME; optstring += "::"; }
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, ...) \
        if constexpr (SHORT_NAME) { optstring += SHORT_NAME; optstring += ":"; }
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, ...) \
        if constexpr (SHORT_NAME) { optstring += SHORT_NAME; optstring += ":"; }

        CLI_CONFIGURATION
        return optstring;
    }

#undef SYMBOL
#undef WO_SYMBOL
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

    option OptionDescriptions[] = {
#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        { #FULL_NAME, no_argument, NULL, E_ ##IDENTIFIER },
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        { #FULL_NAME, optional_argument, NULL, E_ ##IDENTIFIER },
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        { #FULL_NAME, required_argument, NULL, E_ ##IDENTIFIER },
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        { #FULL_NAME, required_argument, NULL, E_ ##IDENTIFIER },

        CLI_CONFIGURATION
        { "help", no_argument, NULL, E_HELP },
        { 0, 0, 0, 0 }
    };

#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

} // namespace PrivateCLIImplementation

struct CLIConfig {
#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
    bool IDENTIFIER = false;
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, ...) \
    bool has_ ##IDENTIFIER = false; \
    TYPE IDENTIFIER = DEFAULT;
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, ...) \
    TYPE IDENTIFIER = DEFAULT;
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
    TYPE IDENTIFIER;

    CLI_CONFIGURATION
};

#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

static bool parse_cli_args(int argc, char* const argv[], CLIConfig* config) {
    using namespace PrivateImplementation;

    // required options handlers
#define CLI_FLAG(...)
#define CLI_VALUABLE_FLAG(...)
#define CLI_OPTIONAL_PROPERTY(...)
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
    std::optional<TYPE> required_ ##IDENTIFIER = std::nullopt;
    CLI_CONFIGURATION
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY
    int option_index = 0;

    while (true) {
        int option_id = getopt_long(
            argc, argv, 
            generate_optstring().c_str(),
            OptionDescriptions,
            &option_index);
        if (-1 == option_id) break;

        switch (option_id) {
#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        case E_ ##IDENTIFIER: \
            config->IDENTIFIER = true; \
            break;
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        case E_ ##IDENTIFIER: \
            config->has_ ##IDENTIFIER = true; \
            if (optarg) { \
                if (!parse_option(optarg, &config->IDENTIFIER)) { \
                    return false; /* TODO: print error */\
                } \
            } \
            break;
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        case E_ ##IDENTIFIER: \
            if (!parse_option(optarg, &config->IDENTIFIER)) { \
                return false; /* TODO: print error */\
            } \
            break;
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        case E_ ##IDENTIFIER: \
            /* TODO */ \
            break;

            CLI_CONFIGURATION
        case '?':
            // TODO: print error
            return false;
        default:
            // TODO: print error
            return false;
        } // switch
    } // while




    return true;
} // parse_cli_args

#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

//#define SYMBOL(s)
//#define WO_SYMBOL
//#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION)
//#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR)
//#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR)
//#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, VALIDATOR)
