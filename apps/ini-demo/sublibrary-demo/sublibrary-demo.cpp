// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "sublibrary-demo.h"

#include <cu/os-utils.hpp>

void sublibrary_demo_print_library_name() {
    std::cout << CU::get_current_module_filename() << std::endl;
}
