// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#if defined(ENABLE_CU_TEST_UTILS)

#  ifndef ENABLE_CU_PROFILE
#error "Profile utils is required for tests"
#  endif

#include <gtest/gtest.h>

#include <cu/file-utils.hpp>
#include <cu/profile-utils.hpp>
#include <cu/math-utils.hpp>

#include <vector>
#include <functional>
#include <initializer_list>

namespace CU {
    template <typename Unit, typename... AdditionalArgs>
        requires std::is_fundamental_v<Unit>
    using TestFunction = std::function<void(const Unit*, int64_t, Unit*, AdditionalArgs...)>;

    template <typename Unit, typename... AdditionalArgs>
    TestFunction<Unit, AdditionalArgs...> make_test_function(
        void(*f)(const Unit*, int64_t, Unit*, AdditionalArgs...)) {
        return f;
    }

    template <typename Unit, typename... AdditionalArgs>
    using TestFunctionsList = std::vector<TestFunction<Unit, AdditionalArgs...>>;
    using TestFunctionsNames = std::vector<std::string>;

    template <typename Unit, typename... AdditionalArgs>
    TestFunctionsList<Unit, AdditionalArgs...> make_test_functions_list(
        std::initializer_list<void(*)(const Unit*, int64_t, Unit*, AdditionalArgs...)> functions
    ) {
        TestFunctionsList<Unit, AdditionalArgs...> result;
        for (auto func : functions) {
            result.push_back(make_test_function(func));
        }
        return result;
    }

    template <typename Unit, typename... AdditionalArgs>
    TestFunctionsList<Unit, AdditionalArgs...> make_test_functions_list(
        std::initializer_list<TestFunction<Unit, AdditionalArgs...>> functions
    ) {
        return TestFunctionsList<Unit, AdditionalArgs...>(functions);
    }

    template <typename Unit, typename... AdditionalArgs>
        requires std::is_fundamental_v<Unit>
    void run_conformance_test(
            const std::filesystem::path& test_data_path,
            const std::filesystem::path& control_data_path,
            TestFunctionsList<Unit, AdditionalArgs...> test_functions,
            AdditionalArgs... additional_args) {
        auto input_data = CU::load_data_from_file<Unit>(test_data_path);
        ASSERT_FALSE(input_data.empty()) << "Failed to load test data";

        auto output_data = CU::load_data_from_file<Unit>(control_data_path);
        ASSERT_FALSE(output_data.empty()) << "Failed to load control data";

        std::vector<Unit> result_data(output_data.size());

        for (auto func : test_functions) {
            func(input_data.data(), input_data.size(), result_data.data(), std::forward(additional_args)...);

            for (unsigned i = 0; i < output_data.size(); i++) {
                if constexpr (std::is_same_v<Unit, float>) {
#ifdef CU_PATCH_CONTROL_DATA
                    if (!testing::internal::CmpHelperFloatingPointEQ("output_data[i]", "result_data[i]", output_data[i], result_data[i])) {
                        if (CU::is_equal(output_data[i], result_data[i], 1.0e-1f, 1.0e-1f)) {
                            static int counter = 0;
                            std::cout << ++counter << ": #" << i << " " << output_data[i] << "!=" << result_data[i] << std::endl;
                            output_data[i] = result_data[i];
                        }
                    }
#endif

                    ASSERT_FLOAT_EQ(output_data[i], result_data[i]) << "Failed control check";
                }
                else if constexpr (std::is_same_v<Unit, double>) {
                    ASSERT_DOUBLE_EQ(output_data[i], result_data[i]) << "Failed control check";
                }
                else
                {
                    ASSERT_EQ(output_data[i], result_data[i]) << "Failed control check";
                }
            }
        }

#ifdef CU_PATCH_CONTROL_DATA
        CU::save_data_to_file(control_data_path, output_data);
#endif
    }

