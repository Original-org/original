#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <compare>
#include <numeric>
import original.basic.iterator;
import original.basic.number;

using namespace original;
using namespace original::literals;


TEST(NormalIteratorTest, DefaultConstruction)
{
    NormalIterator<int, int&, int*, Diff> it;
    EXPECT_EQ(it.operator->(), nullptr);
}

TEST(NormalIteratorTest, PointerConstructionAndDereference)
{
    int arr[5] = {10, 20, 30, 40, 50};
    NormalIterator<int, int&, int*, Diff> it(arr + 2);

    EXPECT_EQ(*it, 30);
    EXPECT_EQ(it.operator->(), &arr[2]);
    EXPECT_EQ(it[0_diff], 30);
    EXPECT_EQ(it[1_diff], 40);
    EXPECT_EQ(it[-1_diff], 20);
}

TEST(NormalIteratorTest, EqualityAndThreeWayComparison)
{
    static constexpr int arr[5]{};
    constexpr DefaultIterator it1(arr);
    constexpr DefaultIterator it2(arr);
    constexpr DefaultIterator it3(arr + 3);

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);

    EXPECT_EQ(it1 <=> it2, std::strong_ordering::equal);
    EXPECT_EQ(it1 <=> it3, std::strong_ordering::less);
    EXPECT_EQ(it3 <=> it1, std::strong_ordering::greater);
}

TEST(NormalIteratorTest, IncrementAndDecrement)
{
    constexpr int arr[5] = {1, 2, 3, 4, 5};
    DefaultIterator it(arr + 1);

    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);

    it++; // NOLINT
    EXPECT_EQ(*it, 4);

    --it;
    EXPECT_EQ(*it, 3);

    it--; // NOLINT
    EXPECT_EQ(*it, 2);
}

TEST(NormalIteratorTest, CompoundAssignment)
{
    constexpr int arr[10]{};
    DefaultIterator it(arr + 2);

    it += 3_diff;
    EXPECT_EQ(it.operator->(), arr + 5);

    it -= static_cast<std::ptrdiff_t>(2);
    EXPECT_EQ(it.operator->(), arr + 3);
}

TEST(NormalIteratorTest, AdditionAndSubtraction)
{
    constexpr int arr[10]{};
    DefaultIterator it(arr + 1);

    auto it2 = it + static_cast<std::ptrdiff_t>(4);
    EXPECT_EQ(it2.operator->(), arr + 5);

    auto it3 = 2_diff + it;
    EXPECT_EQ(it3.operator->(), arr + 3);

    auto it4 = it - 1_diff;
    EXPECT_EQ(it4.operator->(), arr);

    it += 5_diff;
    EXPECT_EQ(*it, arr[6]);

    it -= static_cast<std::ptrdiff_t>(3);
    EXPECT_EQ(*it, arr[3]);
}

TEST(NormalIteratorTest, SubscriptOperator)
{
    static constexpr int arr[5] = {100, 200, 300, 400, 500};
    constexpr DefaultIterator it(arr + 2);

    EXPECT_EQ(it[0_diff], 300);
    EXPECT_EQ(it[1_diff], 400);
    EXPECT_EQ(it[-1_diff], 200);
    EXPECT_EQ(it[2_diff], 500);
    EXPECT_EQ(it[-2_diff], 100);

    EXPECT_EQ(it[static_cast<std::ptrdiff_t>(0)], 300);
    EXPECT_EQ(it[static_cast<std::ptrdiff_t>(1)], 400);
    EXPECT_EQ(it[static_cast<std::ptrdiff_t>(-1)], 200);
    EXPECT_EQ(it[static_cast<std::ptrdiff_t>(2)], 500);
    EXPECT_EQ(it[static_cast<std::ptrdiff_t>(-2)], 100);

    constexpr DefaultIterator cit(arr + 2);
    EXPECT_EQ(cit[0_diff], 300);
    EXPECT_EQ(cit[-1_diff], 200);
}

TEST(NormalIteratorTest, IteratorDifference)
{
    static constexpr int arr[10]{};
    constexpr auto it1(arr);
    constexpr auto it2(arr + 7);

    EXPECT_EQ(it2 - it1, 7_diff);
    EXPECT_EQ(it1 - it2, -7_diff);
}

TEST(NormalIteratorTest, CopyConstructionAndAssignment)
{
    static constexpr int arr[5] = {1, 2, 3, 4, 5};
    constexpr DefaultIterator it1(arr + 3);

    NormalIterator it2(it1);
    EXPECT_EQ(*it2, 4);
    EXPECT_TRUE(it1 == it2);

    constexpr auto it3 = it1;
    EXPECT_EQ(*it3, 4);
    EXPECT_TRUE(it1 == it3);
}

TEST(NormalIteratorTest, DefaultIteratorAlias)
{
    int arr[3] = {7, 8, 9};
    DefaultIterator it(arr + 1);

    EXPECT_EQ(*it, 8);
    ++it;
    EXPECT_EQ(*it, 9);
}

