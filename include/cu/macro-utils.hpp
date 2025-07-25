// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

// preprocessor magic works here
#include <cu/code-generators/macro-helpers.h>

#define CU_REMOVE_PARENS(...) __VA_ARGS__

#define CU_CONCAT(lhs, rhs) lhs ##_ ##rhs
#define CU_EXPAND_CONCAT(lhs, rhs) CU_CONCAT(lhs, rhs)

#define CU_CONT_CONCAT(lhs, rhs) lhs ##rhs
#define CU_EXPAND_CONT_CONCAT(lhs, rhs) CU_CONT_CONCAT(lhs, rhs)

#define CU_STR(x) #x
#define CU_EXPAND_STR(x) CU_STR(x)

#define CU_STR_COMMA(x) CU_STR(x),

// thanks to David Mazieres
// see https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define CU_FOR_EACH(macro, ...) \
    __VA_OPT__(CU_EXPAND(CU_FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define CU_CONCAT_FOR_EACH(value, /*postfixes*/...) \
    __VA_OPT__(CU_EXPAND(CU_CONCAT_FOR_EACH_HELPER(value, __VA_ARGS__)))

// 0-2 args
#define CU_CHOOSE_MACRO_BY_ARGS_COUNT(name, ...) \
    CU_GENERATE_MACRO_IMPL_NAME(name, CU_GET_ARGS_COUNT(__VA_ARGS__))(__VA_ARGS__)
