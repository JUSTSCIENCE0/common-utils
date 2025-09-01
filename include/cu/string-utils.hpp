// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <string>

namespace CU {
    /// only single byte encoding
    static inline std::wstring str_to_wstr_simple(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }

    /// only single byte encoding
    static inline std::string wstr_to_str_simple(const std::wstring& wstr) {
        return std::string(wstr.begin(), wstr.end());
    }

    // TODO:
    // not portable utf8 <-> wstring
}
