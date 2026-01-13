#include <compare>
#include <gtest/gtest.h>

import original.basic.algorithm;
import original.basic.range;
import original.basic.number;

using namespace original::literals;

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
            original::begin(arr1), original::end(arr1),
            original::begin(arr4), original::end(arr4)),
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

TEST(AlgorithmsTest, MinimumOfIntegers) {
    EXPECT_EQ(original::algorithms::minimum(3, 5), 3);
    EXPECT_EQ(original::algorithms::minimum(10, -2), -2);
    EXPECT_EQ(original::algorithms::minimum(3, -2, 1), -2);
    EXPECT_EQ(original::algorithms::minimum(3, 10, -2, 1), -2);
}

TEST(AlgorithmsTest, MaximumOfIntegers) {
    EXPECT_EQ(original::algorithms::maximum(3, 5), 5);
    EXPECT_EQ(original::algorithms::maximum(10, -2), 10);
    EXPECT_EQ(original::algorithms::maximum(3, -2, 1), 3);
    EXPECT_EQ(original::algorithms::maximum(3, 10, -2, 1), 10);
}

TEST(AlgorithmsTest, MinimumElementIteratorRange) {
    static constexpr int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    const auto min_it = original::algorithms::minElement(arr);
    EXPECT_EQ(*min_it, 1);
}

TEST(AlgorithmsTest, MaximumElementIteratorRange) {
    static constexpr int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    const auto max_it = original::algorithms::maxElement(arr);
    EXPECT_EQ(*max_it, 9);
}

TEST(AlgorithmsTest, CustomPredicate) {
    static constexpr int arr[] = {21, 13, 42, 35, 45, 37, 61};
    auto pred = [](const int a, const int b) {
        return a % 10 < b % 10;
    };
    const auto it = original::algorithms::minElement(
        original::begin(arr),
        original::end(arr),
        pred
    );
    EXPECT_EQ(*it, 21);

    const auto it2 = original::algorithms::minElement(arr, pred);
    EXPECT_EQ(*it2, 21);
}

TEST(AlgorithmsTest, MoveBackwardsBasicUsage) {
    {
        int src[] = {10, 20, 30, 40, 50};
        int dst[] = {1, 2, 3, 4, 5, 6, 7};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst)
        );

        EXPECT_EQ(dst[2], 10);
        EXPECT_EQ(dst[3], 20);
        EXPECT_EQ(dst[4], 30);
        EXPECT_EQ(dst[5], 40);
        EXPECT_EQ(dst[6], 50);

        EXPECT_EQ(result, dst + 2);
    }

    {
        int src[] = {100, 200, 300, 400, 500};
        int dst[] = {0, 0, 0, 0, 0, 0};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst),
            3_size
        );

        EXPECT_EQ(dst[3], 300);
        EXPECT_EQ(dst[4], 400);
        EXPECT_EQ(dst[5], 500);

        EXPECT_EQ(dst[0], 0);
        EXPECT_EQ(dst[1], 0);
        EXPECT_EQ(dst[2], 0);

        EXPECT_EQ(result, dst + 3);
    }

    {
        int src[] = {7, 8, 9};
        int dst[8] = {};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst),
            10_size
        );

        EXPECT_EQ(dst[5], 7);
        EXPECT_EQ(dst[6], 8);
        EXPECT_EQ(dst[7], 9);

        EXPECT_EQ(result, dst + 5);
    }
}

TEST(AlgorithmsTest, MoveBackwardsWithDifferentTypes) {
    {
        int src[] = {1, 2, 3, 4};
        long dst[6] = {};

        const auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst)
        );

        EXPECT_EQ(dst[2], 1L);
        EXPECT_EQ(dst[3], 2L);
        EXPECT_EQ(dst[4], 3L);
        EXPECT_EQ(dst[5], 4L);

        EXPECT_EQ(result, dst + 2);
    }
}

TEST(AlgorithmsTest, MoveBackwardsEdgeCases) {
    {
        int src[1]{};
        int dst[5] = {99, 99, 99, 99, 99};

        const auto result = original::algorithms::moveBackwards(
            original::begin(src), original::begin(src),
            original::end(dst)
        );

        EXPECT_EQ(dst[4], 99);
        EXPECT_EQ(result, original::end(dst));
    }

    {
        int src[] = {10, 20};
        int dst[4] = {1, 2, 3, 4};

        const auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst),
            0_size
        );

        EXPECT_EQ(dst[3], 4);
        EXPECT_EQ(result, original::end(dst));
    }
}

