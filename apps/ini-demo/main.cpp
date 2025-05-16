// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "sublibrary-demo/sublibrary-demo.h"

#include <cu/ini-utils.hpp>

#include <filesystem>

int main(int argc, char* argv[]) {
    std::cout << std::filesystem::current_path() << std::endl;
    std::cout << CU::get_current_module_filename() << std::endl;

    sublibrary_demo_print_library_name();
    return 0;
}