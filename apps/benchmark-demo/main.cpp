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
    CU_PROFILE_CHECKBLOCK();
    some_work();
}

int main(int argc, char* argv[]) {
    USE_CU_PROFILE;

    CU_PROFILE_CHECKBLOCK(first);
    some_work();
    CU_STOP_CHECKBLOCK(first);

    CU_PROFILE_CHECKBLOCK();

    TestClass object;
    object.some_work();

    for (int i = 0; i < 5; i++) {
        test_profile();
    }

    return 0;
}
