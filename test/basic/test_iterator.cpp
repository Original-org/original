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
    auto it = iterator::makeIterator<int*>(nullptr);
    EXPECT_EQ(it.operator->(), nullptr);
}

TEST(NormalIteratorTest, PointerConstructionAndDereference)
{
    int arr[5] = {10, 20, 30, 40, 50};
    auto it = iterator::makeIterator(arr + 2);

    EXPECT_EQ(*it, 30);
    EXPECT_EQ(it.operator->(), &arr[2]);
    EXPECT_EQ(it[0_diff], 30);
    EXPECT_EQ(it[1_diff], 40);
    EXPECT_EQ(it[-1_diff], 20);
}

TEST(NormalIteratorTest, EqualityAndThreeWayComparison)
{
    static constexpr int arr[5]{};
    constexpr auto it1 = iterator::makeIterator(arr);
    constexpr auto it2 = iterator::makeIterator(arr);
    constexpr auto it3 = iterator::makeIterator(arr + 3);

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);

    EXPECT_EQ(it1 <=> it2, std::strong_ordering::equal);
    EXPECT_EQ(it1 <=> it3, std::strong_ordering::less);
    EXPECT_EQ(it3 <=> it1, std::strong_ordering::greater);
}

TEST(NormalIteratorTest, IncrementAndDecrement)
{
    constexpr int arr[5] = {1, 2, 3, 4, 5};
    auto it = iterator::makeIterator(arr + 1);

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
    auto it = iterator::makeIterator(arr + 2);

    it += 3_diff;
    EXPECT_EQ(it.operator->(), arr + 5);

    it -= static_cast<std::ptrdiff_t>(2);
    EXPECT_EQ(it.operator->(), arr + 3);
}

TEST(NormalIteratorTest, AdditionAndSubtraction)
{
    constexpr int arr[10]{};
    auto it = iterator::makeIterator(arr + 1);

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
    constexpr auto it = iterator::makeIterator(arr + 2);

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

    constexpr auto cit = iterator::makeIterator(arr + 2);
    EXPECT_EQ(cit[0_diff], 300);
    EXPECT_EQ(cit[-1_diff], 200);
}

TEST(NormalIteratorTest, IteratorDifference)
{
    static constexpr int arr[10]{};
    constexpr auto it1 = iterator::makeIterator(arr);
    constexpr auto it2 = iterator::makeIterator(arr + 7);

    EXPECT_EQ(it2 - it1, 7_diff);
    EXPECT_EQ(it1 - it2, -7_diff);
}

TEST(NormalIteratorTest, CopyConstructionAndAssignment)
{
    static constexpr int arr[5] = {1, 2, 3, 4, 5};
    constexpr auto it1 = iterator::makeIterator(arr + 3);

    auto it2 = it1;
    EXPECT_EQ(*it2, 4);
    EXPECT_TRUE(it1 == it2);

    constexpr auto it3 = it1;
    EXPECT_EQ(*it3, 4);
    EXPECT_TRUE(it1 == it3);
}

TEST(NormalIteratorTest, DefaultIteratorAlias)
{
    int arr[3] = {7, 8, 9};
    auto it = iterator::makeIterator(arr + 1);

    EXPECT_EQ(*it, 8);
    ++it;
    EXPECT_EQ(*it, 9);
}

TEST(StdIteratorAdapterTest, BasicConstructionAndDereference)
{
    int arr[5] = {10, 20, 30, 40, 50};
    const auto base_it = iterator::makeIterator(arr + 2);

    auto it = iterator::toStd(base_it);

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
    constexpr auto base1 = iterator::makeIterator(arr);
    constexpr auto base2 = iterator::makeIterator(arr);
    constexpr auto base3 = iterator::makeIterator(arr + 3);

    constexpr auto it1 = iterator::toStd(base1);
    constexpr auto it2 = iterator::toStd(base2);
    constexpr auto it3 = iterator::toStd(base3);

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);

    EXPECT_EQ(it1 <=> it2, std::strong_ordering::equal);
    EXPECT_EQ(it1 <=> it3, std::strong_ordering::less);
    EXPECT_EQ(it3 <=> it1, std::strong_ordering::greater);
}

TEST(StdIteratorAdapterTest, IncrementAndDecrement)
{
    int arr[5] = {1, 2, 3, 4, 5};
    auto base = iterator::makeIterator(arr + 1);
    auto it = iterator::toStd(base);

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
    auto base = iterator::makeIterator(arr + 2);
    auto it = iterator::toStd(base);

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
    constexpr auto base1 = iterator::makeIterator(arr);
    constexpr auto base2 = iterator::makeIterator(arr + 7);

    constexpr auto it1 = iterator::toStd(base1);
    constexpr auto it2 = iterator::toStd(base2);

    EXPECT_EQ(it2 - it1, 7);
    EXPECT_EQ(it1 - it2, -7);
}

