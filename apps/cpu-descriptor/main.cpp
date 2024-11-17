// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT
#include <cpu-utils.h>

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

void print_all_arch_supported_sets() {
    using namespace CU;
    using std::bitset;

    cout << "All Instructions sets supported for this architecture:" << endl;
    for (int ins_fam = E_INFAM_BEGIN; ins_fam < E_INFAM_END; ins_fam++) {
        cout << "Instructions family #" << ins_fam << "\t" << STR_INSTRUCTIONS_FAMILY[ins_fam] << ",\t";
        cout << "SUPPORT_FLAG is " << bitset<32>(1ll << ins_fam) << endl;
    }
}

int main(/*int argc, char* argv[]*/) {
    print_about("cpu_descriptor");
    print_all_arch_supported_sets();
    return 0;
}