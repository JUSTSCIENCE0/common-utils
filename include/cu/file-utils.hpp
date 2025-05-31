// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#include <linux/limits.h>
#endif

namespace CU {
    static inline std::string get_current_module_filename() {
        namespace fs = std::filesystem;

#if  defined(_WIN32)
        constexpr DWORD max_path = 1024;
        wchar_t buf[max_path] = L"";

        HMODULE hModule = nullptr;
        if (0 == GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&get_current_module_filename),
            &hModule)) {
            // TODO: use log system, instead of stdout
            std::cout << "failed to get current module handle" << std::endl;
            // return file name for executable
            hModule = nullptr;
        }
        GetModuleFileName(hModule, buf, max_path);

        fs::path module_path(buf);
        return module_path.stem().string();
#else
        static_assert(false, "Unsupported OS");
#endif //  _WIN32
    }

    template<typename Unit>
    requires std::is_fundamental_v<Unit>
    std::vector<Unit> load_data_from_file(const std::filesystem::path& file_name) {
        if (!std::filesystem::exists(file_name)) {
            // TODO: use log system, instead of stdout
            std::cout << "selected file " << file_name << " doesn't exist" << std::endl;
            return {};
        }

        std::ifstream file_reader{ file_name, std::ios::binary | std::ios::ate };
        if (!file_reader) {
            // TODO: use log system, instead of stdout
            std::cout << "failed open file " << file_name << std::endl;
            return {};
        }

        auto file_size = file_reader.tellg();
        file_reader.seekg(std::ios::beg);
        if (file_size % sizeof(Unit)) {
            // TODO: use log system, instead of stdout
            std::cout << "file size " << file_size <<
                " is not a multiple of the unit size " << sizeof(Unit) << std::endl;
        }

        auto units_count = file_size / sizeof(Unit);
        std::vector<Unit> result(units_count);

        if (!file_reader.read(reinterpret_cast<char*>(result.data()), units_count * sizeof(Unit))) {
            // TODO: use log system, instead of stdout
            std::cout << "file " << file_name << " - read error" << std::endl;
            return {};
        }

        return result;
    }
}