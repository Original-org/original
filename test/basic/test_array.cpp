#include <gtest/gtest.h>
#include <array>

import original.basic.array;
import original.basic.number;
import original.basic.container;
import original.basic.algorithm;

using namespace original;
using namespace original::literals;

TEST(ArrayTest, BasicOperationsNonEmpty) {
    Array<int, 5> arr;

    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 0);
        EXPECT_EQ(arr.data()[i], 0);
    }

    EXPECT_EQ(arr.size(), 5_size);
    EXPECT_FALSE(arr.empty());

    EXPECT_NE(arr.data(), nullptr);
    EXPECT_EQ(arr.data(), &arr[0_size]);

    arr[0_size] = 10;
    arr[1_size] = 20;
    EXPECT_EQ(arr[0_size], 10);
    EXPECT_EQ(arr[1_size], 20);

    const Array<int, 5>& carr = arr;
    EXPECT_EQ(carr[0_size], 10);
    EXPECT_EQ(carr[1_size], 20);
    EXPECT_EQ(carr.size(), 5_size);
    EXPECT_NE(carr.data(), nullptr);
}

TEST(ArrayTest, AtBoundsChecking) {
    Array<int, 3> arr;
    arr[0_size] = 42;

    EXPECT_EQ(arr.at(0_size), 42);
    EXPECT_NO_THROW(arr.at(2_size));

    EXPECT_THROW(arr.at(3_size), std::out_of_range);
    EXPECT_THROW(arr.at(100_size), std::out_of_range);

    const Array<int, 3>& carr = arr;
    EXPECT_EQ(carr.at(0_size), 42);
    EXPECT_THROW(carr.at(3_size), std::out_of_range);
}

TEST(ArrayTest, IteratorSupport) {
    Array<int, 4> arr;
    arr[0_size] = 1;
    arr[1_size] = 2;
    arr[2_size] = 3;
    arr[3_size] = 4;

    int expected = 1;
    for (int value : arr) {
        EXPECT_EQ(value, expected++);
    }

    auto it = arr.begin();
    EXPECT_EQ(*it, 1); ++it;
    EXPECT_EQ(*it, 2); ++it;
    EXPECT_EQ(*it, 3); ++it;
    EXPECT_EQ(*it, 4); ++it;
    EXPECT_EQ(it, arr.end());

    const Array<int, 4>& carr = arr;
    expected = 1;
    for (auto cit = carr.begin(); cit != carr.end(); ++cit) { // NOLINT
        EXPECT_EQ(*cit, expected++);
    }
}

TEST(ArrayTest, EmptyArray) {
    using ArrayType = Array<int, 0>;
    ArrayType empty_arr;

    EXPECT_EQ(empty_arr.size(), 0_size);
    EXPECT_TRUE(empty_arr.empty());
    EXPECT_EQ(empty_arr.data(), nullptr);

    // begin() == end()
    const auto& cref = empty_arr;
    EXPECT_EQ(empty_arr.begin(), empty_arr.end());
    EXPECT_EQ(cref.begin(), ContainerTraits<ArrayType>::cBegin(empty_arr));
    EXPECT_EQ(cref.end(), ContainerTraits<ArrayType>::cEnd(empty_arr));
    EXPECT_EQ(cref.end(), cref.begin());

    int count = 0;
    for (int _ : empty_arr) { // NOLINT
        ++count;
    }
    EXPECT_EQ(count, 0);

    EXPECT_THROW(empty_arr.at(0_size), std::out_of_range);

    const Array<int, 0>& c_empty = empty_arr;
    EXPECT_THROW(c_empty.at(0_size), std::out_of_range);
}

TEST(ArrayTraitsTest, IsArrayConcept) {
    static_assert(IsArray<Array<int, 5>>);
    static_assert(IsArray<Array<double, 0>>);
    static_assert(IsArray<Array<std::string, 10>>);

    static_assert(!IsArray<int>);
    static_assert(!IsArray<std::vector<int>>);
    static_assert(!IsArray<std::array<int, 5>>);
    static_assert(!IsArray<int[5]>);
}

TEST(ArrayTraitsTest, IsArrayLikeConcept) {
    static_assert(IsArrayLike<Array<int, 5>>);
    static_assert(IsArrayLike<Array<int, 0>>);

    static_assert(IsArrayLike<int[10]>);
    static_assert(IsArrayLike<const int[3]>);

    static_assert(IsArrayLike<std::array<int, 7>>);
    static_assert(IsArrayLike<std::array<std::string, 0>>);

    static_assert(!IsArrayLike<int>);
    static_assert(!IsArrayLike<std::vector<int>>);
}

TEST(ArrayTraitsTest, ArrayLikeType) {
    static_assert(std::same_as<ArrayLikeType<Array<int, 5>>, Array<int, 5>>);
    static_assert(std::same_as<ArrayLikeType<int[10]>, int[10]>);
    static_assert(std::same_as<ArrayLikeType<std::array<double, 3>>, std::array<double, 3>>);
}

