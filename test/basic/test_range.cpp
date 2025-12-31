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

TEST_F(RangeTest, TransformPipelineBasic) {
    const auto transformed = arr | transform([](const int x) { return x * x; });

    auto it = transformed.begin();
    const auto end = transformed.end();

    EXPECT_EQ(*it, 1);   ++it;
    EXPECT_EQ(*it, 4);   ++it;
    EXPECT_EQ(*it, 9);   ++it;
    EXPECT_EQ(*it, 16);  ++it;
    EXPECT_EQ(*it, 25);  ++it;
    EXPECT_EQ(it, end);

    int sum = 0;
    for (const auto& e : arr | transform([](const int x) { return x * x; })) {
        sum += e;
    }
    EXPECT_EQ(sum, 55);  // 1+4+9+16+25
}

TEST_F(RangeTest, TransformPipelineWithCapture) {
    int offset = 10;
    const auto transformed = arr | transform([offset](const int x) { return x + offset; });

    int expected = 11;
    for (const auto& e : transformed) {
        EXPECT_EQ(e, expected++);
    }
}

TEST_F(RangeTest, TransformChainedWithTakeAndSkip) {
    const auto view = arr
                    | skip(1_size)
                    | take(3_size)
                    | transform([](const int x) { return -x; });

    auto it = view.begin();
    EXPECT_EQ(*it, -2); ++it;
    EXPECT_EQ(*it, -3); ++it;
    EXPECT_EQ(*it, -4); ++it;
    EXPECT_EQ(it, view.end());
}

TEST_F(RangeTest, TransformOnEmptyRange) {
    Array<int, 0> empty;
    const auto transformed = empty | transform([](const int x) { return x * 2; });
    EXPECT_EQ(transformed.begin(), transformed.end());
}

TEST_F(RangeTest, TransformConstPropagation) {
    const auto& cref = arr;
    const auto transformed = cref | transform([](const int x) { return x + 1; });

    auto it = transformed.begin();
    EXPECT_EQ(*it, 2); ++it;
    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(*it, 4); ++it;
    EXPECT_EQ(*it, 5);

    int val = 2;
    for (auto&& e: cref | transform([](const int x) { return x + 1; }))
    {
        EXPECT_EQ(val, e);
        ++val;
    }
}

TEST_F(RangeTest, FilterPipelineBasic) {
    const auto filtered = arr | filter([](const int x) { return x % 2 == 1; });

    auto it = filtered.begin();
    const auto end = filtered.end();

    EXPECT_EQ(*it, 1); ++it;
    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(*it, 5); ++it;
    EXPECT_EQ(it, end);

    int count = 0;
    for (const auto& e : arr | filter([](const int x) { return x > 2; })) {
        EXPECT_GT(e, 2);
        ++count;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(RangeTest, FilterPipelineWithCapture) {
    int threshold = 3;
    const auto filtered = arr | filter([threshold](const int x) { return x >= threshold; });

    int i = 0;
    for (const auto& e : filtered)
    {
        constexpr int expected[] = {3, 4, 5};
        EXPECT_EQ(e, expected[i++]);
    }
    EXPECT_EQ(i, 3);
}

TEST_F(RangeTest, FilterChainedWithOtherAdapters) {
    const auto view = arr
                    | take(4_size)
                    | filter([](const int x) { return x % 2 == 0; })
                    | transform([](const int x) { return x * 10; });

    auto it = view.begin();
    EXPECT_EQ(*it, 20); ++it;  // 2 * 10
    EXPECT_EQ(*it, 40); ++it;  // 4 * 10
    EXPECT_EQ(it, view.end());
}

TEST_F(RangeTest, FilterOnEmptyRange) {
    Array<int, 0> empty;
    const auto filtered = empty | filter([](const int) { return true; });
    EXPECT_EQ(filtered.begin(), filtered.end());
}

TEST_F(RangeTest, FilterAllFalseIsEmpty) {
    const auto filtered = arr | filter([](const int) { return false; });
    EXPECT_EQ(filtered.begin(), filtered.end());
}

TEST_F(RangeTest, ExcludePipelineBasic)
{
    const auto excluded = arr | exclude([](const int x) { return x % 2 == 1; });
    auto it = excluded.begin();
    const auto end = excluded.end();

    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(it, end);

    int count = 0;
    for (const auto& e : arr | exclude([](const int x) { return x <= 2; }))
    {
        EXPECT_GT(e, 2);
        ++count;
    }
    EXPECT_EQ(count, 3);  // 3,4,5
}

namespace
{
    class Functor
    {
        int threshold_;
    public:
        explicit Functor(const int threshold) : threshold_(threshold) {} // NOLINT

        bool operator()(const int x) const { return x < this->threshold_; }
    };
}

TEST_F(RangeTest, ExcludePipelineWithCapture)
{
    constexpr int threshold = 3;
    const auto excluded = arr | exclude(Functor{threshold});
    int i = 0;
    for (const auto& e : excluded)
    {
        constexpr int expected[] = {3, 4, 5};
        EXPECT_EQ(e, expected[i++]);
    }
    EXPECT_EQ(i, 3);
}

TEST_F(RangeTest, ExcludeChainedWithOtherAdapters)
{
    const auto view = arr | take(4_size) | exclude([](const int x) { return x % 2 == 0; })
                                     | transform([](const int x) { return x * 10; });
    auto it = view.begin();

    EXPECT_EQ(*it, 10);  // 1 * 10
    ++it;
    EXPECT_EQ(*it, 30);  // 3 * 10
    ++it;
    EXPECT_EQ(it, view.end());
}

TEST_F(RangeTest, ExcludeOnEmptyRange)
{
    Array<int, 0> empty;
    const auto excluded = empty | exclude([](const int) { return true; });
    EXPECT_EQ(excluded.begin(), excluded.end());
}

TEST_F(RangeTest, ExcludeAllTrueIsEmpty)
{
    const auto excluded = arr | exclude([](const int) { return true; });
    EXPECT_EQ(excluded.begin(), excluded.end());
}