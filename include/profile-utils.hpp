// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#ifndef ENABLE_CU_PROFILE
#define ENABLE_CU_PROFILE 0
#endif // !ENABLE_CU_PROFILE

#if ENABLE_CU_PROFILE
#include <iostream>

#define USE_CU_PROFILE         do { std::cout << "USE_CU_PROFILE" << std::endl; } while(0)
#define CU_PROFILE_CHECKPOINT  do { std::cout << "CU_PROFILE_CHECKPOINT " << \
    __FILE__ << " : " << __LINE__ << " - " << __FUNCTION__ << std::endl; } while(0)
#else
#define USE_CU_PROFILE
#define CU_PROFILE_CHECKPOINT
#endif

namespace CU {
}
