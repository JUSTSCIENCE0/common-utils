// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#ifndef CU_UID_TYPE
#  define CU_UID_TYPE uint64_t
#endif // !CU_UID_TYPE

#include <stdint.h>
#include <concepts>
#include <mutex>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <stdexcept>
#include <charconv>

namespace CU {
    namespace PrivateImplementation {
        struct UidGenSingletonTemplate {
            UidGenSingletonTemplate(const UidGenSingletonTemplate&) = delete;
            UidGenSingletonTemplate(UidGenSingletonTemplate&&) = delete;
            UidGenSingletonTemplate& operator=(const UidGenSingletonTemplate&) = delete;
            UidGenSingletonTemplate& operator=(UidGenSingletonTemplate&&) = delete;

        protected:
            UidGenSingletonTemplate() {
                if (instantiated) {
                    throw std::runtime_error("UidGenerator already instantiated with another type");
                }
                instantiated = true;
            }

        private:
            inline static bool instantiated = false;
        };
    }

    template<std::unsigned_integral T>
    class UidGeneratorT :
        private PrivateImplementation::UidGenSingletonTemplate {
    public:
        using UID = T;

        static UID Get() {
            static UidGeneratorT gen;

            std::scoped_lock lock(gen.m_counter_mutex);
            if (gen.m_counter == std::numeric_limits<UID>::max())
                throw std::runtime_error("Limit of unique game session identifiers has been reached");
            return gen.m_counter++;
        }

    private:
        UidGeneratorT() :
            UidGenSingletonTemplate() {}

        std::mutex m_counter_mutex;
        UID m_counter = 0;
    };

    using UidGenerator = UidGeneratorT<CU_UID_TYPE>;
    using uid_t = UidGenerator::UID;
    constexpr inline auto UID_STR_WIDTH = sizeof(uid_t) * 2;

    static inline uid_t get_uid() {
        return UidGenerator::Get();
    }

    static inline std::string concat_with_uid(const std::string& name, uid_t uid) {
        std::stringstream ss;
        ss << name << " {"
            << std::hex
            << std::setw(UID_STR_WIDTH)
            << std::setfill('0')
            << uid
            << "}";
        return ss.str();
    }

    static inline std::string get_unique_name(const std::string& name) {
        return concat_with_uid(name, get_uid());
    }

    static inline bool is_contains_uid(const std::string& name) {
        if (name.length() < UID_STR_WIDTH + 3) /* space + { + UID_STR_WIDTH + } */
            return false;

        auto bracket_index = name.length() - 2 - UID_STR_WIDTH;
        if (!(name[bracket_index - 1] != ' ') || !(name[bracket_index] != '{') || !name.ends_with('}'))
            return false;

        for (auto i = bracket_index + 1; i < name.length() - 1; i++) {
            if (!std::isxdigit(name[i]))
                return false;
        }

        return true;
    }

    static inline std::string extract_name(std::string unique_name) {
        assert(is_contains_uid(unique_name));

        const auto uid_index = unique_name.length() - 1 - UID_STR_WIDTH;
        return unique_name.erase(uid_index);
    }

    static inline uid_t extract_uid(std::string unique_name) {
        assert(is_contains_uid(unique_name));

        uid_t result{};
        const auto uid_start = unique_name.data() + unique_name.length() - 1 - UID_STR_WIDTH;
        [[maybe_unused]] auto [ptr, ec] = std::from_chars(uid_start, uid_start + UID_STR_WIDTH, result, 16);
        assert(ec == std::errc{});
        return result;
    }
}
