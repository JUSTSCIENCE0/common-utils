// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(ENABLE_CU_TEST_UTILS)

#include <gtest/gtest.h>

#include <cu/file-utils.hpp>
#include <cu/macro-utils.hpp>

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
        const std::filesystem::path& test_data_path,
        const std::filesystem::path& control_data_path,
        ConformanceTestFunctionsList<Unit, AdditionalArgs...> test_functions,
        AdditionalArgs... additional_args) {
        auto input_data = CU::load_data_from_file<Unit>(test_data_path);
        ASSERT_FALSE(input_data.empty()) << "Failed to load test data";

        auto output_data = CU::load_data_from_file<Unit>(control_data_path);
        ASSERT_FALSE(output_data.empty()) << "Failed to load control data";

        std::vector<Unit> result_data(output_data.size());

        for (auto func : test_functions) {
            func(input_data.data(), input_data.size(), result_data.data(), std::forward(additional_args)...);

            for (unsigned i = 0; i < output_data.size(); i++) {
                ASSERT_EQ(output_data[i], result_data[i]) << "Failed control check";
            }
        }
    }
}

#define CU_CONFORMANCE_TEST(name, test_data_path, test_file, control_file, /*test_functions*/...) \
    TEST(Conformance, name) { \
        std::filesystem::path test_path{ test_data_path }; \
        test_path.append(test_file); \
        std::filesystem::path control_path{test_data_path}; \
        control_path.append(control_file); \
        auto test_list = CU::make_conformance_list( { __VA_ARGS__ } ); \
        CU::run_conformance_test(test_path, control_path, test_list); \
    }

#define CU_CONFORMANCE_TEST_SIMD(name, test_data_path, test_file, control_file, function, /*simd sets*/...) \
    TEST(Conformance, name) { \
        std::filesystem::path test_path{ test_data_path }; \
        test_path.append(test_file); \
        std::filesystem::path control_path{test_data_path}; \
        control_path.append(control_file); \
        auto test_list = CU::make_conformance_list( { CU_CONCAT_FOR_EACH(function, __VA_ARGS__) } ); \
        CU::run_conformance_test(test_path, control_path, test_list); \
    }

#endif