TEST(StdIteratorAdapterTest, BasicConstructionAndDereference)
{
    int arr[5] = {10, 20, 30, 40, 50};
    DefaultIterator base_it(arr + 2);

    StdIteratorAdapter<DefaultIterator<int>> it(base_it);

    EXPECT_EQ(*it, 30);
    EXPECT_EQ(it.operator->(), &arr[2]);
    EXPECT_EQ(it[0], 30);
    EXPECT_EQ(it[0_diff], 30);
    EXPECT_EQ(it[1], 40);
    EXPECT_EQ(it[1_diff], 40);
    EXPECT_EQ(it[-1], 20);
    EXPECT_EQ(it[-1_diff], 20);
}

TEST(StdIteratorAdapterTest, EqualityAndThreeWayComparison)
{
    static constexpr int arr[5]{};
    constexpr DefaultIterator base1(arr);
    constexpr DefaultIterator base2(arr);
    constexpr DefaultIterator base3(arr + 3);

    constexpr StdIteratorAdapter<DefaultIterator<const int>> it1(base1);
    constexpr StdIteratorAdapter<DefaultIterator<const int>> it2(base2);
    constexpr StdIteratorAdapter<DefaultIterator<const int>> it3(base3);

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);

    EXPECT_EQ(it1 <=> it2, std::strong_ordering::equal);
    EXPECT_EQ(it1 <=> it3, std::strong_ordering::less);
    EXPECT_EQ(it3 <=> it1, std::strong_ordering::greater);
}

TEST(StdIteratorAdapterTest, IncrementAndDecrement)
{
    int arr[5] = {1, 2, 3, 4, 5};
    DefaultIterator base(arr + 1);
    StdIteratorAdapter<DefaultIterator<int>> it(base);

    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);

    it++; // NOLINT
    EXPECT_EQ(*it, 4);

    --it;
    EXPECT_EQ(*it, 3);

    it--; // NOLINT
    EXPECT_EQ(*it, 2);
}

TEST(StdIteratorAdapterTest, CompoundAssignmentAndAdditionSubtraction)
{
    int arr[10]{};
    DefaultIterator base(arr + 2);
    StdIteratorAdapter<DefaultIterator<int>> it(base);

    it += 3;
    EXPECT_EQ(it.operator->(), arr + 5);

    it -= 2_diff;
    EXPECT_EQ(it.operator->(), arr + 3);

    const auto it2 = it + 4;
    EXPECT_EQ(it2.operator->(), arr + 7);

    const auto it3 = 2_diff + it;
    EXPECT_EQ(it3.operator->(), arr + 5);

    const auto it4 = it - 1;
    EXPECT_EQ(it4.operator->(), arr + 2);
}

TEST(StdIteratorAdapterTest, IteratorDifference)
{
    static constexpr int arr[10]{};
    constexpr DefaultIterator base1(arr);
    constexpr DefaultIterator base2(arr + 7);

    constexpr StdIteratorAdapter<DefaultIterator<const int>> it1(base1);
    constexpr StdIteratorAdapter<DefaultIterator<const int>> it2(base2);

    EXPECT_EQ(it2 - it1, 7);
    EXPECT_EQ(it1 - it2, -7);
}

TEST(StdIteratorAdapterTest, SubscriptOperator)
{
    static constexpr int arr[5] = {100, 200, 300, 400, 500};
    constexpr DefaultIterator base(arr + 2);
    constexpr StdIteratorAdapter<DefaultIterator<const int>> it(base);

    EXPECT_EQ(it[0], 300);
    EXPECT_EQ(it[1], 400);
    EXPECT_EQ(it[-1], 200);
    EXPECT_EQ(it[2], 500);
    EXPECT_EQ(it[-2], 100);
    EXPECT_EQ(it[0_diff], 300);
    EXPECT_EQ(it[1_diff], 400);
    EXPECT_EQ(it[-1_diff], 200);
    EXPECT_EQ(it[2_diff], 500);
    EXPECT_EQ(it[-2_diff], 100);
}

TEST(StdIteratorAdapterTest, CopyConstructionAndDefaultConstructor)
{
    static constexpr int arr[5] = {1, 2, 3, 4, 5};
    constexpr DefaultIterator base(arr + 3);
    constexpr StdIteratorAdapter<DefaultIterator<const int>> it1(base);

    const StdIteratorAdapter it2(it1);
    EXPECT_EQ(*it2, 4);
    EXPECT_TRUE(it1 == it2);

    const StdIteratorAdapter<DefaultIterator<int>> it3;
    EXPECT_EQ(it3.operator->(), nullptr);
}

