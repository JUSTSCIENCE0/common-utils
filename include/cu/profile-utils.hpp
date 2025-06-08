// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(ENABLE_CU_PROFILE)
#include <cu/macro-utils.hpp>

#include <iostream>
#include <string>
#include <chrono>
#include <unordered_map>
#include <cassert>
#include <mutex>
#include <ranges>
#include <algorithm>

// Macro descriptions:
//
// USE_CU_PROFILE
// Macro that enables automatic logging of data collected from all timers when the executable module terminates.
// This macro must be called at least once within the executable module to activate the logging upon exit.
// If the macro is not called, timers created with CU_PROFILE_CHECKBLOCK will still function and store
// their measurement results in memory, but no automatic logging will occur.
// The measurement results can still be retrieved by calling CU_PROFILE_GET_RESULTS.
//
// CU_PROFILE_CHECKBLOCK(NAME, KEY) (NAME and KEY are optional)
// Macro that creates a timer measuring the duration from its creation to the end of the current code block
// (i.e., the end of the current compound statement).
// If NAME is provided, a named timer is created,
// which can be manually stopped before the block ends using CU_STOP_CHECKBLOCK.
// If NAME is omitted, an anonymous timer is created that cannot be stopped early.
// After stopping, the timer stores the measurement results.
// If KEY is provided, the created named timer will use the variable passed in KEY as the key for storing results.
// This allows a single named timer to save its measurements under different keys,
// depending on the value of the variable in KEY. KEY must be convertible to std::string.
//
// CU_STOP_CHECKBLOCK(NAME)
// Macro that stops the timer with the specified NAME.
//
// CU_PROFILE_GET_RESULTS(FILTER) (FILTER is optional)
// Macro that returns all available measurement results.
// If FILTER is provided, only the results matching the keys in FILTER will be returned.
// FILTER must be a range-iterating container and contain values convertible to std::string

#define USE_CU_PROFILE               CU::ProfilerAggregator::Setup()
#define CU_PROFILE_CHECKBLOCK(...)   CU_CHOOSE_MACRO_BY_ARGS_COUNT(CU_PROFILE_CHECKBLOCK, __VA_ARGS__)
#define CU_STOP_CHECKBLOCK(NAME)     NAME ##_timer.Stop()
#define CU_PROFILE_GET_RESULTS(...)  CU::ProfilerAggregator::GetCurrentResults(__VA_ARGS__)

// Implementation
#define CU_TIMER_NAME CU_EXPAND_CONCAT(timer, __LINE__)
#define CU_PROFILE_CHECKBLOCK_0() CU::CheckBlockTimer CU_TIMER_NAME { "", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__}
#define CU_PROFILE_CHECKBLOCK_1(NAME) CU::CheckBlockTimer NAME ##_timer{ std::string(#NAME) + ": ", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__}
#define CU_PROFILE_CHECKBLOCK_2(NAME, KEY) CU::CheckBlockTimer NAME ##_timer{ std::string(KEY) + ": ", \
                                              std::string(__FILE__).erase(0, CU_PREFIX_LENGTH + 1), __LINE__, __FUNCTION__}

// implementation
namespace CU {
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

        int64_t GetAvgNS() const {
            if (!m_activations_count)
                return 0;

            return m_total_duration_ns / m_activations_count;
        }
    };
    std::ostream& operator<<(std::ostream& os, const TimerResult& tr);

    class ProfilerAggregator {
    public:
        using TimersResults = std::unordered_map<std::string, TimerResult>;

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

        static TimersResults GetCurrentResults() {
            std::scoped_lock _(m_timer_results_lock);
            return m_timer_results;
        }

        template<typename StrKeyContainer>
            requires std::ranges::range<StrKeyContainer> &&
                     std::is_convertible_v<std::ranges::range_value_t<StrKeyContainer>, std::string>
        static TimersResults GetCurrentResults(StrKeyContainer&& filter) {
            auto all_results = GetCurrentResults();

            TimersResults filtered_results;

            for (const std::string& key : filter) {
                auto has_selected_key = [key](const TimersResults::value_type& node) {
                    return node.first.compare(0, key.size(), key) == 0;
                    };

                const auto key_result = std::find_if(
                    all_results.begin(),
                    all_results.end(),
                    has_selected_key);

                if (all_results.end() == key_result) {
                    // TODO: ability to use a logging system depending on the configuration.
                    constexpr std::ostream& out = std::cout;
                    out << "Key <" << key << "> not found in timers results" << std::endl;
                    continue;
                }

                filtered_results[key] = key_result->second;
            }
            return filtered_results;
        }

    private:
        ProfilerAggregator(
            // TODO: configuration
        ) = default;

        static std::mutex m_timer_results_lock;
        static TimersResults m_timer_results;
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
#define CU_PROFILE_GET_RESULTS() {}
#endif
