// Copyright (c) 2024-2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <concepts>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>

namespace CU {
    template <typename OptionType>
    concept StreamInsertable = requires(std::ostream & os, OptionType value) {
        { os << value } -> std::same_as<std::ostream&>;
    };

    // validator without any limitations
    template <StreamInsertable OptionType>
    class BaseValidator {
    public:
        virtual bool CheckValue(const OptionType& value) const { return true; }
        virtual std::string GetDescription(std::string prefix = "") const {
            return "there are no restrictions applied to the value";
        }
    };

    // validator that checks if a value is within the range [min, max]
    template <std::three_way_comparable OptionType>
    class RangeValidator final :
        public BaseValidator<OptionType> {
    public:
        RangeValidator(
            const OptionType& min,
            const OptionType& max) :
            m_min(min),
            m_max(max) {
            assert(min < max);
        }

        bool CheckValue(const OptionType& value) const override {
            return (m_min <= value) && (value <= m_max);
        }
        std::string GetDescription(std::string prefix = "") const override {
            std::stringstream result;
            result << "value must be within the range [" << m_min << ", " << m_max << "]";
            return result.str();
        }
    private:
        const OptionType m_min;
        const OptionType m_max;
    };

    // validator that checks if a value is present in a predefined list
    template <std::equality_comparable OptionType>
    class ListValidator final :
        public BaseValidator<OptionType> {
    public:
        ListValidator(std::initializer_list<OptionType> valid_options) :
            m_valid_options(valid_options) {
        }

        bool CheckValue(const OptionType& value) const override {
            return m_valid_options.end() !=
                std::find(m_valid_options.begin(), m_valid_options.end(), value);
        }
        std::string GetDescription(std::string prefix = "") const override {
            std::stringstream result;
            result << "value must match one of the following:";
            for (auto value : m_valid_options) {
                result << "\n" << prefix << " - '" << value << "';";
            }
            return result.str();
        }
    private:
        const std::list<OptionType> m_valid_options;
    };

#ifdef CUSTOM_VALIDATORS
    // The user can define their own validator class analogously in the CUSTOM_VALIDATORS macro, and its code will be inserted here.
    CUSTOM_VALIDATORS
#endif // CUSTOM_VALIDATORS

    template <typename OptionType>
    bool validate_option(const OptionType& value, const BaseValidator<OptionType>& validator) {
        return validator.CheckValue(value);
    }
}