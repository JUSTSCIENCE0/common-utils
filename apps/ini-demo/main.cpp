// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "ini-utils.hpp"

#include <filesystem>

int main(int argc, char* argv[]) {
    std::cout << std::filesystem::current_path() << std::endl;
    std::cout << CU::get_current_module_filename() << std::endl;
    return 0;
}