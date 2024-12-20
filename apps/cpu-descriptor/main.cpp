// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#define CLI_CONFIGURATION \
    CLI_FLAG(conf-descr, SYMBOL(d), print_configuration_description, "print only current cpu configuration description")

#define CLI_ABOUT \
    "Copyright (c) 2024, Yakov Usoltsev\n" \
    "Email: yakovmen62@gmail.com\n" \
    "License: MIT"

#include <cli-utils.hpp>
#include <cpu-utils.hpp>

#include <iostream>
#include <bitset>
#include <stdint.h>

using std::cout, std::endl;

int main(int argc, char* argv[]) {
    CU::CLIConfig cli_config{};
    if (!CU::parse_cli_args(argc, argv, &cli_config))
        return -1;

    if (cli_config.print_configuration_description) {
        cout << CU::get_current_configuration_description() << endl;
        return 0;
    }

    cout << CU::CURRENT_CPU_CONFIGURATION << endl;
    return 0;
}