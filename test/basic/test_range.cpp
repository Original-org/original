#include <gtest/gtest.h>

import original.basic.array;
import original.basic.range;
import original.basic.number;

using namespace original;
using namespace original::range;
using namespace original::literals;

class RangeTest : public testing::Test {
protected:
    Array<int, 5> arr {1, 2, 3, 4, 5};
};

TEST_F(RangeTest, ArraySatisfiesRangeConcept) {
    static_assert(Range<decltype(arr)>);
    EXPECT_TRUE(Range<decltype(arr)>);
    static_assert(IterRange<decltype(arr)>);
}

TEST_F(RangeTest, TakeRangeSatisfiesIterRange) {
    using TakenType = decltype(arr | take(3_size));
    static_assert(IterRange<TakenType>);
    EXPECT_TRUE(IterRange<TakenType>);
}

TEST_F(RangeTest, SkipRangeSatisfiesIterRange) {
    using SkippedType = decltype(arr | skip(2_size));
    static_assert(IterRange<SkippedType>);
    EXPECT_TRUE(IterRange<SkippedType>);
}

TEST_F(RangeTest, EnumRangeSatisfiesIterRange) {
    using EnumeratedType = decltype(arr | enumerate());
    static_assert(IterRange<EnumeratedType>);
    EXPECT_TRUE(IterRange<EnumeratedType>);
}

TEST_F(RangeTest, RangeTraitsExtraction) {
    using ArrType = decltype(arr);
    static_assert(Range<ArrType>);
    static_assert(IterRange<ArrType>);
}

TEST_F(RangeTest, TakePipeline) {
    const auto taken = arr | take(3_size);
    auto it = taken.begin();
    const auto end = taken.end();

    EXPECT_EQ(*it, 1); ++it;
    EXPECT_EQ(*it, 2); ++it;
    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(it, end);

    int sum = 0;
    int cnt = 0;
    for (const auto& e: arr | take(3_size))
    {
        sum += e;
        cnt += 1;
    }
    EXPECT_EQ(sum, 6);
    EXPECT_EQ(cnt, 3);
}

TEST_F(RangeTest, SkipPipeline) {
    const auto skipped = arr | skip(2_size);
    auto it = skipped.begin();
    const auto end = skipped.end();

    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(*it, 4); ++it;
    EXPECT_EQ(*it, 5); ++it;
    EXPECT_EQ(it, end);

    int sum = 0;
    int cnt = 0;
    for (const auto& e: arr | skip(2_size))
    {
        sum += e;
        cnt += 1;
    }
    EXPECT_EQ(sum, 12);
    EXPECT_EQ(cnt, 3);
}

TEST_F(RangeTest, EnumeratePipelineDefaultStart) {
    const auto enumerated = arr | enumerate();
    auto it = enumerated.begin();
    const auto end = enumerated.end();

    auto [i0, v0] = *it; EXPECT_EQ(i0, 0_size); EXPECT_EQ(v0, 1); ++it;
    auto [i1, v1] = *it; EXPECT_EQ(i1, 1_size); EXPECT_EQ(v1, 2); ++it;
    auto [i2, v2] = *it; EXPECT_EQ(i2, 2_size); EXPECT_EQ(v2, 3); ++it;
    EXPECT_NE(it, end);

    std::pair p = {0_size, 1};
    for (auto&& [index, val]: arr | enumerate())
    {
        EXPECT_EQ(index, p.first);
        EXPECT_EQ(val, p.second);
        ++p.first;
        ++p.second;
    }
}

TEST_F(RangeTest, EnumeratePipelineCustomStart) {
    const auto enumerated = arr | enumerate(10_size);
    auto it = enumerated.begin();

    auto [i0, v0] = *it; EXPECT_EQ(i0, 10_size); EXPECT_EQ(v0, 1); ++it;
    auto [i1, v1] = *it; EXPECT_EQ(i1, 11_size); EXPECT_EQ(v1, 2); ++it;

    auto i = 10_size;
    for (const auto& cref = arr;
        auto&& [index, val]: cref | enumerate(10_size)) {
        EXPECT_EQ(i, index);
        ++i;
    }
}

TEST_F(RangeTest, TakeZeroIsEmpty) {
    const auto taken = arr | take(0_size);
    EXPECT_EQ(taken.begin(), taken.end());
}

TEST_F(RangeTest, SkipBeyondLengthIsEmpty) {
    const auto skipped = arr | skip(10_size);
    EXPECT_EQ(skipped.begin(), skipped.end());
}

TEST_F(RangeTest, ChainedPipeline) {
    std::pair p {1_size, 2};
    for (const auto view =
        arr | take(4_size) | skip(1_size) | enumerate(1_size);
        auto&& [index, val]: view)
    {
        EXPECT_EQ(index, p.first);
        EXPECT_EQ(val, p.second);
        ++p.first;
        ++p.second;
    }
}

TEST_F(RangeTest, EmptyArraySatisfiesRange) {
    Array<int, 0> empty;
    static_assert(Range<decltype(empty)>);
    EXPECT_TRUE(Range<decltype(empty)>);
    EXPECT_EQ(empty.begin(), empty.end());
}