TEST(AlgorithmsTest, MoveBackwardsWithFourIterators) {
    {
        int src[] = {10, 20, 30, 40, 50};
        int dst[] = {1, 2, 3, 4, 5, 6, 7};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst)
        );

        EXPECT_EQ(dst[2], 10);
        EXPECT_EQ(dst[3], 20);
        EXPECT_EQ(dst[4], 30);
        EXPECT_EQ(dst[5], 40);
        EXPECT_EQ(dst[6], 50);

        EXPECT_EQ(result, dst + 2);
    }

    {
        int src[] = {100, 200, 300, 400, 500};
        int dst[] = {0, 0, 0, 0, 0, 0};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst),
            3_size
        );

        EXPECT_EQ(dst[3], 300);
        EXPECT_EQ(dst[4], 400);
        EXPECT_EQ(dst[5], 500);

        EXPECT_EQ(dst[0], 0);
        EXPECT_EQ(dst[1], 0);
        EXPECT_EQ(dst[2], 0);

        EXPECT_EQ(result, dst + 3);
    }

    {
        int src[] = {1, 2, 3, 4, 5};
        int dst[3] = {};

        auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst)
        );

        EXPECT_EQ(dst[0], 3);
        EXPECT_EQ(dst[1], 4);
        EXPECT_EQ(dst[2], 5);

        EXPECT_EQ(result, dst);
    }
}

TEST(AlgorithmsTest, MoveBackwardsWithCountLimiter) {
    {
        int src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dst[15] = {};

        const auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::end(dst),
            5_size
        );

        EXPECT_EQ(dst[10], 6);
        EXPECT_EQ(dst[11], 7);
        EXPECT_EQ(dst[12], 8);
        EXPECT_EQ(dst[13], 9);
        EXPECT_EQ(dst[14], 10);

        for (int i = 0; i < 10; ++i) {
            EXPECT_EQ(dst[i], 0);
        }

        EXPECT_EQ(result, dst + 10);
    }

    {
        int src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dst[15] = {};

        const auto result = original::algorithms::moveBackwards(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst),
            3_size
        );

        EXPECT_EQ(dst[12], 8);
        EXPECT_EQ(dst[13], 9);
        EXPECT_EQ(dst[14], 10);

        for (int i = 0; i < 12; ++i) {
            EXPECT_EQ(dst[i], 0);
        }

        EXPECT_EQ(result, dst + 12);
    }
}

TEST(AlgorithmsTest, MoveBackwardsOverlappingRanges) {
    {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        const auto result = original::algorithms::moveBackwards(
            data + 2, data + 7,
            data + 9,
            5_size
        );

        EXPECT_EQ(data[0], 1);
        EXPECT_EQ(data[1], 2);
        EXPECT_EQ(data[2], 3);
        EXPECT_EQ(data[3], 4);
        EXPECT_EQ(data[4], 3);
        EXPECT_EQ(data[5], 4);
        EXPECT_EQ(data[6], 5);
        EXPECT_EQ(data[7], 6);
        EXPECT_EQ(data[8], 7);
        EXPECT_EQ(data[9], 10);

        EXPECT_EQ(result, data + 4);
    }
}

TEST(AlgorithmsTest, MoveBasicUsage) {
    {
        int src[] = {10, 20, 30, 40, 50};
        int dst[] = {1, 2, 3, 4, 5, 6, 7};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst)
        );

        EXPECT_EQ(dst[0], 10);
        EXPECT_EQ(dst[1], 20);
        EXPECT_EQ(dst[2], 30);
        EXPECT_EQ(dst[3], 40);
        EXPECT_EQ(dst[4], 50);
        EXPECT_EQ(dst[5], 6);
        EXPECT_EQ(dst[6], 7);

        EXPECT_EQ(result, dst + 5);
    }

    {
        int src[] = {100, 200, 300, 400, 500};
        int dst[] = {0, 0, 0, 0, 0, 0};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst),
            3_size
        );

        EXPECT_EQ(dst[0], 100);
        EXPECT_EQ(dst[1], 200);
        EXPECT_EQ(dst[2], 300);
        for (const auto& e: dst | original::range::skip(3_size))
        {
            EXPECT_EQ(e, 0);
        }

        EXPECT_EQ(result, dst + 3);
    }

    {
        int src[] = {7, 8, 9};
        int dst[8] = {};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst),
            10_size
        );

        EXPECT_EQ(dst[0], 7);
        EXPECT_EQ(dst[1], 8);
        EXPECT_EQ(dst[2], 9);
        for (const auto& e: dst | original::range::skip(3_size))
        {
            EXPECT_EQ(e, 0);
        }

        EXPECT_EQ(result, dst + 3);
    }
}

