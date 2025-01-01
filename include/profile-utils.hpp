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
#include <unordered_map>
#include <cassert>
#include <mutex>

// TODO: doc profiler interface
// USE_CU_PROFILE
// CU_PROFILE_CHECKBLOCK(NAME), NAME - optional
// CU_STOP_CHECKBLOCK(NAME)
//

#define USE_CU_PROFILE               CU::ProfilerAggregator::Setup()
#define CU_PROFILE_CHECKBLOCK(...)   CU_CHECKBLOCK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define CU_STOP_CHECKBLOCK(NAME)     NAME ##_timer.Stop()

// preprocessor magic
#define CU_FUNC_CHOOSER(_f1, _f2, ...) _f2
#define CU_FUNC_RECOMPOSER(argsWithParentheses) CU_FUNC_CHOOSER argsWithParentheses
#define CU_CHOOSE_FROM_ARG_COUNT(...) CU_FUNC_RECOMPOSER((__VA_ARGS__, CU_PROFILE_CHECKBLOCK_NAMED, ))
#define CU_NO_ARG_EXPANDER() ,CU_PROFILE_CHECKBLOCK_ANONYMOUS
#define CU_CHECKBLOCK_MACRO_CHOOSER(...) CU_CHOOSE_FROM_ARG_COUNT(CU_NO_ARG_EXPANDER __VA_ARGS__ ())

#define CU_PROFILE_CHECKBLOCK_ANONYMOUS() CU::CheckBlockTimer CU_TIMER_NAME( "", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__)
#define CU_PROFILE_CHECKBLOCK_NAMED(NAME) CU::CheckBlockTimer NAME ##_timer( std::string(#NAME) + ": ", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__)

#define CU_CONCAT_(lhs, rhs) lhs ## rhs
#define CU_CONCAT(lhs, rhs) CU_CONCAT_(lhs, rhs)
#define CU_TIMER_NAME CU_CONCAT(timer_, __LINE__)

// implementation
namespace CU {
    class ProfilerAggregator {
    public:
        static void Setup(
            // TODO: configuration
        ) {
            static ProfilerAggregator profiler{};
        }

        ProfilerAggregator(const ProfilerAggregator&)            = delete;
        ProfilerAggregator(ProfilerAggregator&&)                 = delete;
        ProfilerAggregator& operator=(const ProfilerAggregator&) = delete;
        ProfilerAggregator& operator=(ProfilerAggregator&&)      = delete;

        ~ProfilerAggregator() {
            // TODO: ability to use a logging system depending on the configuration.
            constexpr std::ostream& out = std::cout;

            out << "Profiler results:" << std::endl;

            std::scoped_lock _(m_timer_results_lock);
            for (const auto& [timer_id, timer_result] : m_timer_results) {
                out << timer_id << ":" << std::endl;
                out << timer_result << std::endl;
            }
        }

        static void NotifyTimer(const std::string& timer_id, int64_t duration_ns) {
            std::scoped_lock _(m_timer_results_lock);
            m_timer_results[timer_id].StoreDuration(duration_ns);
        }

    private:
        ProfilerAggregator(
            // TODO: configuration
        ) = default;

        struct TimerResult {
            int64_t m_activations_count = 0;
            int64_t m_total_duration_ns = 0;
            int64_t m_min_duration_ns = std::numeric_limits<int64_t>::max();
            int64_t m_max_duration_ns = -1;

            void StoreDuration(int64_t duration_ns) {
                assert(duration_ns >= 0);

                m_activations_count++;
                m_total_duration_ns += duration_ns;
                if (m_min_duration_ns > duration_ns)
                    m_min_duration_ns = duration_ns;
                if (m_max_duration_ns < duration_ns)
                    m_max_duration_ns = duration_ns;
            }
        };

        static std::mutex m_timer_results_lock;
        static std::unordered_map<std::string, TimerResult> m_timer_results;

        friend std::ostream& operator<<(std::ostream& os, const ProfilerAggregator::TimerResult& tr);
    };

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

            auto elapsed_time_ns = std::chrono::duration_cast<nano>(clock::now() - m_start_time).count();
            m_is_stopped = true;

            ProfilerAggregator::NotifyTimer(m_identifier, elapsed_time_ns);
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
