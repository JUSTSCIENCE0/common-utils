// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#define CLI_CONFIGURATION \
    CLI_FLAG(test-flag1, SYMBOL(f), test_flag1, "first test flag")  \
    CLI_FLAG(test-flag2, WO_SYMBOL, test_flag2, "second test flag") \
    CLI_VALUABLE_FLAG(val-flag, WO_SYMBOL, val_flag, "test flag with optional value", int, 10, BaseValidator) \
    CLI_OPTIONAL_PROPERTY(optional-prop, SYMBOL(o), optional_prop, "test optional property", \
        float, 0.0f, RangeValidator, -1.0f, 1.0f ) \
    CLI_REQUIRED_PROPERTY(required-prop, SYMBOL(r), required_prop, "test required property", \
        std::string, ListValidator, { "123", "abc", "qwe" } )

#include "cli-utils.hpp"

#include <vector>

int main(int argc, char* argv[]) {
    CU::CLIConfig cli_config{};
    if (!CU::parse_cli_args(argc, argv, &cli_config))
        return -1;

    return 0;
}