TEST(StdIteratorAdapterTest, SubscriptOperator)
{
    static constexpr int arr[5] = {100, 200, 300, 400, 500};
    constexpr auto base = iterator::makeIterator(arr + 2);
    constexpr auto it = iterator::toStd(base);

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
    constexpr auto base = iterator::makeIterator(arr + 3);
    constexpr auto it1 = iterator::toStd(base);

    const auto it2 = it1;
    EXPECT_EQ(*it2, 4);
    EXPECT_TRUE(it1 == it2);

    const auto it3 = iterator::toStd(iterator::makeIterator<int*>(nullptr));
    EXPECT_EQ(it3.operator->(), nullptr);
}

TEST(StdIteratorAdapterTest, WorksWithExplicitDifferenceType)
{
    const int arr[5] = {10, 20, 30, 40, 50};
    const auto base = iterator::makeIterator(arr + 2);

    auto it = iterator::toStd(base);

    EXPECT_EQ(*it, 30);
    it += 2_diff;
    EXPECT_EQ(*it, 50);

    const auto diff = it - iterator::toStd(iterator::makeIterator(arr + 2));
    EXPECT_EQ(diff, 2_diff);
}

TEST(StdIteratorAdapterTest, STLCompatibility)
{
    int arr[5] = {5, 3, 1, 4, 2};

    const auto begin = iterator::makeIterator(arr);
    const auto end = iterator::makeIterator(arr + 5);

    const auto adapted_begin = iterator::toStd(begin);
    const auto adapted_end = iterator::toStd(end);

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
        std::contiguous_iterator_tag>);
}

TEST(StdIteratorAdapterTest, WorksWithSTLContainers)
{
    std::vector src = {1, 2, 3, 4, 5};
    const auto begin = iterator::makeIterator(src.data());
    const auto end = iterator::makeIterator(src.data() + src.size());
    auto adapted_begin = iterator::toStd(begin);
    const auto adapted_end = iterator::toStd(end);

    int sum = 0;
    std::for_each(adapted_begin, adapted_end, [&sum](const int x) { sum += x; });
    EXPECT_EQ(sum, 15);

    const auto it = std::find(adapted_begin, adapted_end, 3);
    EXPECT_NE(it, adapted_end);
    EXPECT_EQ(*it, 3);

    const int count = std::count(adapted_begin, adapted_end, 2); // NOLINT
    EXPECT_EQ(count, 1);
}

TEST(StdIteratorAdapterTest, WorksWithStandardAlgorithms)
{
    std::array arr = {6, 2, 8, 4, 1, 9};

    const auto begin = iterator::toStd(iterator::makeIterator(arr.data()));
    const auto end = iterator::toStd(iterator::makeIterator(arr.data() + arr.size()));

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
                  std::contiguous_iterator_tag>);

    using ConstIter = StdIteratorAdapter<DefaultIterator<const int>>;
    using ConstTraits = std::iterator_traits<ConstIter>;

    static_assert(std::is_same_v<ConstTraits::value_type, const int>);
    static_assert(std::is_same_v<ConstTraits::reference, const int&>);
    static_assert(std::is_same_v<ConstTraits::pointer, const int*>);

    using OriginalTraits = IterTraits<Iter>;

    static_assert(std::is_same_v<OriginalTraits::IterType, Iter>);
}

TEST(StdIteratorAdapterTest, MoveSemantics)
{
    std::vector vec = {1, 2, 3};

    auto it1 = iterator::toStd(iterator::makeIterator(vec.data()));
    auto it2 = std::move(it1); // NOLINT

    EXPECT_EQ(*it2, 1);

    it1 = std::move(it2); // NOLINT
    EXPECT_EQ(*it1, 1);
}

TEST(IteratorFactoryFunctions, EnumerateBasicUsage)
{
    int arr[5] = {10, 20, 30, 40, 50};
    auto begin = iterator::makeIterator(arr);
    auto enum_it = iterator::enumerate(begin);

    EXPECT_EQ((*enum_it).first, 0_size);
    EXPECT_EQ((*enum_it).second, 10);

    ++enum_it;
    EXPECT_EQ((*enum_it).first, 1_size);
    EXPECT_EQ((*enum_it).second, 20);

    ++enum_it;
    EXPECT_EQ((*enum_it).first, 2_size);
    EXPECT_EQ((*enum_it).second, 30);

    ++enum_it;
    EXPECT_EQ((*enum_it).first, 3_size);
    EXPECT_EQ((*enum_it).second, 40);

    enum_it++; // NOLINT
    EXPECT_EQ((*enum_it).first, 4_size);
    EXPECT_EQ((*enum_it).second, 50);
}