TEST(AlgorithmsTest, MoveWithDifferentTypes) {
    {
        int src[] = {1, 2, 3, 4};
        long dst[6] = {};

        const auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst)
        );

        EXPECT_EQ(dst[0], 1L);
        EXPECT_EQ(dst[1], 2L);
        EXPECT_EQ(dst[2], 3L);
        EXPECT_EQ(dst[3], 4L);
        EXPECT_EQ(dst[4], 0L);
        EXPECT_EQ(dst[5], 0L);

        EXPECT_EQ(result, dst + 4);
    }
}

TEST(AlgorithmsTest, MoveEdgeCases) {
    {
        int src[1]{};
        int dst[5] = {99, 99, 99, 99, 99};

        const auto result = original::algorithms::move(
            original::begin(src), original::begin(src),
            original::begin(dst)
        );

        EXPECT_EQ(dst[0], 99);
        EXPECT_EQ(dst[1], 99);
        EXPECT_EQ(dst[2], 99);
        EXPECT_EQ(dst[3], 99);
        EXPECT_EQ(dst[4], 99);

        EXPECT_EQ(result, original::begin(dst));
    }

    {
        int src[] = {10, 20};
        int dst[4] = {1, 2, 3, 4};

        const auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst),
            0_size
        );

        EXPECT_EQ(dst[0], 1);
        EXPECT_EQ(dst[1], 2);
        EXPECT_EQ(dst[2], 3);
        EXPECT_EQ(dst[3], 4);

        EXPECT_EQ(result, original::begin(dst));
    }
}

TEST(AlgorithmsTest, MoveWithFourIterators) {
    {
        int src[] = {10, 20, 30, 40, 50};
        int dst[] = {1, 2, 3, 4, 5, 6, 7};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst)
        );

        EXPECT_EQ(dst[0], 10);
        EXPECT_EQ(dst[1], 20);
        EXPECT_EQ(dst[2], 30);
        EXPECT_EQ(dst[3], 40);
        EXPECT_EQ(dst[4], 50);
        EXPECT_EQ(dst[5], 6);
        EXPECT_EQ(dst[6], 7);

        EXPECT_EQ(result, dst + 5);
    }

    {
        int src[] = {100, 200, 300, 400, 500};
        int dst[] = {0, 0, 0, 0, 0, 0};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst),
            3_size
        );

        EXPECT_EQ(dst[0], 100);
        EXPECT_EQ(dst[1], 200);
        EXPECT_EQ(dst[2], 300);
        EXPECT_EQ(dst[3], 0);
        EXPECT_EQ(dst[4], 0);
        EXPECT_EQ(dst[5], 0);

        EXPECT_EQ(result, dst + 3);
    }

    {
        int src[] = {1, 2, 3, 4, 5};
        int dst[3] = {};

        auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst)
        );

        EXPECT_EQ(dst[0], 1);
        EXPECT_EQ(dst[1], 2);
        EXPECT_EQ(dst[2], 3);

        EXPECT_EQ(result, dst + 3);
    }
}

TEST(AlgorithmsTest, MoveWithCountLimiter) {
    {
        int src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dst[15] = {};

        const auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst),
            5_size
        );

        EXPECT_EQ(dst[0], 1);
        EXPECT_EQ(dst[1], 2);
        EXPECT_EQ(dst[2], 3);
        EXPECT_EQ(dst[3], 4);
        EXPECT_EQ(dst[4], 5);
        for (const auto& e: dst | original::range::skip(5_size))
        {
            EXPECT_EQ(e, 0);
        }

        EXPECT_EQ(result, dst + 5);
    }

    {
        int src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int dst[15] = {};

        const auto result = original::algorithms::move(
            original::begin(src), original::end(src),
            original::begin(dst), original::end(dst),
            7_size
        );

        EXPECT_EQ(dst[0], 1);
        EXPECT_EQ(dst[1], 2);
        EXPECT_EQ(dst[2], 3);
        EXPECT_EQ(dst[3], 4);
        EXPECT_EQ(dst[4], 5);
        EXPECT_EQ(dst[5], 6);
        EXPECT_EQ(dst[6], 7);

        EXPECT_EQ(result, dst + 7);
    }
}

TEST(AlgorithmsTest, MoveOverlappingRanges) {
    {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        const auto result = original::algorithms::move(
            data + 2, data + 7,
            data + 0,
            5_size
        );

        EXPECT_EQ(data[0], 3);
        EXPECT_EQ(data[1], 4);
        EXPECT_EQ(data[2], 5);
        EXPECT_EQ(data[3], 6);
        EXPECT_EQ(data[4], 7);
        EXPECT_EQ(data[5], 6);
        EXPECT_EQ(data[6], 7);
        EXPECT_EQ(data[7], 8);
        EXPECT_EQ(data[8], 9);
        EXPECT_EQ(data[9], 10);

        EXPECT_EQ(result, data + 5);
    }
}