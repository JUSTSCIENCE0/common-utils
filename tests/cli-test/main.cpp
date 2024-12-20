// Copyright (c) 2024, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#define CLI_CONFIGURATION \
    CLI_FLAG(test-flag1, SYMBOL(f), test_flag1, "first test flag")  \
    CLI_FLAG(test-flag2, WO_SYMBOL, test_flag2, "second test flag") \
    CLI_VALUABLE_FLAG(val-flag, WO_SYMBOL, val_flag, "test flag with optional value", int, 10, BaseValidator) \
    CLI_OPTIONAL_PROPERTY(optional-prop, SYMBOL(o), optional_prop, "test optional property", \
        float, 0.0f, RangeValidator, -1.0f, 1.0f ) \
    CLI_REQUIRED_PROPERTY(required-prop, SYMBOL(r), required_prop, "test required property", \
        std::string, ListValidator, "123", "abc", "qwe" )
#include "cli-utils.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <utility>

struct ArgsSet {
    std::vector<char*> args;
    bool is_valid = false;
    CU::CLIConfig reference{};
};

class ArgsTest :
    public testing::TestWithParam<ArgsSet> {
protected:
    void TestParse() {
        ArgsSet args_set = GetParam();

        CU::CLIConfig cli_config{};
        ASSERT_EQ(
            args_set.is_valid,
            CU::parse_cli_args(
                static_cast<int>(args_set.args.size()),
                args_set.args.data(),
                &cli_config
            )
        );

        if (!args_set.is_valid) return;

        ASSERT_EQ(cli_config, args_set.reference);
    }
};

const ArgsSet kTestCases[] = {
    // show help, using full name
    {
        // arguments
        {
            const_cast<char*>("C://test/app.exe"),
            const_cast<char*>("--help"),
        },
        // not parsable
        false, {}
    },
    // show help, using short name
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("-h"),
        },
        // not parsable
        false, {}
    },
    // missed required property
    {
        // arguments
        {
            const_cast<char*>("//test/app")
        },
    // not parsable
    false, {}
},
// required property has invalid value
{
    // arguments
    {
        const_cast<char*>("//test/app"),
        const_cast<char*>("--required-prop=456"),
    },
    // not parsable
    false, {}
    },
    // optional property has invalid value
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("--required-prop=123"),
            const_cast<char*>("--optional-prop=2.0"),
        },
        // not parsable
        false, {}
    },
    // valid properties, full names
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("--required-prop=123"),
            const_cast<char*>("--optional-prop=0.5"),
        },
        true,
        {
            .test_flag1 = false,
            .test_flag2 = false,
            .has_val_flag = false,
            .val_flag = 10,
            .optional_prop = 0.5f,
            .required_prop = "123"
        }
    },
    // valid properties, short names
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("-r abc"),
            const_cast<char*>("-o -0.5"),
        },
        true,
        {
            .test_flag1 = false,
            .test_flag2 = false,
            .has_val_flag = false,
            .val_flag = 10,
            .optional_prop = -0.5f,
            .required_prop = "abc"
        }
    },
    // valid properties and flags, mixed names
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("-r abc"),
            const_cast<char*>("-o -0.5"),
            const_cast<char*>("-f"),
            const_cast<char*>("--test-flag2"),
            const_cast<char*>("--val-flag"),
        },
        true,
        {
            .test_flag1 = true,
            .test_flag2 = true,
            .has_val_flag = true,
            .val_flag = 10,
            .optional_prop = -0.5f,
            .required_prop = "abc"
        }
    },
    // valid properties and flags (with value), mixed names
    {
        // arguments
        {
            const_cast<char*>("//test/app"),
            const_cast<char*>("-r abc"),
            const_cast<char*>("-o -0.5"),
            const_cast<char*>("-f"),
            const_cast<char*>("--test-flag2"),
            const_cast<char*>("--val-flag=75"),
        },
        true,
        {
            .test_flag1 = true,
            .test_flag2 = true,
            .has_val_flag = true,
            .val_flag = 75,
            .optional_prop = -0.5f,
            .required_prop = "abc"
        }
    },
};

TEST_P(ArgsTest, FullTest) {
    TestParse();
}

INSTANTIATE_TEST_SUITE_P(
    ArgsSets, ArgsTest,
    testing::ValuesIn(kTestCases)
);

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}