// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT
#include "cli-utils.hpp"

#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2)
        return -1;

    using arg_type = long double;
    std::vector<arg_type> values(argc - 1);

    for (int i = 0; i < argc - 1; i++) {
        const char* current_arg = argv[i + 1];
        std::cout << "Argument: \"" << current_arg << "\"" << std::endl;

        if (!CU::parse_option(current_arg, values[i])) {
            std::cout << "Parse error" << std::endl;
        }

        std::cout << "Value: " << values[i] << std::endl;

        if (!CU::validate_option(values[i], CU::RangeValidator<arg_type>(0, 50))) {
            std::cout << "Validation error" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}