TEST(IteratorFactoryFunctions, EnumerateWithStartIndex)
{
    int arr[5] = {100, 200, 300, 400, 500};
    const auto begin = iterator::makeIterator(arr + 1);

    auto enum_it = iterator::enumerate(begin, 10_size);

    EXPECT_EQ((*enum_it).first, 10_size);
    EXPECT_EQ((*enum_it).second, 200);

    ++enum_it;
    EXPECT_EQ((*enum_it).first, 11_size);
    EXPECT_EQ((*enum_it).second, 300);
}

TEST(IteratorFactoryFunctions, EnumerateConstIterator)
{
    int arr[3] = {1, 2, 3};
    const auto begin = iterator::makeIterator(arr);

    const auto enum_it = iterator::enumerate(begin);

    auto [idx, val] = *enum_it;
    EXPECT_EQ(idx, 0_size);
    EXPECT_EQ(val, 1);

    static_assert(std::is_const_v<std::remove_reference_t<decltype(idx)>>);
}

TEST(IteratorFactoryFunctions, EnumeratePrePostIncrementSemantics)
{
    int arr[3] = {7, 8, 9};
    auto it = iterator::enumerate(iterator::makeIterator(arr));

    const auto it_post = it++;
    EXPECT_EQ((*it_post).first, 0_size);
    EXPECT_EQ((*it_post).second, 7);

    EXPECT_EQ((*it).first, 1_size);
    EXPECT_EQ((*it).second, 8);

    const auto& it_pre = ++it;
    EXPECT_EQ((*it_pre).first, 2_size);
    EXPECT_EQ((*it_pre).second, 9);
}

TEST(IteratorFactoryFunctions, EnumerateIndexTracksUnderlyingIterator)
{
    int arr[4] = {10, 20, 30, 40};
    auto base = iterator::makeIterator(arr);
    auto enum_it = iterator::enumerate(base, 5_size);

    ++base;
    ++enum_it;

    EXPECT_EQ((*enum_it).first, 6_size);
    EXPECT_EQ((*enum_it).second, 20);
}

TEST(EnumIteratorTest, BasicUsageAndDereference)
{
    int arr[5] = {10, 20, 30, 40, 50};
    const auto base_begin = iterator::makeIterator(arr);
    auto enum_begin = iterator::enumerate(base_begin);

    EXPECT_EQ((*enum_begin).first, 0_size);
    EXPECT_EQ((*enum_begin).second, 10);

    ++enum_begin;
    EXPECT_EQ((*enum_begin).first, 1_size);
    EXPECT_EQ((*enum_begin).second, 20);

    enum_begin++; // NOLINT
    EXPECT_EQ((*enum_begin).first, 2_size);
    EXPECT_EQ((*enum_begin).second, 30);
}

TEST(EnumIteratorTest, StartIndexOverloads)
{
    int arr[4] = {100, 200, 300, 400};
    const auto base = iterator::makeIterator(arr + 1);

    const auto enum1 = iterator::enumerate(base, 10_size);
    EXPECT_EQ((*enum1).first, 10_size);
    EXPECT_EQ((*enum1).second, 200);

    const auto enum2 = iterator::enumerate(base, 20u);
    EXPECT_EQ((*enum2).first, 20_size);
    EXPECT_EQ((*enum2).second, 200);
}

TEST(EnumIteratorTest, EqualityComparison)
{
    int arr[3] = {1, 2, 3};
    const auto begin = iterator::makeIterator(arr);

    auto enum1 = iterator::enumerate(begin);
    const auto enum2 = iterator::enumerate(begin);
    EXPECT_TRUE(enum1 == enum2);

    ++enum1;
    EXPECT_FALSE(enum1 == enum2);

    const auto enum3 = iterator::enumerate(begin + 1_diff, 1_size);
    EXPECT_TRUE(enum1 == enum3);
}

TEST(EnumIteratorTest, PreAndPostIncrementSemantics)
{
    int arr[3] = {7, 8, 9};
    auto enum_it = iterator::enumerate(iterator::makeIterator(arr));

    const auto post = enum_it++;
    EXPECT_EQ((*post).first, 0_size);
    EXPECT_EQ((*post).second, 7);
    EXPECT_EQ((*enum_it).first, 1_size);
    EXPECT_EQ((*enum_it).second, 8);

    const auto& pre = ++enum_it;
    EXPECT_EQ((*pre).first, 2_size);
    EXPECT_EQ((*pre).second, 9);
}