namespace
{
    struct NonTrivial {
        int value;
        bool constructed = false;
        bool moved_from = false;  // 新增：标记是否已被移动

        NonTrivial() : value(0) {}

        explicit NonTrivial(const int v) : value(v), constructed(true) {}

        NonTrivial(const NonTrivial& other) : value(other.value), constructed(true) {}

        NonTrivial& operator=(const NonTrivial& other)
        {
            if (this == &other)
            {
                return *this;
            }

            value = other.value;
            constructed = true;
            return *this;
        }

        NonTrivial(NonTrivial&& other) noexcept
            : value(other.value), constructed(other.constructed)
        {
            other.value = 0;
            other.constructed = false;
            other.moved_from = true;
        }

        NonTrivial& operator=(NonTrivial&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            value = other.value;
            constructed = other.constructed;
            other.value = 0;
            other.constructed = false;
            other.moved_from = true;
            return *this;
        }
    };
}

TEST(ArrayTest, NonTrivialType) {
    Array<NonTrivial, 2> arr;
    EXPECT_EQ(arr[0_size].value, 0);
    EXPECT_EQ(arr[1_size].value, 0);

    arr[0_size] = NonTrivial(42);
    EXPECT_EQ(arr[0_size].value, 42);

    Array moved_construct(std::move(arr));
    EXPECT_EQ(moved_construct[0_size].value, 42);
    EXPECT_EQ(moved_construct[0_size].constructed, true);
    EXPECT_EQ(moved_construct[1_size].value, 0);

    EXPECT_EQ(arr[0_size].value, 0);
    EXPECT_EQ(arr[0_size].constructed, false);
    EXPECT_TRUE(arr[0_size].moved_from);
    EXPECT_EQ(arr[1_size].value, 0);
    EXPECT_TRUE(arr[1_size].moved_from);

    Array<NonTrivial, 2> target;
    target[0_size] = NonTrivial(100);
    target = std::move(moved_construct);

    EXPECT_EQ(target[0_size].value, 42);
    EXPECT_EQ(target[1_size].value, 0);

    EXPECT_TRUE(moved_construct[0_size].moved_from);
    EXPECT_EQ(moved_construct[0_size].value, 0);
    EXPECT_EQ(moved_construct[0_size].constructed, false);
}

TEST(ContainerTraitsTest, ArraySatisfiesContainerConcept) {
    static_assert(Container<Array<int, 5>>);
    static_assert(Container<Array<double, 0>>);
    static_assert(!Container<int>);
}

TEST(ContainerTraitsTest, ArraySatisfiesContiguousContainerConcept) {
    static_assert(ContiguousContainer<Array<int, 5>>);
    static_assert(ContiguousContainer<Array<char, 3>>);

    static_assert(ContiguousContainer<Array<int, 0>>);
}

TEST(ContainerTraitsTest, ContainerTraitsAccess) {
    using Arr5 = Array<int, 5>;
    using Arr0 = Array<int, 0>;

    static_assert(std::same_as<ContainerTraits<Arr5>::IterType, Arr5::IterType>);
    static_assert(std::same_as<ContainerTraits<Arr5>::ConstIterType, Arr5::ConstIterType>);
    static_assert(std::same_as<ContainerTraits<Arr5>::ValueType, int>);
    static_assert(std::same_as<ContainerTraits<Arr5>::SizeType, Size>);

    Arr5 arr;
    const Arr5::IterType b = ContainerTraits<Arr5>::begin(arr);
    const Arr5::IterType e = ContainerTraits<Arr5>::end(arr);
    const Arr5::ConstIterType f = ContainerTraits<Arr5>::cBegin(arr);
    const Arr5::ConstIterType g = ContainerTraits<Arr5>::cEnd(arr);
    const auto& cref = arr;
    EXPECT_EQ(b, arr.begin());
    EXPECT_EQ(e, arr.end());
    EXPECT_EQ(f, cref.begin());
    EXPECT_EQ(g, cref.end());

    EXPECT_EQ(ContainerTraits<Arr5>::size(arr), 5_size);
    EXPECT_FALSE(ContainerTraits<Arr5>::empty(arr));

    constexpr Arr0 empty;
    EXPECT_EQ(ContainerTraits<Arr0>::size(empty), 0_size);
    EXPECT_TRUE(ContainerTraits<Arr0>::empty(empty));

    EXPECT_NE(arr.data(), nullptr);
    static_assert(std::same_as<decltype(arr.data()), int*>);
}

TEST(ArrayGetTest, BasicGetAccess)
{
    Array<int, 4> arr(1, 2, 3, 4);

    EXPECT_EQ(std::get<0>(arr), 1);
    EXPECT_EQ(std::get<1>(arr), 2);
    EXPECT_EQ(std::get<2>(arr), 3);
    EXPECT_EQ(std::get<3>(arr), 4);

    std::get<1>(arr) = 42;
    EXPECT_EQ(arr[1_size], 42);
    EXPECT_EQ(std::get<1>(arr), 42);

    const Array<int, 4>& carr = arr;
    EXPECT_EQ(std::get<0>(carr), 1);
    EXPECT_EQ(std::get<1>(carr), 42);

    EXPECT_EQ(std::get<3>(std::move(arr)), 4); // NOLINT
}

