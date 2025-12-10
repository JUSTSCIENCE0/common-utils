// Copyright (c) 2025, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <cu/id-utils.hpp>

#include <gtest/gtest.h>

#include <array>

TEST(IdPoolTest, BaseFunctional) {
    CU::IdPool id_pool{};
    ASSERT_EQ(id_pool.LockId(), 0);
    ASSERT_EQ(id_pool.LockId(), 1);
    ASSERT_EQ(id_pool.LockId(), 2);
    ASSERT_EQ(id_pool.LockId(), 3);

    id_pool.FreeId(2);
    ASSERT_EQ(id_pool.LockId(), 2);

    id_pool.FreeId(2);
    id_pool.FreeId(1);
    ASSERT_EQ(id_pool.LockId(), 1);

    id_pool.FreeId(1);
    id_pool.FreeId(3);
    ASSERT_EQ(id_pool.LockId(), 1);
    ASSERT_EQ(id_pool.LockId(), 2);
    ASSERT_EQ(id_pool.LockId(), 3);

    id_pool.FreeId(3);
    ASSERT_EQ(id_pool.LockId(), 3);

    id_pool.FreeId(3);
    id_pool.FreeId(2);
    ASSERT_EQ(id_pool.LockId(), 2);
}

TEST(UidGeneratorTest, UniqueImpl) {
    // first instantiation is ok
    EXPECT_NO_THROW(CU::UidGeneratorT<uint32_t>::Get());

    // subsequent instantiations throw an exception
    EXPECT_THROW(CU::UidGeneratorT<uint64_t>::Get(), std::runtime_error);
    EXPECT_THROW(CU::UidGeneratorT<uint16_t>::Get(), std::runtime_error);
    EXPECT_THROW(CU::UidGeneratorT<uint8_t>::Get(),  std::runtime_error);

    // can access already instantiated
    EXPECT_NO_THROW(CU::UidGeneratorT<uint32_t>::Get());
}

TEST(UidGeneratorTest, BaseFunctional) {
    constexpr size_t vals_count = 10;
    std::array<CU::uid_t, vals_count> vals = {};
    for (auto& val : vals) {
        val = CU::UidGeneratorT<uint32_t>::Get();
    }

    for (size_t i = 0; i < vals_count; i++) {
        for (size_t j = i + 1; j < vals_count; j++) {
            ASSERT_NE(vals[i], vals[j]);
        }
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}