    template <size_t repeats_count = 10u,
              size_t result_size_scale_num = 1u,
              size_t result_size_scale_den = 1u,
              bool   strong_less = false,
              typename Unit, typename... AdditionalArgs>
        requires std::is_fundamental_v<Unit>
    void run_performance_test(
            const std::filesystem::path& test_data_path,
            TestFunctionsList<Unit, AdditionalArgs...> test_functions,
            TestFunctionsNames test_functions_names,
            AdditionalArgs... additional_args) {
        ASSERT_EQ(test_functions.size(), test_functions_names.size()) << 
            "The number of functions and their names must match";

        auto input_data = CU::load_data_from_file<Unit>(test_data_path);
        ASSERT_FALSE(input_data.empty()) << "Failed to load test data";

        size_t result_size = input_data.size() * result_size_scale_num / result_size_scale_den;
        std::vector<float> result_data(result_size);

        for (size_t index = 0; index < test_functions.size(); index++) {
            for (size_t i = 0; i < repeats_count; i++) {
                CU_PROFILE_CHECKBLOCK(performance, test_functions_names[index]);
                test_functions[index](input_data.data(), input_data.size(), result_data.data());
            }
        }

        auto results = CU_PROFILE_GET_RESULTS(test_functions_names);
        ASSERT_NE(results.find(test_functions_names[0]), results.end()) << "Profiler implementation error";
        auto prev_avg_ns = results[test_functions_names[0]].GetAvgNS();

        for (size_t index = 1; index < test_functions.size(); index++) {
            ASSERT_NE(results.find(test_functions_names[index]), results.end()) << "Profiler implementation error";

            const auto current_avg_ns = results[test_functions_names[index]].GetAvgNS();

            if (current_avg_ns >= prev_avg_ns) {
                EXPECT_FALSE(strong_less) << "Subsequent implementation is not faster than the previous one";

                // check if results almost equal
                auto epsilon_ns = prev_avg_ns / 2;
                auto delta_ns = current_avg_ns - prev_avg_ns;
                EXPECT_LE(delta_ns, epsilon_ns) << "Subsequent implementation is significantly slower than the previous one";
            }

            prev_avg_ns = current_avg_ns;
        }

#if defined(CU_PRINT_PERFORMANCE_TEST_RESULT)
        for (const auto& func_name : test_functions_names) {
            std::cout << func_name << ":" << std::endl;
            std::cout << results[func_name] << std::endl;
        }
#endif
    }
}

#define CU_CONFORMANCE_TEST(name, test_data_path, test_file, control_file, test_functions, /* additional args*/...) \
    TEST(Conformance, name) { \
        std::filesystem::path test_path{ test_data_path }; \
        test_path.append(test_file); \
        std::filesystem::path control_path{test_data_path}; \
        control_path.append(control_file); \
        auto test_list = CU::make_test_functions_list( { CU_REMOVE_PARENS test_functions } ); \
        CU::run_conformance_test(test_path, control_path, test_list __VA_OPT__(,) __VA_ARGS__); \
    }

#define CU_CONFORMANCE_TEST_SIMD(name, test_data_path, test_file, control_file, function, simd_sets, /* additional args*/...) \
    CU_CONFORMANCE_TEST(name, test_data_path, test_file, control_file, \
        ( CU_CONCAT_FOR_EACH(function, CU_REMOVE_PARENS simd_sets) ) __VA_OPT__(,) __VA_ARGS__ )

#define CU_PERFORMANCE_TEST_CONFIGURABLE(name, test_data_path, test_file, result_size_scale_num, result_size_scale_den, \
            repeats_count, strong_less, test_functions, /* additional args*/...) \
    TEST(Performance, name) { \
        std::filesystem::path test_path{ test_data_path }; \
        test_path.append(test_file); \
        auto test_list = CU::make_test_functions_list( { CU_REMOVE_PARENS test_functions } ); \
        CU::TestFunctionsNames test_names = { CU_FOR_EACH(CU_STR_COMMA, CU_REMOVE_PARENS test_functions) }; \
        CU::run_performance_test<repeats_count, result_size_scale_num, result_size_scale_den, strong_less> \
                (test_path, test_list, test_names __VA_OPT__(,) __VA_ARGS__ );\
    }

#define CU_PERFORMANCE_TEST(name, test_data_path, test_file, test_functions, /* additional args*/...) \
    CU_PERFORMANCE_TEST_CONFIGURABLE(name, test_data_path, test_file, 1, 1, 10, false, test_functions, ##__VA_ARGS__ )

#define CU_PERFORMANCE_TEST_STRONG(name, test_data_path, test_file, test_functions, /* additional args*/...) \
    CU_PERFORMANCE_TEST_CONFIGURABLE(name, test_data_path, test_file, 1, 1, 10, true, test_functions, ##__VA_ARGS__ )

#define CU_PERFORMANCE_TEST_SIMD_CONFIGURABLE(name, test_data_path, test_file, result_size_scale_num, result_size_scale_den, \
            repeats_count, strong_less, function, simd_sets, /* additional args*/...) \
    CU_PERFORMANCE_TEST_CONFIGURABLE(name, test_data_path, test_file, result_size_scale_num, result_size_scale_den, \
        repeats_count, strong_less, ( CU_CONCAT_FOR_EACH(function, CU_REMOVE_PARENS simd_sets) ) __VA_OPT__(,) __VA_ARGS__ )

#define CU_PERFORMANCE_TEST_SIMD(name, test_data_path, test_file, function, simd_sets, /* additional args*/...) \
    CU_PERFORMANCE_TEST_SIMD_CONFIGURABLE(name, test_data_path, test_file, 1, 1, 10, false, function, simd_sets __VA_OPT__(,) __VA_ARGS__ )

#define CU_PERFORMANCE_TEST_SIMD_STRONG(name, test_data_path, test_file, function, simd_sets, /* additional args*/...) \
    CU_PERFORMANCE_TEST_SIMD_CONFIGURABLE(name, test_data_path, test_file, 1, 1, 10, true, function, simd_sets __VA_OPT__(,) __VA_ARGS__ )

#endif

