// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "profile-utils.hpp"

#if ENABLE_CU_PROFILE
namespace CU {
    std::unordered_map<std::string, ProfilerAggregator::TimerResult> ProfilerAggregator::m_timer_results;

    std::ostream& operator<<(std::ostream& os, const ProfilerAggregator::TimerResult& tr) {
        // TODO: configurable timescale
        // now timescale - milliseconds
        constexpr double TIMESCALE = 1e-6;
        constexpr std::string TIMESCALE_NAME = "ms.";

        double scaledTotalTime = double(tr.m_total_duration_ns) * TIMESCALE;
        double scaledAverageTime = scaledTotalTime / double(tr.m_activations_count);
        double scaledMinTime = double(tr.m_min_duration_ns) * TIMESCALE;
        double scaledMaxTime = double(tr.m_max_duration_ns) * TIMESCALE;

        os << "\tminimum duration = " << scaledMinTime << " " << TIMESCALE_NAME << std::endl;
        os << "\tmaximum duration = " << scaledMaxTime << " " << TIMESCALE_NAME << std::endl;
        os << "\taverage duration = " << scaledAverageTime << " " << TIMESCALE_NAME << std::endl;
        os << "\tactivations count = " << tr.m_activations_count << std::endl;
        os << "\ttotal = " << scaledTotalTime << TIMESCALE_NAME << std::endl;

        return os;
    }
}
#endif