#include <algorithm>
#include <gtest/gtest.h>
#include <compare>
#include <numeric>
import original.basic.iterator;
import original.basic.number;
import original.basic.types;

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

TEST(IteratorTest, IterTraitsForBuiltInPointer)
{
    using Pointer = int*;
    using Ref = IterTraits<Pointer>::ReferenceType;
    static_assert(SameType<Ref, int&>);
    using P = IterTraits<Pointer>::PointerType;
    static_assert(SameType<P, int*>);
    using Val = IterTraits<Pointer>::ValueType;
    static_assert(SameType<Val, int>);
}