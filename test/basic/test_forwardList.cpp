#include <gtest/gtest.h>
#include <concepts>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

import original.basic.forwardList;
import original.basic.iterator;
import original.basic.number;

using original::ForwardList;
using namespace original::literals;

template<typename T>
void ExpectListPrefixEqualsVector(ForwardList<T>& list, const std::vector<T>& expected)
{
    auto it = list.begin();
    for (size_t i = 0; i < expected.size(); ++i)
    {
        EXPECT_EQ(*it, expected[i]) << "Mismatch at index " << i;
        ++it;
    }
}

TEST(ForwardListTypeTraits, CopyAndMoveForCopyableValueType)
{
    using List = ForwardList<int>;

    static_assert(std::is_default_constructible_v<List>);
    static_assert(std::is_copy_constructible_v<List>);
    static_assert(std::is_copy_assignable_v<List>);
    static_assert(std::is_move_constructible_v<List>);
    static_assert(std::is_move_assignable_v<List>);
}

TEST(ForwardListTypeTraits, MoveOnlyValueTypeDisablesCopy)
{
    using MoveOnlyList = ForwardList<std::unique_ptr<int>>;

    static_assert(std::is_default_constructible_v<MoveOnlyList>);
    static_assert(!std::is_copy_constructible_v<MoveOnlyList>);
    static_assert(!std::is_copy_assignable_v<MoveOnlyList>);
    static_assert(std::is_move_constructible_v<MoveOnlyList>);
    static_assert(std::is_move_assignable_v<MoveOnlyList>);
}

TEST(ForwardListLifecycle, DefaultConstructionAndDestruction)
{
    EXPECT_NO_THROW({
        ForwardList<int> list;
    });
}

TEST(ForwardListLifecycle, MoveConstructionAndMoveAssignment)
{
    EXPECT_NO_THROW({
        ForwardList<int> source;
        ForwardList<int> target;

        ForwardList<int> moved{std::move(source)};
        target = std::move(moved);
    });
}

TEST(ForwardListLifecycle, SelfMoveAssignment)
{
    ForwardList<int> list;
    EXPECT_NO_THROW({
        list = std::move(list);
    });
}

TEST(ForwardListMutation, PushInterfacesMaintainOrder)
{
    ForwardList<int> list;

    list.pushBegin(2);
    list.pushBegin(1);
    list.pushEnd(4);
    list.push(2_size, 3);
    list.push(4_size, 5);

    ExpectListPrefixEqualsVector(list, {1, 2, 3, 4, 5});
}

TEST(ForwardListMutation, PushRvalueOverloadsMaintainOrder)
{
    ForwardList<int> list;

    int one = 1;
    int three = 3;
    int five = 5;

    list.pushBegin(std::move(three));
    list.pushBegin(std::move(one));
    list.pushEnd(std::move(five));
    list.push(2_size, 4);
    list.push(1_size, 2);

    ExpectListPrefixEqualsVector(list, {1, 2, 3, 4, 5});
}

TEST(ForwardListMutation, PopInterfacesMaintainOrder)
{
    ForwardList<int> list;
    list.pushEnd(1);
    list.pushEnd(2);
    list.pushEnd(3);
    list.pushEnd(4);
    list.pushEnd(5);

    list.popBegin();
    list.pop(1_size);
    list.popEnd();

    ExpectListPrefixEqualsVector(list, {2, 4});
}

TEST(ForwardListMutation, PushIndexOutOfRangeThrows)
{
    ForwardList<int> list;

    EXPECT_THROW(list.push(1_size, 10), std::out_of_range);

    list.pushEnd(1);
    EXPECT_THROW(list.push(2_size, 10), std::out_of_range);
}

TEST(ForwardListMutation, PopOnEmptyThrows)
{
    ForwardList<int> list;

    EXPECT_THROW(list.popBegin(), std::logic_error);
    EXPECT_THROW(list.popEnd(), std::logic_error);
    EXPECT_THROW(list.pop(0_size), std::logic_error);
}

TEST(ForwardListMutation, PopIndexOutOfRangeThrows)
{
    ForwardList<int> list;
    list.pushEnd(1);
    list.pushEnd(2);

    EXPECT_THROW(list.pop(2_size), std::out_of_range);
}

TEST(ForwardListIterator, IteratorConceptAndTraits)
{
    using Iter = ForwardList<int>::IterType;
    using ConstIter = ForwardList<int>::ConstIterType;

    static_assert(original::ForwardIterator<Iter>);
    static_assert(original::ForwardIterator<ConstIter>);

    static_assert(std::same_as<decltype(*std::declval<Iter&>()), int&>);
    static_assert(std::same_as<decltype(*std::declval<const Iter&>()), int&>);
    static_assert(std::same_as<decltype(*std::declval<ConstIter&>()), const int&>);
    static_assert(std::same_as<decltype(*std::declval<const ConstIter&>()), const int&>);

    using IterTraits = original::IterTraits<Iter>;
    using ConstIterTraits = original::IterTraits<ConstIter>;

    static_assert(std::same_as<IterTraits::ValueType, int>);
    static_assert(std::same_as<IterTraits::ReferenceType, int&>);
    static_assert(std::same_as<ConstIterTraits::ValueType, const int>);
    static_assert(std::same_as<ConstIterTraits::ReferenceType, const int&>);
}

TEST(ForwardListIterator, DefaultIteratorIncrementThrows)
{
    ForwardList<int>::IterType it;

    EXPECT_THROW(++it, std::out_of_range);
    EXPECT_THROW(it++, std::out_of_range);
}

TEST(ForwardListIterator, DefaultIteratorEquality)
{
    ForwardList<int>::IterType it1;
    ForwardList<int>::IterType it2;
    ForwardList<int>::ConstIterType cit1;
    ForwardList<int>::ConstIterType cit2;

    EXPECT_TRUE(it1 == it2);
    EXPECT_TRUE(cit1 == cit2);
}

TEST(ForwardListIterator, DefaultIteratorCopyAndAssignment)
{
    ForwardList<int>::IterType it1;
    ForwardList<int>::IterType it2{it1};
    ForwardList<int>::IterType it3;
    it3 = it2;

    EXPECT_TRUE(it1 == it2);
    EXPECT_TRUE(it2 == it3);

    ForwardList<int>::ConstIterType cit1;
    ForwardList<int>::ConstIterType cit2{cit1};
    ForwardList<int>::ConstIterType cit3;
    cit3 = cit2;

    EXPECT_TRUE(cit1 == cit2);
    EXPECT_TRUE(cit2 == cit3);
}

TEST(ForwardListIterator, IteratorStateUnchangedAfterIncrementThrow)
{
    ForwardList<int>::IterType it;
    ForwardList<int>::IterType def;

    EXPECT_THROW(++it, std::out_of_range);
    EXPECT_TRUE(it == def);

    EXPECT_THROW(it++, std::out_of_range);
    EXPECT_TRUE(it == def);
}
