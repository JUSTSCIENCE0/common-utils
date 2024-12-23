// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <profile-utils.hpp>

#include "some_class.h"

#include <thread>

void TestClass::some_work() {
    CU_PROFILE_CHECKPOINT;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CU_PROFILE_CHECKPOINT;
}
