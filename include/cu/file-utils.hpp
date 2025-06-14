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
#include <cstring>

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#endif

#if defined(__linux__)
#include <dlfcn.h>
#include <unistd.h>
#include <linux/limits.h>
#endif

namespace CU {
    static inline std::string get_current_module_filename() {
        namespace fs = std::filesystem;

        constexpr int max_path = 1024;
#if  defined(_WIN32)
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
#elif defined(__linux__)
        char buf[max_path] = "";

        Dl_info info{};
        if (dladdr(reinterpret_cast<void*>(&get_current_module_filename), &info) && info.dli_fname) {
            std::strncpy(buf, info.dli_fname, max_path);
        }
        else {
            // fallback: /proc/self/exe
            // return file name for executable
            readlink("/proc/self/exe", buf, max_path);
        }
#else
        static_assert(!"Unsupported OS");
#endif //  _WIN32

        fs::path module_path(buf);
        return module_path.stem().string();
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

    template <typename T>
    concept FundamentalContainer = requires(T c) {
        typename T::value_type;
        { c.data() } -> std::convertible_to<const typename T::value_type *>;
        { c.size() } -> std::convertible_to<std::size_t>;
    } &&
    std::is_fundamental_v<typename T::value_type>;

    template<FundamentalContainer Container>
    bool save_data_to_file(const std::filesystem::path& file_name, const Container& values) {
        std::ofstream file_writer{ file_name, std::ios::binary };
        if (!file_writer) {
            // TODO: use log system, instead of stdout
            std::cout << "failed open file " << file_name << std::endl;
            return false;
        }

        if (!file_writer.write(
                reinterpret_cast<const char*>(values.data()),
                values.size() * sizeof(typename Container::value_type))) {
            // TODO: use log system, instead of stdout
            std::cout << "file " << file_name << " - write error" << std::endl;
            return false;
        }

        return true;
    }

}