#include <compare>
#include <gtest/gtest.h>

import original.basic.algorithm;
import original.basic.range;

TEST(AlgorithmsTest, EqualWithNativeArraysAndPointers) {

  constexpr int arr1[] = {1, 2, 3, 4, 5};
  constexpr int arr2[] = {1, 2, 3, 4, 5};
  constexpr int arr3[] = {1, 2, 3, 4, 6};
  constexpr int arr4[] = {1, 2, 3};
  constexpr int arr5[] = {1, 2, 3, 4, 5, 6};

  EXPECT_TRUE(original::algorithms::equal(arr1, arr2));
  EXPECT_FALSE(original::algorithms::equal(arr1, arr3));

  auto eq_ignore_parity = [](const int a, const int b) {
    return a % 2 == b % 2;
  };
  EXPECT_TRUE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr2), eq_ignore_parity));
  EXPECT_FALSE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr3), eq_ignore_parity));

  EXPECT_TRUE(original::algorithms::equal(
      original::begin(arr1), original::end(arr1), original::begin(arr2)));
  EXPECT_FALSE(original::algorithms::equal(
      original::begin(arr1), original::end(arr1), original::begin(arr3)));

  EXPECT_TRUE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr2), eq_ignore_parity));
  EXPECT_FALSE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr3), eq_ignore_parity));

  EXPECT_TRUE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr2), original::end(arr2)));
  EXPECT_FALSE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr3), original::end(arr3)));
  EXPECT_FALSE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr4), original::end(arr4)));
  EXPECT_FALSE(
      original::algorithms::equal(original::begin(arr1), original::end(arr1),
                                  original::begin(arr5), original::end(arr5)));

  EXPECT_TRUE(original::algorithms::equal(
      original::begin(arr1), original::end(arr1), original::begin(arr2),
      original::end(arr2), eq_ignore_parity));
  EXPECT_FALSE(original::algorithms::equal(
      original::begin(arr1), original::end(arr1), original::begin(arr3),
      original::end(arr3), eq_ignore_parity));
}

TEST(AlgorithmsTest, LexicographicalCompareWithNativeArraysAndPointers) {
  constexpr int arr1[] = {1, 2, 3, 4, 5};
  constexpr int arr2[] = {1, 2, 3, 4, 5};
  constexpr int arr3[] = {1, 2, 3, 4, 6};
  constexpr int arr4[] = {1, 2, 3};
  constexpr int arr5[] = {1, 2, 4, 4, 5};
  constexpr int arr6[] = {1, 2, 3, 4, 5, 6};

  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr1, arr2),
            std::strong_ordering::equal);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr1, arr3),
            std::strong_ordering::less);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr3, arr1),
            std::strong_ordering::greater);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr1, arr4),
            std::strong_ordering::greater);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr4, arr1),
            std::strong_ordering::less);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(arr1, arr5),
            std::strong_ordering::less);

  auto rev_cmp = [](const int a, const int b) { return b <=> a; };
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr1), original::end(arr1),
                original::begin(arr2), rev_cmp),
            std::strong_ordering::equal);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr1), original::end(arr1),
                original::begin(arr3), rev_cmp),
            std::strong_ordering::greater);

  EXPECT_EQ(
      original::algorithms::lexicographicallyCompare(
          original::begin(arr1), original::end(arr1), original::begin(arr2)),
      std::strong_ordering::equal);
  EXPECT_EQ(
      original::algorithms::lexicographicallyCompare(
          original::begin(arr1), original::end(arr1), original::begin(arr4)),
      std::strong_ordering::greater);

  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr1), original::end(arr1),
                original::begin(arr2), rev_cmp),
            std::strong_ordering::equal);

  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr1), original::end(arr1),
                original::begin(arr6), original::end(arr6)),
            std::strong_ordering::less);
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr6), original::end(arr6),
                original::begin(arr1), original::end(arr1)),
            std::strong_ordering::greater);

  EXPECT_EQ(original::algorithms::lexicographicallyCompare(
                original::begin(arr1), original::end(arr1),
                original::begin(arr2), original::end(arr2), rev_cmp),
            std::strong_ordering::equal);
}

TEST(AlgorithmsTest, EqualAndLexCompareWithRawPointers) {
  const auto ptr1 = new int[5]{1, 2, 3, 4, 5};
  const auto ptr2 = new int[5]{1, 2, 3, 4, 5};
  const auto ptr3 = new int[5]{1, 2, 3, 4, 0};

  EXPECT_TRUE(original::algorithms::equal(ptr1, ptr1 + 5, ptr2));
  EXPECT_FALSE(original::algorithms::equal(ptr1, ptr1 + 5, ptr3));

  EXPECT_EQ(
      original::algorithms::lexicographicallyCompare(ptr1, ptr1 + 5, ptr2),
      std::strong_ordering::equal);
  EXPECT_EQ(
      original::algorithms::lexicographicallyCompare(ptr1, ptr1 + 5, ptr3),
      std::strong_ordering::greater);

  EXPECT_TRUE(original::algorithms::equal(ptr1, ptr1 + 5, ptr2, ptr2 + 5));
  EXPECT_EQ(original::algorithms::lexicographicallyCompare(ptr1, ptr1 + 5, ptr2,
                                                           ptr2 + 5),
            std::strong_ordering::equal);

  delete[] ptr1;
  delete[] ptr2;
  delete[] ptr3;
}