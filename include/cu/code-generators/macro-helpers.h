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

// helpers for selecting an implementation based on whether the macro has arguments.
// if arguments are present, the CU_PROFILE_NAMED_OPTION implementation will be selected
// otherwise, CU_ANONYMOUS_OPTION will be used.
#define CU_FUNC_CHOOSER(_f1, _f2, ...) _f2
#define CU_FUNC_RECOMPOSER(argsWithParentheses) CU_FUNC_CHOOSER argsWithParentheses
#define CU_CHOOSE_FROM_ARG_COUNT(...) CU_FUNC_RECOMPOSER((__VA_ARGS__, CU_PROFILE_NAMED_OPTION, ))
#define CU_NO_ARG_EXPANDER() ,CU_ANONYMOUS_OPTION
#define CU_CHECKBLOCK_MACRO_CHOOSER(...) CU_CHOOSE_FROM_ARG_COUNT(CU_NO_ARG_EXPANDER __VA_ARGS__ ())
