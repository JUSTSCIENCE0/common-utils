// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <profile-utils.hpp>

#include "some_class.h"

#include <thread>

static inline void some_work() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

static inline void test_profile() {
    CU_PROFILE_CHECKPOINT;
    some_work();
    CU_PROFILE_CHECKPOINT;
}

int main(int argc, char* argv[]) {
    USE_CU_PROFILE;

    CU_PROFILE_CHECKPOINT;

    some_work();

    CU_PROFILE_CHECKPOINT;

    TestClass object;
    object.some_work();
    test_profile();

    CU_PROFILE_CHECKPOINT;

    return 0;
}