TEST(StdIteratorAdapterTest, WorksWithExplicitDifferenceType)
{
    using CustomIter = NormalIterator<const int, const int&, const int*, long long>;
    const int arr[5] = {10, 20, 30, 40, 50};
    const CustomIter base(arr + 2);

    StdIteratorAdapter<CustomIter> it(base);

    EXPECT_EQ(*it, 30);
    it += 2_diff;
    EXPECT_EQ(*it, 50);

    const auto diff = it - StdIteratorAdapter<CustomIter>(base);
    EXPECT_EQ(diff, 2_diff);
}

TEST(StdIteratorAdapterTest, STLCompatibility)
{
    int arr[5] = {5, 3, 1, 4, 2};

    const DefaultIterator begin(arr);
    const DefaultIterator end(arr + 5);

    const StdIteratorAdapter<DefaultIterator<int>> adapted_begin(begin);
    const StdIteratorAdapter<DefaultIterator<int>> adapted_end(end);

    const auto min_it = std::min_element(adapted_begin, adapted_end);
    EXPECT_EQ(*min_it, 1);

    const auto max_it = std::max_element(adapted_begin, adapted_end);
    EXPECT_EQ(*max_it, 5);

    std::sort(adapted_begin, adapted_end);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);
}

TEST(StdIteratorAdapterTest, IteratorCategoryDetection)
{
    using DefaultIter = DefaultIterator<int>;
    using DefaultAdapter = StdIteratorAdapter<DefaultIter>;

    static_assert(std::random_access_iterator<DefaultAdapter>);
    static_assert(std::is_same_v<
        DefaultAdapter::iterator_category,
        std::random_access_iterator_tag>);
}

TEST(StdIteratorAdapterTest, WorksWithSTLContainers)
{
    std::vector src = {1, 2, 3, 4, 5};
    const DefaultIterator begin(src.data());
    const DefaultIterator end(src.data() + src.size());
    StdIteratorAdapter<DefaultIterator<int>> adapted_begin(begin);
    const StdIteratorAdapter<DefaultIterator<int>> adapted_end(end);

    int sum = 0;
    std::for_each(adapted_begin, adapted_end, [&sum](const int x) { sum += x; });
    EXPECT_EQ(sum, 15);

    const auto it = std::find(adapted_begin, adapted_end, 3);
    EXPECT_NE(it, adapted_end);
    EXPECT_EQ(*it, 3);

    const int count = std::count(adapted_begin, adapted_end, 2);
    EXPECT_EQ(count, 1);
}

TEST(StdIteratorAdapterTest, WorksWithStandardAlgorithms)
{
    std::array arr = {6, 2, 8, 4, 1, 9};

    const StdIteratorAdapter<DefaultIterator<int>> begin(DefaultIterator{arr.data()});
    const StdIteratorAdapter<DefaultIterator<int>> end(DefaultIterator{arr.data() + arr.size()});

    std::sort(begin, end);
    EXPECT_TRUE(std::is_sorted(begin, end));
    EXPECT_EQ(begin[0], 1);
    EXPECT_EQ(begin[1], 2);
    EXPECT_EQ(end[-1], 9);

    EXPECT_TRUE(std::binary_search(begin, end, 4));
    EXPECT_FALSE(std::binary_search(begin, end, 7));

    const auto lower = std::lower_bound(begin, end, 5);
    EXPECT_EQ(*lower, 6);

    const auto upper = std::upper_bound(begin, end, 5);
    EXPECT_EQ(*upper, 6);

    EXPECT_EQ(std::accumulate(begin, end, 0), 30); // 1+2+4+6+8+9
}

TEST(StdIteratorAdapterTest, IteratorTraitsCompleteness)
{
    using Iter = StdIteratorAdapter<DefaultIterator<int>>;
    using Traits = std::iterator_traits<Iter>;

    static_assert(std::is_same_v<Traits::value_type, int>);
    static_assert(std::is_same_v<Traits::reference, int&>);
    static_assert(std::is_same_v<Traits::pointer, int*>);
    static_assert(std::is_same_v<Traits::difference_type, std::ptrdiff_t>);
    static_assert(std::is_same_v<Traits::iterator_category,
                  std::random_access_iterator_tag>);

    using ConstIter = StdIteratorAdapter<DefaultIterator<const int>>;
    using ConstTraits = std::iterator_traits<ConstIter>;

    static_assert(std::is_same_v<ConstTraits::value_type, const int>);
    static_assert(std::is_same_v<ConstTraits::reference, const int&>);
    static_assert(std::is_same_v<ConstTraits::pointer, const int*>);
}

TEST(StdIteratorAdapterTest, MoveSemantics)
{
    std::vector vec = {1, 2, 3};

    StdIteratorAdapter<DefaultIterator<int>> it1(DefaultIterator{vec.data()});
    StdIteratorAdapter it2(std::move(it1));

    EXPECT_EQ(*it2, 1);

    it1 = std::move(it2);
    EXPECT_EQ(*it1, 1);
}