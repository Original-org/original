#include <gtest/gtest.h>
#include <array>

import original.basic.array;
import original.basic.number;
import orginal.basic.container;

using namespace original;
using namespace original::literals;

TEST(ArrayTest, BasicOperationsNonEmpty) {
    Array<int, 5> arr;

    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 0);
        EXPECT_EQ(arr.data()[i], 0);
    }

    EXPECT_EQ(arr.size(), 5);
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
    EXPECT_EQ(carr.size(), 5);
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

    // 使用范围 for
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
    for (auto cit = carr.begin(); cit != carr.end(); ++cit) {
        EXPECT_EQ(*cit, expected++);
    }
}

TEST(ArrayTest, EmptyArray) {
    using ArrayType = Array<int, 0>;
    ArrayType empty_arr;

    EXPECT_EQ(empty_arr.size(), 0);
    EXPECT_TRUE(empty_arr.empty());
    EXPECT_EQ(empty_arr.data(), nullptr);

    // begin() == end()
    const auto& cref = empty_arr;
    EXPECT_EQ(empty_arr.begin(), empty_arr.end());
    EXPECT_EQ(cref.begin(), ContainerTraits<ArrayType>::cBegin(empty_arr));
    EXPECT_EQ(cref.end(), ContainerTraits<ArrayType>::cEnd(empty_arr));
    EXPECT_EQ(cref.end(), cref.begin());

    int count = 0;
    for (int _ : empty_arr) {
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

struct NonTrivial {
    int value;
    bool constructed = false;

    NonTrivial() : value(0) {}
    explicit NonTrivial(const int v) : value(v), constructed(true) {}
    NonTrivial(const NonTrivial& other) : value(other.value), constructed(true) {}
};

TEST(ArrayTest, NonTrivialType) {
    Array<NonTrivial, 2> arr;
    EXPECT_EQ(arr[0_size].value, 0);
    EXPECT_EQ(arr[1_size].value, 0);

    arr[0_size] = NonTrivial(42);
    EXPECT_EQ(arr[0_size].value, 42);
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
    static_assert(std::same_as<ContainerTraits<Arr5>::SizeType, std::size_t>);

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

    EXPECT_EQ(ContainerTraits<Arr5>::size(arr), 5);
    EXPECT_FALSE(ContainerTraits<Arr5>::empty(arr));

    constexpr Arr0 empty;
    EXPECT_EQ(ContainerTraits<Arr0>::size(empty), 0);
    EXPECT_TRUE(ContainerTraits<Arr0>::empty(empty));

    EXPECT_NE(arr.data(), nullptr);
    static_assert(std::same_as<decltype(arr.data()), int*>);
}
