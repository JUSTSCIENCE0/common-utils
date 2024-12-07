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
    std::cout << CU::PrivateImplementation::generate_optstring() << std::endl;

    if (argc < 2)
        return -1;

    using arg_type = CU::HEX;
    std::vector<arg_type> values(argc - 1);

    for (int i = 0; i < argc - 1; i++) {
        const char* current_arg = argv[i + 1];
        std::cout << "Argument: \"" << current_arg << "\"" << std::endl;

        if (!CU::parse_option(current_arg, &values[i])) {
            std::cout << "Parse error" << std::endl;
        }

        std::cout << "Value: " << values[i] << std::endl;

        if (!CU::validate_option(values[i], CU::ListValidator<arg_type>({ 0x123, 0x0, 50 }))) {
            std::cout << "Validation error" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}