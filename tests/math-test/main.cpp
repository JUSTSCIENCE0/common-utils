// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <cu/math-utils.hpp>

#include <gtest/gtest.h>

#include <variant>

template <typename FloatT>
    requires std::is_floating_point_v<FloatT>
struct EqualityTestSet {
    FloatT lhs;
    FloatT rhs;
    FloatT absolute_epsilon;
    FloatT relative_epsilon;
    bool   result;
};

template <typename FloatT>
class EqualityTest :
    public testing::TestWithParam<EqualityTestSet<FloatT>> {
protected:
    void TestEquality(const EqualityTestSet<FloatT>& test_case) {
        ASSERT_EQ(
            test_case.result,
            CU::is_equal(test_case.lhs,
                         test_case.rhs,
                         test_case.absolute_epsilon,
                         test_case.relative_epsilon));
    }
};

const EqualityTestSet<float> kFloatCases[] = {
//    lhs     | rhs     | absolute_epsilon | relative_epsilon | result
    {  10.0f  ,  10.1f  , 1e-3f            , 1e-3f            , false }, // not equal
    {  10.0f  ,  10.01f , 1e-1f            , 1e-3f            , true  }, // equal by absolute_epsilon
    {  10.0f  ,  10.01f , 1e-3f            , 1e-3f            , true  }, // equal by relative_epsilon
    { -10.0f  , -10.1f  , 1e-3f            , 1e-3f            , false }, // negative not equal
    { -10.0f  , -10.01f , 1e-1f            , 1e-3f            , true  }, // negative equal by absolute_epsilon
    { -10.0f  , -10.01f , 1e-3f            , 1e-3f            , true  }, // negative equal by relative_epsilon
};

const EqualityTestSet<double> kDoubleCases[] = {
//    lhs     | rhs     | absolute_epsilon | relative_epsilon | result
    {  10.0   ,  10.1   , 1e-3             , 1e-3             , false }, // not equal
    {  10.0   ,  10.01  , 1e-1             , 1e-3             , true  }, // equal by absolute_epsilon
    {  10.0   ,  10.01  , 1e-3             , 1e-3             , true  }, // equal by relative_epsilon
    { -10.0   , -10.1   , 1e-3             , 1e-3             , false }, // negative not equal
    { -10.0   , -10.01  , 1e-1             , 1e-3             , true  }, // negative equal by absolute_epsilon
    { -10.0   , -10.01  , 1e-3             , 1e-3             , true  }, // negative equal by relative_epsilon
};

const EqualityTestSet<long double> kLongDoubleCases[] = {
//    lhs     | rhs     | absolute_epsilon | relative_epsilon | result
    {  10.0l  ,  10.1l  , 1e-3l            , 1e-3l            , false }, // not equal
    {  10.0l  ,  10.01l , 1e-1l            , 1e-3l            , true  }, // equal by absolute_epsilon
    {  10.0l  ,  10.01l , 1e-3l            , 1e-3l            , true  }, // equal by relative_epsilon
    { -10.0l  , -10.1l  , 1e-3l            , 1e-3l            , false }, // negative not equal
    { -10.0l  , -10.01l , 1e-1l            , 1e-3l            , true  }, // negative equal by absolute_epsilon
    { -10.0l  , -10.01l , 1e-3l            , 1e-3l            , true  }, // negative equal by relative_epsilon
};

using FloatEqualityTest = EqualityTest<float>;
using DoubleEqualityTest = EqualityTest<double>;
using LongDoubleEqualityTest = EqualityTest<long double>;

TEST_P(FloatEqualityTest, FloatTest) {
    TestEquality(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    FloatSets, FloatEqualityTest,
    testing::ValuesIn(kFloatCases)
);

TEST_P(DoubleEqualityTest, DoubleTest) {
    TestEquality(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    DoubleSets, DoubleEqualityTest,
    testing::ValuesIn(kDoubleCases)
);

TEST_P(LongDoubleEqualityTest, LongDoubleTest) {
    TestEquality(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    LongDoubleSets, LongDoubleEqualityTest,
    testing::ValuesIn(kLongDoubleCases)
);

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}