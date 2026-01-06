#include <algorithm>
#include <numeric>
#include <gtest/gtest.h>

import original.basic.array;
import original.basic.types;
import original.basic.range;
import original.basic.number;
import original.basic.iterator;
import original.basic.structural;

using namespace original;
using namespace original::range;
using namespace original::literals;

class RangeTest : public testing::Test {
protected:
    Array<int, 5> arr {1, 2, 3, 4, 5};
};

TEST_F(RangeTest, RangeConcept) {
    static_assert(Range<decltype(arr)>);
    EXPECT_TRUE(Range<decltype(arr)>);
    static_assert(IterRange<decltype(arr)>);
}

TEST_F(RangeTest, RangeInterfacesForBuiltinArray)
{
    constexpr int builtin[] = {1, 2, 3, 4, 5, 6}; // NOLINT
    static_assert(SameType<const int*, decltype(begin(builtin))>);
    static_assert(SameType<const int*, decltype(end(builtin))>);
    static_assert(Iterator<decltype(begin(builtin))>);
    static_assert(Iterator<decltype(end(builtin))>);
    static_assert(Range<decltype(builtin)>);
    static_assert(IterRange<decltype(builtin)>);
    static_assert(SameType<const int*, RangeTraits<decltype(builtin)>::BeginIterType>);
    static_assert(SameType<const int*, RangeTraits<decltype(builtin)>::EndIterType>);
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

    Couple p = {0_size, 1};
    for (const auto& [index, val]: arr | enumerate())
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
    class Closure
    {
        int threshold_;
    public:
        explicit Closure(const int threshold) : threshold_(threshold) {} // NOLINT

        bool operator()(const int x) const { return x < this->threshold_; }
    };
}

