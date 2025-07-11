// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

// do not use separately from macro-utils.hpp

// helpers for recursive CU_FOR_EACH and CU_CONCAT_FOR_EACH
// see https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define CU_PARENS ()
#define CU_EXPAND(...) CU_EXPAND4(CU_EXPAND4(CU_EXPAND4(CU_EXPAND4(__VA_ARGS__))))
#define CU_EXPAND4(...) CU_EXPAND3(CU_EXPAND3(CU_EXPAND3(CU_EXPAND3(__VA_ARGS__))))
#define CU_EXPAND3(...) CU_EXPAND2(CU_EXPAND2(CU_EXPAND2(CU_EXPAND2(__VA_ARGS__))))
#define CU_EXPAND2(...) CU_EXPAND1(CU_EXPAND1(CU_EXPAND1(CU_EXPAND1(__VA_ARGS__))))
#define CU_EXPAND1(...) __VA_ARGS__
#define CU_FOR_EACH_HELPER(macro, a1, ...) \
    macro(a1) \
    __VA_OPT__(CU_FOR_EACH_AGAIN CU_PARENS (macro, __VA_ARGS__))
#define CU_FOR_EACH_AGAIN() CU_FOR_EACH_HELPER
#define CU_CONCAT_FOR_EACH_HELPER(value, a1, ...) \
    CU_EXPAND_CONCAT(value, a1), \
    __VA_OPT__(CU_CONCAT_FOR_EACH_AGAIN CU_PARENS (value, __VA_ARGS__))
#define CU_CONCAT_FOR_EACH_AGAIN() CU_CONCAT_FOR_EACH_HELPER

// helpers for selecting an implementation based on the number of arguments passed to a macro.
// supports up to 2 arguments
#define CU_GET_ARG_NO_3(_1, _2, N, ...) N
#define CU_GET_ARGS_COUNT(...) CU_GET_ARG_NO_3(__VA_ARGS__ __VA_OPT__(,) 2, 1, 0)
#define CU_GENERATE_MACRO_IMPL_NAME(name, count) CU_EXPAND_CONCAT(name, count)