TEST(EnumIteratorTest, IteratorAccess)
{
    int arr[3] = {1, 2, 3};
    const auto base = iterator::makeIterator(arr + 1);
    auto enum_it = iterator::enumerate(base);

    EXPECT_EQ(*enum_it.iterator(), 2);
    ++enum_it;
    EXPECT_EQ(*enum_it.iterator(), 3);
}

TEST(EnumIteratorTest, STLCompatibility)
{
    std::array arr = {5, 1, 4, 2, 3};
    const auto base_begin = iterator::makeIterator(arr.data());
    const auto base_end   = iterator::makeIterator(arr.data() + arr.size());

    const auto enum_begin = iterator::enumerate(base_begin);
    const auto enum_end   = iterator::enumerate(base_end, Size{arr.size()});

    auto sum_indices = 0_size;
    int sum_values = 0;
    std::for_each(enum_begin, enum_end,
                  [&sum_indices, &sum_values](const auto& pair)
                  {
                      sum_indices += pair.first;
                      sum_values += pair.second;
                  });
    EXPECT_EQ(sum_indices, 0_size + 1_size + 2_size + 3_size + 4_size);
    EXPECT_EQ(sum_values, 15);
}

TEST(ReversedIteratorTest, BasicForwardAndBackwardTraversal)
{
    int arr[5] = {10, 20, 30, 40, 50};
    const auto base_end = iterator::makeIterator(arr + 5);
    auto rev_it = iterator::reverse(base_end);

    ++rev_it;
    EXPECT_EQ(*rev_it, 50);

    ++rev_it;
    EXPECT_EQ(*rev_it, 40);

    --rev_it;
    EXPECT_EQ(*rev_it, 50);

    rev_it++; // NOLINT
    EXPECT_EQ(*rev_it, 40);
}

TEST(ReversedIteratorTest, EqualityComparison)
{
    int arr[3] = {1, 2, 3};
    const auto base = iterator::makeIterator(arr + 2);

    auto rev1 = iterator::reverse(base);
    const auto rev2 = iterator::reverse(base);
    EXPECT_TRUE(rev1 == rev2);

    ++rev1;
    EXPECT_FALSE(rev1 == rev2);
}

TEST(ReversedIteratorTest, PreAndPostIncrementDecrement)
{
    int arr[4] = {10, 20, 30, 40};
    const auto base = iterator::makeIterator(arr + 3);
    auto rev = iterator::reverse(base);

    const auto post_inc = rev++;
    EXPECT_EQ(*post_inc, 40);
    EXPECT_EQ(*rev, 30);

    const auto& pre_inc = ++rev;
    EXPECT_EQ(*pre_inc, 20);

    const auto post_dec = rev--;
    EXPECT_EQ(*post_dec, 20);
    EXPECT_EQ(*rev, 30);

    const auto& pre_dec = --rev;
    EXPECT_EQ(*pre_dec, 40);
}

TEST(ReversedIteratorTest, IteratorAccess)
{
    int arr[3] = {1, 2, 3};
    const auto base = iterator::makeIterator(arr + 2);
    auto rev = iterator::reverse(base);

    EXPECT_EQ(*rev.iterator(), 3);
    ++rev;
    EXPECT_EQ(*rev.iterator(), 2);
}

TEST(ReversedIteratorTest, STLCompatibilityAccumulateReverse)
{
    int arr[5] = {10, 20, 30, 40, 50};
    const auto base_begin = iterator::makeIterator(arr - 1);
    const auto base_end   = iterator::makeIterator(arr + 4);

    const auto rev_begin = iterator::reverse(base_end);
    const auto rev_end   = iterator::reverse(base_begin);

    const int sum = std::accumulate(rev_begin, rev_end, 0);
    EXPECT_EQ(sum, 150);  // 50 + 40 + 30 + 20 + 10
}

TEST(IteratorViewConcept, DetectsIteratorWithIteratorMember)
{
    using DefaultIter = DefaultIterator<int>;
    static_assert(Iterator<DefaultIter>);

    using Adapter = StdIteratorAdapter<DefaultIter>;
    static_assert(IteratorView<Adapter>);

    using Enum = EnumIterator<DefaultIter>;
    static_assert(IteratorView<Enum>);
    using Rev = ReversedIterator<DefaultIter>;
    static_assert(IteratorView<Rev>);
}