TEST_F(RangeTest, ExcludePipelineWithCapture)
{
    constexpr int threshold = 3;
    const auto excluded = arr | exclude(Closure{threshold});
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

TEST_F(RangeTest, ConcatPipelineBasic) {
    Array<int, 3> a{10, 20, 30};
    Array<int, 2> b{40, 50};

    const auto concatenated = arr | concat(a) | concat(b);

    auto it = concatenated.begin();
    const auto end = concatenated.end();

    EXPECT_EQ(*it, 1); ++it;
    EXPECT_EQ(*it, 2); ++it;
    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(*it, 4); ++it;
    EXPECT_EQ(*it, 5); ++it;
    EXPECT_EQ(*it, 10); ++it;
    EXPECT_EQ(*it, 20); ++it;
    EXPECT_EQ(*it, 30); ++it;
    EXPECT_EQ(*it, 40); ++it;
    EXPECT_EQ(*it, 50); ++it;
    EXPECT_EQ(it, end);

    int sum = 0;
    int count = 0;
    for (const auto& e : arr | concat(a) | concat(b)) {
        sum += e;
        ++count;
    }
    EXPECT_EQ(sum, 165);   // 1+2+3+4+5+10+20+30+40+50
    EXPECT_EQ(count, 10);
}

TEST_F(RangeTest, ConcatPipelineWithEmptyRange) {
    Array<int, 0> empty;

    {
        int count = 0;
        for (const auto& _ : arr | concat(empty)) {
            ++count;
        }
        EXPECT_EQ(count, 5);
    }

    {
        int count = 0;
        for (const auto& _ : empty | concat(arr)) {
            ++count;
        }
        EXPECT_EQ(count, 5);
    }

    {
        auto view = empty | concat(empty);
        EXPECT_EQ(view.begin(), view.end());
    }

    {
        int invoked = 0;
        for (const auto& _ : empty | concat(empty)) {
            ++invoked;  // 不应执行
        }
        EXPECT_EQ(invoked, 0);
    }
}

TEST_F(RangeTest, ConcatChainedWithOtherAdapters) {
    Array<int, 3> extra{100, 200, 300};

    const auto view = arr
                    | take(3_size)
                    | transform([](const int x) { return x * 10; })
                    | concat(extra | transform([](const int x) { return x / 10; }));

    auto it = view.begin();
    const auto end = view.end();

    EXPECT_EQ(*it, 10); ++it;
    EXPECT_EQ(*it, 20); ++it;
    EXPECT_EQ(*it, 30); ++it;
    EXPECT_EQ(*it, 10); ++it;
    EXPECT_EQ(*it, 20); ++it;
    EXPECT_EQ(*it, 30); ++it;
    EXPECT_EQ(it, end);

    int sum = 0;
    int count = 0;
    for (const auto& e : arr
                         | take(3_size)
                         | transform([](int x) { return x * 10; })
                         | concat(extra | transform([](int x) { return x / 10; }))) {
        sum += e;
        ++count;
    }
    EXPECT_EQ(sum, 120);  // 10+20+30 + 10+20+30
    EXPECT_EQ(count, 6);
}

TEST_F(RangeTest, ZipPipelineBasic) {
    Array<int, 5> other{10, 20, 30, 40, 50};

    const auto zipped = arr | zip(other);

    auto it = zipped.begin();
    const auto end = zipped.end();

    auto [l1, r1] = *it; EXPECT_EQ(l1, 1);  EXPECT_EQ(r1, 10); ++it;
    auto [l2, r2] = *it; EXPECT_EQ(l2, 2);  EXPECT_EQ(r2, 20); ++it;
    auto [l3, r3] = *it; EXPECT_EQ(l3, 3);  EXPECT_EQ(r3, 30); ++it;
    auto [l4, r4] = *it; EXPECT_EQ(l4, 4);  EXPECT_EQ(r4, 40); ++it;
    auto [l5, r5] = *it; EXPECT_EQ(l5, 5);  EXPECT_EQ(r5, 50); ++it;
    EXPECT_EQ(it, end);

    int i = 1;
    for (const auto& [left, right] : arr | zip(other)) {
        EXPECT_EQ(left, i);
        EXPECT_EQ(right, i * 10);
        ++i;
    }
    EXPECT_EQ(i, 6);
}

TEST_F(RangeTest, ZipPipelineShorterRight) {
    Array<int, 3> shorter{100, 200, 300};

    const auto zipped = arr | zip(shorter);

    auto it = zipped.begin();
    const auto end = zipped.end();

    auto [l1, r1] = *it; EXPECT_EQ(l1, 1);   EXPECT_EQ(r1, 100); ++it;
    auto [l2, r2] = *it; EXPECT_EQ(l2, 2);   EXPECT_EQ(r2, 200); ++it;
    auto [l3, r3] = *it; EXPECT_EQ(l3, 3);   EXPECT_EQ(r3, 300); ++it;
    EXPECT_EQ(it, end);

    int idx = 0;
    for (const auto& [l, r] : arr | zip(shorter)) {
        int expected_right[] = {100, 200, 300};
        int expected_left[] = {1, 2, 3};
        EXPECT_EQ(l, expected_left[idx]);
        EXPECT_EQ(r, expected_right[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 3);
}

TEST_F(RangeTest, ZipPipelineShorterLeft) {
    constexpr Array<int, 3> shorter{100, 200, 300};

    const auto zipped = shorter | zip(arr);

    auto it = zipped.begin();
    const auto end = zipped.end();

    auto [l1, r1] = *it; EXPECT_EQ(l1, 100); EXPECT_EQ(r1, 1); ++it;
    auto [l2, r2] = *it; EXPECT_EQ(l2, 200); EXPECT_EQ(r2, 2); ++it;
    auto [l3, r3] = *it; EXPECT_EQ(l3, 300); EXPECT_EQ(r3, 3); ++it;
    EXPECT_EQ(it, end);

    auto cnt = 0_size;
    Array<Couple<int, int>, 3> elements {Couple{100, 1}, Couple{200, 2}, Couple{300, 3}};
    for (const auto& [index, e]: shorter | zip(arr) | enumerate())
    {
        EXPECT_EQ(index, cnt);
        EXPECT_EQ(elements[cnt], e);
        ++cnt;
    }

    int idx = 0;
    for (const auto& cref = arr; auto&& [l, r] : shorter | zip(cref)) {
        constexpr int expected_right[] = {1, 2, 3};
        constexpr int expected_left[] = {100, 200, 300};
        EXPECT_EQ(l, expected_left[idx]);
        EXPECT_EQ(r, expected_right[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 3);
}

TEST_F(RangeTest, ZipChainedWithOtherAdapters) {
    Array<int, 5> other{10, 20, 30, 40, 50};

    const auto view = arr
                    | skip(1_size)
                    | take(3_size)
                    | zip(other | skip(1_size) | take(3_size));

    auto it = view.begin();
    const auto end = view.end();

    auto [l1, r1] = *it; EXPECT_EQ(l1, 2); EXPECT_EQ(r1, 20); ++it;
    auto [l2, r2] = *it; EXPECT_EQ(l2, 3); EXPECT_EQ(r2, 30); ++it;
    auto [l3, r3] = *it; EXPECT_EQ(l3, 4); EXPECT_EQ(r3, 40); ++it;
    EXPECT_EQ(it, end);

    int idx = 0;
    for (const auto& [l, r] : arr
                             | skip(1_size)
                             | take(3_size)
                             | zip(other | skip(1_size) | take(3_size))) {
        int expected_left[] = {2, 3, 4};
        int expected_right[] = {20, 30, 40};
        EXPECT_EQ(l, expected_left[idx]);
        EXPECT_EQ(r, expected_right[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 3);
}

TEST_F(RangeTest, ZipWithEmptyRange) {
    Array<int, 0> empty;

    auto view1 = arr | zip(empty);
    EXPECT_EQ(view1.begin(), view1.end());
    auto view2 = empty | zip(arr);
    EXPECT_EQ(view2.begin(), view2.end());

    {
        int invoked = 0;
        for (const auto& _ : arr | zip(empty)) {
            ++invoked;
        }
        EXPECT_EQ(invoked, 0);
    }
    {
        int invoked = 0;
        for (const auto& _ : empty | zip(arr)) {
            ++invoked;
        }
        EXPECT_EQ(invoked, 0);
    }
}

TEST_F(RangeTest, ReverseRange) {
    constexpr Array<int, 5> arr {1, 2, 3, 4, 5};
    const auto reversed = arr | reverse();
    int cur = 5;
    for (const auto& e : reversed)
    {
        EXPECT_EQ(e, cur);
        --cur;
    }

    for (const auto reverse_enum = arr | reverse() | enumerate();
         const auto& [index, val]: reverse_enum)
    {
        EXPECT_EQ(index.value() + val, 5);
    }

    static constexpr int builtin[5] = {1, 2, 3, 4, 5};
    int i = 5;
    for (const auto reversed_builtin = builtin | reverse();
         const auto& e: reversed_builtin)
    {
        EXPECT_EQ(e, i);
        --i;
    }
}

TEST_F(RangeTest, CallReverseMultipleTimes)
{
    static constexpr int builtin[5] = {1, 2, 3, 4, 5};
    for (const auto chain_odd_times = builtin | reverse() | reverse() | reverse() | enumerate();
         const auto& [index, val]: chain_odd_times)
    {
        EXPECT_EQ(index.value() + val, 5);
    }

    for (const auto chain_even_times = builtin | reverse() | reverse() | reverse() | reverse() | enumerate();
         const auto& [index, val]: chain_even_times)
    {
        EXPECT_EQ(index.value(), val - 1);
    }
}

TEST_F(RangeTest, BuiltinRangeViewChain)
{
    static constexpr int builtin[5] = {1, 2, 3, 4, 5};
    const auto chain1 = builtin | take(10_size);
    int i = 1;
    for (const auto& e: chain1)
    {
        EXPECT_EQ(i, e);
        ++i;
    }

    const auto chain2 = builtin | take(6_size) | enumerate();
    int j = 1;
    for (const auto& [index, val]: chain2)
    {
        EXPECT_EQ(index.value(), j - 1);
        EXPECT_EQ(val, j);
        ++j;
    }

    const auto chain3 = builtin | take(5_size) | enumerate() | skip(1_size);
    int cnt = 0;
    int k = 2;
    for (const auto& [index, val]: chain3)
    {
        EXPECT_EQ(val, k);
        ++k;
        ++cnt;
    }
    EXPECT_EQ(cnt, 4);

    const auto chain4 = builtin
        | reverse()
        | enumerate()
        | skip(1_size)
        | take(3_size);
    for (const auto& [index, val]: chain4)
    {
        EXPECT_EQ(index.value() + val, 5);
        EXPECT_NE(val, 0);
        EXPECT_NE(val, 5);
    }
}

TEST_F(RangeTest, ToStdBasic) {
    const auto std_range = arr | toStd();
    static_assert(std::random_access_iterator<decltype(std_range.begin())>);
    EXPECT_TRUE((std::same_as<std::contiguous_iterator_tag, decltype(std_range.begin())::iterator_category>));

    int sum = 0;
    std::ranges::for_each(std_range, [&sum](const int x) { sum += x; });
    EXPECT_EQ(sum, 15);  // 1+2+3+4+5

    const auto dist = std::distance(std_range.begin(), std_range.end());
    EXPECT_EQ(dist, 5);

    const auto accum = std::accumulate(std_range.begin(), std_range.end(), 0);
    EXPECT_EQ(accum, 15);

    // Test random access operations
    auto it = std_range.begin();
    EXPECT_EQ(it[0], 1);
    EXPECT_EQ(it[2], 3);
    EXPECT_EQ(it[4], 5);
    it += 3;
    EXPECT_EQ(*it, 4);
    it -= 2;
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(it + 4, std_range.end());
    EXPECT_EQ(std_range.end() - it, 4);
}

TEST_F(RangeTest, ToStdWithEmptyRange) {
    Array<int, 0> empty;
    const auto std_range = empty | toStd();

    EXPECT_EQ(std_range.begin(), std_range.end());
    const auto dist = std::distance(std_range.begin(), std_range.end());
    EXPECT_EQ(dist, 0);

    int sum = 0;
    std::ranges::for_each(std_range, [&sum](int) { sum += 1; });
    EXPECT_EQ(sum, 0);

    const auto accum = std::accumulate(std_range.begin(), std_range.end(), 0);
    EXPECT_EQ(accum, 0);
}

TEST_F(RangeTest, ToStdWithBuiltinArray) {
    constexpr int builtin[] = {1, 2, 3, 4, 5};
    const auto std_range = builtin | toStd();
    static_assert(std::contiguous_iterator<decltype(std_range.begin())>);
    EXPECT_TRUE((std::same_as<std::contiguous_iterator_tag, decltype(std_range.begin())::iterator_category>));

    int sum = 0;
    std::ranges::for_each(std_range, [&sum](const int x) { sum += x; });
    EXPECT_EQ(sum, 15);

    const auto dist = std::distance(std_range.begin(), std_range.end());
    EXPECT_EQ(dist, 5);

    auto it = std_range.begin();
    EXPECT_EQ(it[0], 1);
    EXPECT_EQ(it[4], 5);
    EXPECT_EQ(*(it + 2), 3);
}

TEST_F(RangeTest, ToStdWithChainedAdapters) {
    const auto chained = arr | skip(1_size) | take(3_size) | transform([](const int x) { return x * 2; }) | toStd();
    static_assert(std::forward_iterator<decltype(chained.begin())>);  // transform reduces to forward
    EXPECT_TRUE((std::same_as<std::forward_iterator_tag, decltype(chained.begin())::iterator_category>));

    std::vector<int> result;
    std::ranges::copy(chained, std::back_inserter(result));
    EXPECT_EQ(result, std::vector({4, 6, 8}));  // (2*2, 3*2, 4*2)

    const auto dist = std::distance(chained.begin(), chained.end());
    EXPECT_EQ(dist, 3);

    const auto accum = std::accumulate(chained.begin(), chained.end(), 0);
    EXPECT_EQ(accum, 18);  // 4+6+8
}

TEST_F(RangeTest, ToStdWithFilter) {
    const auto filtered = arr | filter([](int x) { return x % 2 == 1; }) | toStd();
    static_assert(std::forward_iterator<decltype(filtered.begin())>);  // filter is forward
    EXPECT_TRUE((std::same_as<std::forward_iterator_tag, decltype(filtered.begin())::iterator_category>));

    std::vector<int> result;
    std::ranges::copy(filtered, std::back_inserter(result));
    EXPECT_EQ(result, std::vector<int>({1, 3, 5}));

    const auto dist = std::distance(filtered.begin(), filtered.end());
    EXPECT_EQ(dist, 3);
}

TEST_F(RangeTest, ToStdWithReverse) {
    const auto reversed = arr | reverse() | toStd();
    static_assert(std::bidirectional_iterator<decltype(reversed.begin())>);  // reverse is bidirectional
    EXPECT_TRUE((std::same_as<std::bidirectional_iterator_tag, decltype(reversed.begin())::iterator_category>));

    std::vector<int> result;
    std::ranges::copy(reversed, std::back_inserter(result));
    EXPECT_EQ(result, std::vector({5, 4, 3, 2, 1}));

    auto it = reversed.begin();
    ++it; ++it;  // at 3
    EXPECT_EQ(*it, 3);
    --it;  // at 4
    EXPECT_EQ(*it, 4);

    const auto dist = std::distance(reversed.begin(), reversed.end());
    EXPECT_EQ(dist, 5);
}

// TEST_F(RangeTest, ToStdWithZip) {
//     Array<int, 5> other{10, 20, 30, 40, 50};
//     const auto zipped = arr | zip(other) | toStd();
//     static_assert(std::forward_iterator<decltype(zipped.begin())>);  // zip is forward
//     EXPECT_TRUE((std::same_as<std::forward_iterator_tag, decltype(zipped.begin())::iterator_category>));
//
//     using Pair = Couple<int, int>;
//     std::vector<Pair> result;
//     std::copy(zipped.begin(), zipped.end(), std::back_inserter(result));
//     EXPECT_EQ(result[0], (Pair{1, 10}));
//     EXPECT_EQ(result[1], (Pair{2, 20}));
//     EXPECT_EQ(result[2], (Pair{3, 30}));
//     EXPECT_EQ(result[3], (Pair{4, 40}));
//     EXPECT_EQ(result[4], (Pair{5, 50}));
//
//     const auto dist = std::distance(zipped.begin(), zipped.end());
//     EXPECT_EQ(dist, 5);
// }

TEST_F(RangeTest, ToStdIteratorComparisonAndArithmetic) {
    const auto std_range = arr | toStd();
    auto it1 = std_range.begin();
    const auto it2 = std_range.begin() + 2;
    EXPECT_NE(it1, it2);
    EXPECT_LT(it1, it2);
    EXPECT_LE(it1, it2);
    EXPECT_GT(it2, it1);
    EXPECT_GE(it2, it1);
    EXPECT_EQ(it2 - it1, 2);

    it1 += 3;
    EXPECT_EQ(*it1, 4);
    it1 -= 1;
    EXPECT_EQ(*it1, 3);

    EXPECT_EQ(it1[1], 4);  // 3 + 1 offset
}