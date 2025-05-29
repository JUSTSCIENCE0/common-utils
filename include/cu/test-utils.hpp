// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(ENABLE_CU_TEST_UTILS)

#include <gtest/gtest.h>

#include <cu/file-utils.hpp>

#include <vector>
#include <functional>
#include <initializer_list>

namespace CU {
    template <typename Unit, typename... AdditionalArgs>
        requires std::is_fundamental_v<Unit>
    using ConformanceTestFunction = std::function<void(const Unit*, int64_t, Unit*, AdditionalArgs...)>;

    template <typename Unit, typename... AdditionalArgs>
    ConformanceTestFunction<Unit, AdditionalArgs...> make_conformance_test_func(
        void(*f)(const Unit*, int64_t, Unit*, AdditionalArgs...)) {
        return f;
    }

    template <typename Unit, typename... AdditionalArgs>
    using ConformanceTestFunctionsList = std::vector<ConformanceTestFunction<Unit, AdditionalArgs...>>;

    template <typename Unit, typename... AdditionalArgs>
    ConformanceTestFunctionsList<Unit, AdditionalArgs...> make_conformance_list(
        std::initializer_list<void(*)(const Unit*, int64_t, Unit*, AdditionalArgs...)> functions
    ) {
        ConformanceTestFunctionsList<Unit, AdditionalArgs...> result;
        for (auto func : functions) {
            result.push_back(make_conformance_test_func(func));
        }
        return result;
    }

    template <typename Unit, typename... AdditionalArgs>
    ConformanceTestFunctionsList<Unit, AdditionalArgs...> make_conformance_list(
        std::initializer_list<ConformanceTestFunction<Unit, AdditionalArgs...>> functions
    ) {
        return ConformanceTestFunctionsList<Unit, AdditionalArgs...>(functions);
    }

    template <typename Unit, typename... AdditionalArgs>
        requires std::is_fundamental_v<Unit>
    void run_conformance_test(
        const std::string& test_data_path,
        const std::string& control_data_path,
        ConformanceTestFunctionsList<Unit, AdditionalArgs...> test_functions,
        AdditionalArgs... additional_args) {
        auto input_data = CU::load_data_from_file<Unit>(test_data_path);
        ASSERT_FALSE(input_data.empty());

        auto output_data = CU::load_data_from_file<Unit>(control_data_path);
        ASSERT_FALSE(output_data.empty());

        std::vector<Unit> result_data(output_data.size());

        for (auto func : test_functions) {
            func(input_data.data(), input_data.size(), result_data.data(), std::forward(additional_args)...);

            for (unsigned i = 0; i < output_data.size(); i++) {
                ASSERT_EQ(output_data[i], result_data[i]);
            }
        }
    }
}

#endif