TEST(ArrayTupleTraitsTest, TupleSizeAndElement)
{
    static_assert(std::tuple_size_v<Array<double, 5>> == 5);
    static_assert(std::tuple_size_v<Array<int, 0>> == 0);

    static_assert(std::is_same_v<std::tuple_element_t<0, Array<double, 5>>, double>);
    static_assert(std::is_same_v<std::tuple_element_t<3, Array<double, 5>>, double>);
}

TEST(ArrayStructuredBindingTest, BindingNonConst)
{
    Array<int, 3> arr(10, 20, 30);

    auto&& [a, b, c] = arr;

    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 20);
    EXPECT_EQ(c, 30);

    a = 100;
    b = 200;

    EXPECT_EQ(arr[0_size], 100);
    EXPECT_EQ(arr[1_size], 200);
    EXPECT_EQ(std::get<2>(arr), 30);
}

TEST(ArrayStructuredBindingTest, BindingConst)
{
    constexpr Array<int, 3> arr(5, 6, 7);

    auto [x, y, z] = arr;

    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 6);
    EXPECT_EQ(z, 7);
}

TEST(ArrayStructuredBindingTest, BindingCopy)
{
    Array<int, 2> arr(1, 2);

    auto&& [p, q] = arr;
    auto [r, s] = arr;

    r = 10; // NOLINT
    EXPECT_EQ(arr[0_size], 1);

    p = 100;
    EXPECT_EQ(arr[0_size], 100);
}

TEST(ArrayStructuredBindingTest, EmptyArrayBinding)
{
    static_assert(std::tuple_size_v<Array<int, 0>> == 0);
}

TEST(ArrayAlgoTest, EqualCompareBetweenArray) {
  constexpr Array<int, 4> a(1, 2, 3, 4);
  Array<int, 4> b(1, 2, 3, 4);

  // Member operator overload
  EXPECT_TRUE(a == b);

  // range overload
  EXPECT_TRUE(original::algorithms::equal(a, b));

  // iterator overload
  EXPECT_TRUE(original::algorithms::equal(a.begin(), a.end(), b.begin()));

  // change an element
  b[2_size] = 42;
  EXPECT_FALSE(original::algorithms::equal(a, b));

  // predicate overload (compare absolute values)
  Array<int, 4> c(1, -2, 3, -4);
  EXPECT_TRUE(original::algorithms::equal(
      a, c, [](const int x, const int y) { return x == std::abs(y); }));

  // bounded iterator overload
  b[2_size] = 3; // restore
  EXPECT_TRUE(
      original::algorithms::equal(a.begin(), a.end(), b.begin(), b.end()));

  constexpr Array<int, 0> empty1;
  constexpr Array<int, 0> empty2;

  EXPECT_TRUE(empty1 == empty2);
}

TEST(ArrayAlgoTest, LexicographicallyCompare) {
  constexpr Array<int, 4> a(1, 2, 3, 4);
  constexpr Array<int, 4> b(1, 2, 3, 4);
  constexpr Array<int, 4> c(1, 2, 4, 0);
  constexpr Array<int, 4> d(1, 2, 2, 5);

  // range overload - equal
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a, b) == 0);

  // range overload - less than
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a, c) < 0);

  // range overload - greater than
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a, d) > 0);

  // Member operator overload
  EXPECT_TRUE((a <=> b) == 0);
  EXPECT_TRUE((a <=> c) < 0);
  EXPECT_TRUE((a <=> d) > 0);
  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a <= c);
  EXPECT_TRUE(a > d);

  // iterator overload
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a.begin(), a.end(),
                                                             b.begin()) == 0);
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a.begin(), a.end(),
                                                             c.begin()) < 0);

  // predicate overload (compare absolute values)
  Array<int, 4> neg_a(1, -2, 3, -4);
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(
                  a, neg_a, [](const int x, const int y) {
                    return x <=> std::abs(y);
                  }) == 0);

  // bounded iterator overload
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(
                  a.begin(), a.end(), b.begin(), b.end()) == 0);
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(
                  a.begin(), a.end(), c.begin(), c.end()) < 0);

  // different sized ranges
  constexpr Array<int, 3> shorter(1, 2, 3);
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(shorter, a) < 0);
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(a, shorter) > 0);
  EXPECT_TRUE(shorter < a);
  EXPECT_TRUE(shorter <= a);
  EXPECT_TRUE(a > shorter);
  EXPECT_TRUE(a >= shorter);

  // empty ranges
  constexpr Array<int, 0> empty1;
  constexpr Array<int, 0> empty2;
  EXPECT_TRUE(original::algorithms::lexicographicallyCompare(empty1, empty2) == 0);
  EXPECT_TRUE((empty1 <=> empty2) == 0);
}