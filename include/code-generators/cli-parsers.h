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
#define SYMBOL(s) = #s[0], E_ ##s ##_NEXT_VAL = 256 + #s[0]
// An additional enum is required because:
// 1) It allows detecting duplicate symbolic options during the creation of E_OPTIONS.
// 2) It helps avoid collisions between auto-incremented enum values and symbolic characters.
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

    static std::string get_usage(const char* bin_path) {
        std::string result = "usage:\n";

        using path = std::filesystem::path;
        result += path(bin_path).stem().string() + " [-h|--help] ";

#define SYMBOL(s) "-" #s "|"
#define WO_SYMBOL
#define CLI_FLAG(FULL_NAME, SHORT_NAME, ...) \
        result += "[" SHORT_NAME "--" #FULL_NAME "] ";
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
        result += "[" SHORT_NAME "--" #FULL_NAME "[=<" #TYPE " " #IDENTIFIER ">]] ";
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
        result += "[" SHORT_NAME "--" #FULL_NAME "=<" #TYPE " " #IDENTIFIER ">] ";
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
        result +=     SHORT_NAME "--" #FULL_NAME "=<" #TYPE " " #IDENTIFIER "> ";

        CLI_CONFIGURATION
        return result;
    }
#undef SYMBOL
#undef WO_SYMBOL
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

    static std::string get_help(const char* bin_path) {
        std::stringstream result;
        result << get_usage(bin_path);
        result << "\n\noptions:\n";
        result << "  --help, -h\n";
        result << "    show this help message\n";

#define SYMBOL(s) ", -" #s
#define WO_SYMBOL
#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION) \
        result << "  --" #FULL_NAME SHORT_NAME "\n"; \
        result << "    " DESCRIPTION "\n";
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, ...) \
        result << "  --" #FULL_NAME SHORT_NAME " -- [" #TYPE " " #IDENTIFIER "]\n"; \
        result << "    " DESCRIPTION "\n"; \
        result << "    value type: " #TYPE ", default value = '" << DEFAULT << "'\n"; \
        result << "    validation: " << VALIDATOR<TYPE>{__VA_ARGS__}.GetDescription("    ") << "\n";
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, ...) \
        result << "  --" #FULL_NAME SHORT_NAME " -- "  #TYPE " " #IDENTIFIER "\n"; \
        result << "    " DESCRIPTION "\n"; \
        result << "    value type: " #TYPE ", default value = '" << DEFAULT << "'\n"; \
        result << "    validation: " << VALIDATOR<TYPE>{__VA_ARGS__}.GetDescription("    ") << "\n";
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, VALIDATOR, ...) \
        result << "  --" #FULL_NAME SHORT_NAME " -- "  #TYPE " " #IDENTIFIER "\n"; \
        result << "    " DESCRIPTION "\n"; \
        result << "    value type: " #TYPE "\n"; \
        result << "    validation: " << VALIDATOR<TYPE>{__VA_ARGS__}.GetDescription("    ") << "\n";

        CLI_CONFIGURATION
        return result.str();
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
} // namespace PrivateImplementation

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

    bool operator==(const CLIConfig&) const = default;
};

#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

static bool parse_cli_args(int argc, char* const argv[], CLIConfig* config) {
    if (argc < 1 || !argv) {
        std::cerr << "invalid arguments" << std::endl;
        return false;
    }

    using namespace PrivateImplementation;

    // reset getopt
    optind = 0;
    optreset = 1;

    // required options handlers
#define CLI_FLAG(...)
#define CLI_VALUABLE_FLAG(...)
#define CLI_OPTIONAL_PROPERTY(...)
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
    bool has_ ##IDENTIFIER = false;
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
                    std::cerr << "flag '" #FULL_NAME "' " \
                                 "has invalid option -- " << optarg << std::endl; \
                    return false; \
                } \
            } \
            break;
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
        case E_ ##IDENTIFIER: \
            if (!parse_option(optarg, &config->IDENTIFIER)) { \
                std::cerr << "option '" #FULL_NAME "' " \
                             "has invalid value -- " << optarg << std::endl; \
                return false; \
            } \
            break;
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, ...) \
        case E_ ##IDENTIFIER: { \
            if (!parse_option(optarg, &config->IDENTIFIER)) { \
                std::cerr << "option '" #FULL_NAME "' " \
                             "has invalid value -- " << optarg << std::endl; \
                return false; \
            } \
            has_ ##IDENTIFIER = true; \
            break; \
        }
            CLI_CONFIGURATION
        case E_HELP:
            std::cout << get_help(argv[0]) << std::endl;
            return false;
        case '?':
        default:
            return false;
        } // switch
    } // while
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

    // check required properties
#define CLI_FLAG(...)
#define CLI_VALUABLE_FLAG(...)
#define CLI_OPTIONAL_PROPERTY(...)
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, ...) \
    if (!has_ ##IDENTIFIER) { \
        std::cerr << "mandatory option is missing -- " #FULL_NAME << std::endl; \
        return false; \
    }
    CLI_CONFIGURATION
#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY

    // validate options
#define VALIDATE(FULL_NAME, IDENTIFIER, TYPE, VALIDATOR, ...) { \
    VALIDATOR<TYPE> validator{__VA_ARGS__}; \
    if (!validate_option(config->IDENTIFIER, validator)) { \
        std::cerr << #FULL_NAME " -- value '" << config->IDENTIFIER << "' don't pass validation" << std::endl; \
        std::cerr << validator.GetDescription(); \
        return false; \
        } \
    }

#define CLI_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION)
#define CLI_VALUABLE_FLAG(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, ...) \
    VALIDATE(FULL_NAME, IDENTIFIER, TYPE, VALIDATOR, __VA_ARGS__)
#define CLI_OPTIONAL_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, DEFAULT, VALIDATOR, ...) \
    VALIDATE(FULL_NAME, IDENTIFIER, TYPE, VALIDATOR, __VA_ARGS__)
#define CLI_REQUIRED_PROPERTY(FULL_NAME, SHORT_NAME, IDENTIFIER, DESCRIPTION, TYPE, VALIDATOR, ...) \
    VALIDATE(FULL_NAME, IDENTIFIER, TYPE, VALIDATOR, __VA_ARGS__)
    CLI_CONFIGURATION

    return true;
} // parse_cli_args

#undef CLI_FLAG
#undef CLI_VALUABLE_FLAG
#undef CLI_OPTIONAL_PROPERTY
#undef CLI_REQUIRED_PROPERTY
