// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <string>
#include <filesystem>
#include <iostream>

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
            //TODO: use log system, instead of stdout
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
}