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

// TODO: doc profiler interface
// USE_CU_PROFILE
// CU_PROFILE_CHECKBLOCK(NAME), NAME - optional
// CU_STOP_CHECKBLOCK(NAME)
//

#define USE_CU_PROFILE               do { std::cout << "USE_CU_PROFILE" << std::endl; } while(0)
#define CU_PROFILE_CHECKBLOCK(...)   CU_CHECKBLOCK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define CU_STOP_CHECKBLOCK(NAME)     NAME ##_timer.Stop()

// preprocessor magic
#define CU_FUNC_CHOOSER(_f1, _f2, ...) _f2
#define CU_FUNC_RECOMPOSER(argsWithParentheses) CU_FUNC_CHOOSER argsWithParentheses
#define CU_CHOOSE_FROM_ARG_COUNT(...) CU_FUNC_RECOMPOSER((__VA_ARGS__, CU_PROFILE_CHECKBLOCK_NAMED, ))
#define CU_NO_ARG_EXPANDER() ,CU_PROFILE_CHECKBLOCK_ANONYMOUS
#define CU_CHECKBLOCK_MACRO_CHOOSER(...) CU_CHOOSE_FROM_ARG_COUNT(CU_NO_ARG_EXPANDER __VA_ARGS__ ())

#define CU_PROFILE_CHECKBLOCK_ANONYMOUS() CU::CheckBlockTimer CU_TIMER_NAME( "", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__);
#define CU_PROFILE_CHECKBLOCK_NAMED(NAME) CU::CheckBlockTimer NAME ##_timer( std::string(#NAME) + ": ", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__);

#define CU_CONCAT_(lhs, rhs) lhs ## rhs
#define CU_CONCAT(lhs, rhs) CU_CONCAT_(lhs, rhs)
#define CU_TIMER_NAME CU_CONCAT(timer_, __LINE__)

// implementation
namespace CU {
    class CheckBlockTimer {
    public:
        CheckBlockTimer(const CheckBlockTimer&)            = delete;
        CheckBlockTimer(CheckBlockTimer&&)                 = delete;
        CheckBlockTimer& operator=(const CheckBlockTimer&) = delete;
        CheckBlockTimer& operator=(CheckBlockTimer&&)      = delete;

        CheckBlockTimer(
            const std::string& _prefix,
            const std::string& _file,
            int _line,
            const std::string& _function) :
            m_identifier(_prefix + _file + ", " + std::to_string(_line) + " - " + _function),
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
#define CU_PROFILE_CHECKBLOCK(...)
#define CU_STOP_CHECKBLOCK(NAME)
#endif
