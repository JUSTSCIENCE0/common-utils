// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "profile-utils.hpp"

#if ENABLE_CU_PROFILE
namespace CU {
    std::mutex ProfilerAggregator::m_timer_results_lock;
    std::unordered_map<std::string, ProfilerAggregator::TimerResult> ProfilerAggregator::m_timer_results;

    std::string scale_time_duration_ns(int64_t nanosec) {
        assert(nanosec >= 0);

        if (nanosec < 1000) {
            return std::to_string(nanosec) + " ns.";
        }

        if (nanosec < 1000000) {
            constexpr double TIMESCALE = 1e-3;
            return std::to_string(double(nanosec) * TIMESCALE) + " us.";
        }

        if (nanosec < 1000000000) {
            constexpr double TIMESCALE = 1e-6;
            return std::to_string(double(nanosec) * TIMESCALE) + " ms.";
        }

        constexpr double TIMESCALE = 1e-9;
        return std::to_string(double(nanosec) * TIMESCALE) + " s.";
    }

    std::ostream& operator<<(std::ostream& os, const ProfilerAggregator::TimerResult& tr) {
        auto average_time = int64_t(double(tr.m_total_duration_ns) / double(tr.m_activations_count));

        os << "\tminimum duration = " << scale_time_duration_ns(tr.m_min_duration_ns) << std::endl;
        os << "\tmaximum duration = " << scale_time_duration_ns(tr.m_max_duration_ns) << std::endl;
        os << "\taverage duration = " << scale_time_duration_ns(average_time) << std::endl;
        os << "\tactivations count = " << tr.m_activations_count << std::endl;
        os << "\ttotal = " << scale_time_duration_ns(tr.m_total_duration_ns) << std::endl;

        return os;
    }
}
#endif