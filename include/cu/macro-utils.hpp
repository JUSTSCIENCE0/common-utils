// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#define CU_CONCAT(lhs, rhs) lhs ##_ ## rhs
#define CU_EXPAND_CONCAT(lhs, rhs) CU_CONCAT(lhs, rhs)

#define CU_CONT_CONCAT(lhs, rhs) lhs ## rhs
#define CU_EXPAND_CONT_CONCAT(lhs, rhs) CU_CONT_CONCAT(lhs, rhs)

#define CU_STR(x) #x
#define CU_EXPAND_STR(x) CU_STR(x)
