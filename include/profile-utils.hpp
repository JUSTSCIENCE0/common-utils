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
#include <string>
#include <chrono>

#define USE_CU_PROFILE         do { std::cout << "USE_CU_PROFILE" << std::endl; } while(0)
#define CU_PROFILE_CHECKBLOCK  CU::CheckBlockTimer CU_TIMER_NAME( \
                                    std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__);

#define CU_CONCAT_(a, b) a ## b
#define CU_CONCAT(a, b) CU_CONCAT_(a, b)
#define CU_TIMER_NAME CU_CONCAT(timer_, __LINE__)

namespace CU {
    class CheckBlockTimer {
    public:
        CheckBlockTimer(const CheckBlockTimer&)            = delete;
        CheckBlockTimer(CheckBlockTimer&&)                 = delete;
        CheckBlockTimer& operator=(const CheckBlockTimer&) = delete;
        CheckBlockTimer& operator=(CheckBlockTimer&&)      = delete;

        CheckBlockTimer(const std::string& _file, int _line, const std::string& _function) :
            m_identifier(_file + " : " + std::to_string(_line) + " - " + _function),
            m_start_time(clock::now()) {}

        ~CheckBlockTimer() { Stop(); }

        void Stop() {
            if (m_is_stopped) return;

            auto elapsed_time = std::chrono::duration_cast<nano>(clock::now() - m_start_time).count();
            m_is_stopped = true;

            // TODO: store elapsed time to global object
            std::cout << m_identifier << " - elapsed time = " << elapsed_time << std::endl;
        }

    private:
        using nano = std::chrono::nanoseconds;
        using clock = std::chrono::steady_clock;

        const std::string       m_identifier;
        const clock::time_point m_start_time;

        bool m_is_stopped = false;
    };
}

#else
#define USE_CU_PROFILE
#define CU_PROFILE_CHECKBLOCK
#endif
