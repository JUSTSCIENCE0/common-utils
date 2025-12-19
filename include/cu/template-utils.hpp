// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <type_traits>
#include <cstddef>

namespace CU {
    /// contains_v
    template<typename T, typename... Ts>
    constexpr bool contains_v = (std::is_same_v<T, Ts> || ...);

    /// type_index_v
    template<typename T, typename... Ts>
    struct type_index;

    template<typename T, typename... Rest>
    struct type_index<T, T, Rest...> {
        static constexpr std::size_t value = 0;
    };

    template<typename T, typename First, typename... Rest>
    struct type_index<T, First, Rest...> {
        static constexpr size_t value =
            1 + type_index<T, Rest...>::value;
    };

    template<typename T>
    struct type_index<T> {
        static_assert(!std::is_same_v<T, T>, "Type not found in type list");
    };

    template<typename T, typename... Ts>
    constexpr size_t type_index_v = type_index<T, Ts...>::value;
}
