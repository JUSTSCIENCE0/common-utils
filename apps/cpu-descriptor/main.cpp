// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT
#include <cpu-utils.hpp>

#include <iostream>
#include <bitset>
#include <stdint.h>

using std::cout, std::endl;

void print_about(const char* prog_name) {
    cout << prog_name << endl;
    cout << "Copyright (c) 2024, Yakov Usoltsev" << endl;
    cout << "Email: yakovmen62@gmail.com" << endl;
    cout << "License: MIT" << endl;
    cout << endl;
}

int main(/*int argc, char* argv[]*/) {
    print_about("cpu_descriptor");
    CU::CPUConfiguration cpu_conf = CU::read_cpu_configuration();
    cout << cpu_conf << endl;
    cout << CU::get_current_configuration_description() << endl;
    return 0;
}