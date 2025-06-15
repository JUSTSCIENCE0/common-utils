// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <type_traits>
#include <algorithm>
#include <cmath>

namespace CU {
    template <typename FloatT>
        requires std::is_floating_point_v<FloatT>
    inline bool is_equal(FloatT lhs, FloatT rhs, FloatT absolute_epsilon = 1e-5f, FloatT relative_epsilon = 1e-5f) noexcept {
        auto abs_delta = std::abs(lhs - rhs);
        if (abs_delta < absolute_epsilon)
            return true;

        auto rel_delta = relative_epsilon * std::max(lhs, rhs);
        if (abs_delta < rel_delta)
            return true;

        return false;
    }
}
