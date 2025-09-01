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
        std::string result;
        result.reserve(wstr.size());
        for (wchar_t wc : wstr) {
            result.push_back(static_cast<char>(wc));
        }
        return result;
    }

    // TODO:
    // not portable utf8 <-> wstring
}
