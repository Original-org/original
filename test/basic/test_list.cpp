#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

import original.basic.list;
import original.basic.types;
import original.basic.number;
import original.basic.algorithm;
import original.basic.iterator;
import original.basic.range;

using original::List;
using original::Size;
using namespace original::literals;
using namespace original::range;

template<typename T>
void ExpectListEqualsVector(const List<T>& lst, const std::vector<T>& vec) {
    ASSERT_EQ(lst.size(), vec.size()) << "Size mismatch";

    auto it = lst.begin();
    for (size_t i = 0; i < vec.size(); ++i, ++it) {
        EXPECT_EQ(*it, vec[i]) << "Mismatch at index " << i;
    }
}

template<typename T>
class ListTest : public ::testing::Test {
protected:
    List<T> list_;
};

using IntListTest    = ListTest<int>;
using StringListTest = ListTest<std::string>;

TEST(ListTest, DefaultConstructor) {
    List<int> lst;
    EXPECT_EQ(lst.size(), 0_size);
    EXPECT_EQ(lst.capacity(), 1_size);
    EXPECT_TRUE(lst.begin() == lst.end());
}

TEST(ListTest, CapacityConstructor) {
    const List<int> lst{5_size};
    EXPECT_EQ(lst.size(), 0_size);
    EXPECT_GE(lst.capacity(), 5_size);

    const List<int> lst2{0_size};
    EXPECT_EQ(lst2.capacity(), 1_size);
}

TEST(ListTest, InitializerListConstructor) {
    const List lst{1, 2, 3, 4, 5};
    EXPECT_EQ(lst.size(), 5_size);
    EXPECT_GE(lst.capacity(), 5_size);

    const std::vector expected{1, 2, 3, 4, 5};
    ExpectListEqualsVector(lst, expected);
}

TEST(ListTest, CopyConstructor) {
    const List<std::string> src{"apple", "banana", "cherry"};
    const List dst{src};

    EXPECT_EQ(dst.size(), src.size());
    ExpectListEqualsVector(dst, {"apple", "banana", "cherry"});
}

TEST(ListTest, MoveConstructor) {
    List src{10, 20, 30, 40};
    const List<int> dst{std::move(src)};

    EXPECT_EQ(dst.size(), 4_size);
    EXPECT_EQ(src.size(), 0_size);
    EXPECT_TRUE(src.begin() == src.end());
}

TEST(ListTest, AtAndOperatorSquareBrackets) {
    List lst{100, 200, 300};

    EXPECT_EQ(lst.at(0_size), 100);
    EXPECT_EQ(lst.at(2_size), 300);
    EXPECT_EQ(lst[1_size], 200);

    EXPECT_THROW(lst.at(3_size), std::out_of_range);
    EXPECT_NO_THROW(lst[3_size]); // Test that operator[] does not throw an exception, even if the code is UB
}

TEST(ListTest, DataPointer) {
    const List lst{7, 14, 21};
    const int* raw = lst.data();

    EXPECT_EQ(raw[0], 7);
    EXPECT_EQ(raw[1], 14);
    EXPECT_EQ(raw[2], 21);
}

TEST(ListTest, PushEndAndEmplaceEnd) {
    List<std::string> lst;

    lst.pushEnd("first");
    lst.emplaceEnd("second v");
    lst.pushEnd(std::string{"third"});

    EXPECT_EQ(lst.size(), 3_size);
    EXPECT_EQ(lst[0_size], "first");
    EXPECT_EQ(lst[1_size], "second v");
    EXPECT_EQ(lst[2_size], "third");
}

TEST(ListTest, EmplaceAtMiddle) {
    List lst{1, 3, 4};

    lst.emplace(1_size, 2);
    lst.emplace(0_size, 0);

    const std::vector expected{0,1,2,3,4};
    ExpectListEqualsVector(lst, expected);
}

TEST(ListTest, PopEnd) {
    List lst{10, 20, 30, 40};

    lst.popEnd();
    EXPECT_EQ(lst.size(), 3_size);
    EXPECT_EQ(lst[2_size], 30);

    lst.popEnd();
    lst.popEnd();
    EXPECT_EQ(lst.size(), 1_size);

    lst.popEnd();
    EXPECT_EQ(lst.size(), 0_size);

    EXPECT_THROW(lst.popEnd(), std::logic_error);
}

TEST(ListTest, PopMiddle) {
    List lst{1, 2, 3, 4, 5};

    lst.pop(2_size);
    lst.pop(0_size);
    lst.pop(2_size);

    const std::vector expected{2,4};
    ExpectListEqualsVector(lst, expected);

    EXPECT_THROW(lst.pop(2_size), std::out_of_range);
}

TEST(ListTest, Reserve) {
    List<int> lst;
    EXPECT_EQ(lst.capacity(), 1_size);

    lst.reserve(100_size);
    EXPECT_GE(lst.capacity(), 100_size);
    EXPECT_EQ(lst.size(), 0_size);

    lst.reserve(50_size);
    EXPECT_GE(lst.capacity(), 100_size);
}

TEST(ListTest, AutomaticReallocationOnInsert) {
    List<int> lst;

    for (int i = 0; i < 100; ++i) {
        lst.pushEnd(i);
    }

    EXPECT_GE(lst.capacity(), 100_size);
    EXPECT_EQ(lst.size(), 100_size);
}

TEST(ListTest, EqualityAndThreeWayComparison) {
    const List a{1, 2, 3};
    const List b{1, 2, 3};
    const List c{1, 2, 4};
    const List d{1, 2};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(a == d);

    EXPECT_TRUE(a <=> b == 0);
    EXPECT_TRUE(a <=> c < 0);
    EXPECT_TRUE(c <=> a > 0);
    EXPECT_TRUE(d <=> a < 0);
}

TEST(ListTest, MoveOnlyTypeSemantics) {
    using UniqueIntPtr = std::unique_ptr<int>;

    List<UniqueIntPtr> src;
    src.emplaceEnd(std::make_unique<int>(10));
    src.emplaceEnd(std::make_unique<int>(20));
    src.emplaceEnd(std::make_unique<int>(30));

    EXPECT_EQ(src.size(), 3_size);
    EXPECT_EQ(*src[0_size], 10);
    EXPECT_EQ(*src[1_size], 20);
    EXPECT_EQ(*src[2_size], 30);

    List dst{std::move(src)};
    EXPECT_EQ(dst.size(), 3_size);
    EXPECT_EQ(src.size(), 0_size);
    EXPECT_EQ(*dst[0_size], 10);
    EXPECT_EQ(*dst[1_size], 20);
    EXPECT_EQ(*dst[2_size], 30);

    List<UniqueIntPtr> another;
    another.emplaceEnd(std::make_unique<int>(40));
    another = std::move(dst);
    EXPECT_EQ(another.size(), 3_size);
    EXPECT_EQ(dst.size(), 0_size);
    EXPECT_EQ(*another[0_size], 10);
    EXPECT_EQ(*another[1_size], 20);
    EXPECT_EQ(*another[2_size], 30);

    another.emplace(1_size, std::make_unique<int>(15));
    EXPECT_EQ(another.size(), 4_size);
    EXPECT_EQ(*another[0_size], 10);
    EXPECT_EQ(*another[1_size], 15);
    EXPECT_EQ(*another[2_size], 20);
    EXPECT_EQ(*another[3_size], 30);

    another.pop(2_size);
    EXPECT_EQ(another.size(), 3_size);
    EXPECT_EQ(*another[2_size], 30);

    List list1 {std::make_unique<int>(10), std::make_unique<int>(20), std::make_unique<int>(30)};
    List list2 {std::make_unique<int>(10), std::make_unique<int>(20), std::make_unique<int>(30)};
    EXPECT_FALSE(list1 <=> list2 == 0);
    EXPECT_FALSE(list1 == list2);
    EXPECT_TRUE(original::algorithms::equal(list1, list2,
    [](const std::unique_ptr<int> &a, const std::unique_ptr<int> &b)
    {
        return *a == *b;
    }));
    auto three_way_cmp_res =
    original::algorithms::lexicographicallyCompare(list1,
                                                   list2,
                                                   [](const std::unique_ptr<int> &a, const std::unique_ptr<int> &b)
    {
        return *a <=> *b;
    });
    EXPECT_EQ(three_way_cmp_res, std::strong_ordering::equal);
}

TEST(ListTest, ListIsRange) {
    List lst{1, 2, 3};

    auto it = lst.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(ListTest, TakeBasic) {
    List lst{1, 2, 3, 4, 5};

    List<int> result;
    for (int x : lst | take(3_size))
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 3_size);
    EXPECT_EQ(result[0_size], 1);
    EXPECT_EQ(result[2_size], 3);
}

TEST(ListTest, SkipBasic) {
    List lst{1, 2, 3, 4, 5};

    List<int> result;
    for (int x : lst | skip(2_size))
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 3_size);
    EXPECT_EQ(result[0_size], 3);
}

TEST(ListTest, ReverseBasic) {
    List lst{1, 2, 3};

    List<int> result;
    for (int x : lst | reverse())
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 3_size);
    EXPECT_EQ(result[0_size], 3);
    EXPECT_EQ(result[1_size], 2);
    EXPECT_EQ(result[2_size], 1);
}

TEST(ListTest, EnumerateBasic) {
    List lst{10, 20, 30};
    auto i = 0_size;
    for (const auto& [idx, val] : lst | enumerate())
    {
        EXPECT_EQ(idx, i);
        EXPECT_EQ(val, lst[i]);
        ++i;
    }
}

TEST(ListTest, EnumerateWithStart) {
    List lst{10, 20};
    const auto it = (lst | enumerate(5_size)).begin();
    const auto& [idx, val] = *it;
    EXPECT_EQ(idx, 5_size);
    EXPECT_EQ(val, 10);
}

TEST(ListTest, TransformBasic) {
    List lst{1, 2, 3};

    List<int> result;
    for (int x : lst | transform([](const int y){ return y * 2; }))
        result.pushEnd(x);

    EXPECT_EQ(result[0_size], 2);
    EXPECT_EQ(result[2_size], 6);
}

TEST(ListTest, FilterBasic) {
    List lst{1, 2, 3, 4, 5};

    List<int> result;
    for (int x : lst | filter([](const int y){ return y % 2 == 0; }))
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 2_size);
    EXPECT_EQ(result[0_size], 2);
    EXPECT_EQ(result[1_size], 4);
}

TEST(ListTest, ConcatBasic) {
    List a{1, 2};
    List b{3, 4};

    List<int> result;
    for (int x : a | concat(b))
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 4_size);
    EXPECT_EQ(result[3_size], 4);
}

TEST(ListTest, ZipStopsAtShorter) {
    List a{1, 2, 3};
    List b{10, 20};

    List<int> sum;
    for (const auto& [x, y] : a | zip(b))
        sum.pushEnd(x + y);

    EXPECT_EQ(sum.size(), 2_size);
    EXPECT_EQ(sum[0_size], 11);
    EXPECT_EQ(sum[1_size], 22);
}

TEST(ListTest, IotaBasic) {
    List<Size> result;
    for (auto x : iota(0_size, 5_size))
        result.pushEnd(x);

    EXPECT_EQ(result.size(), 5_size);
    EXPECT_EQ(result[4_size], 4_size);
}

TEST(ListTest, PipelineComposition) {
    List lst{1, 2, 3, 4, 5, 6};

    List<int> result;
    for (int x : lst
        | skip(1_size)
        | take(4_size)
        | filter([](const int y){ return y % 2 == 0; })
        | transform([](const int z){ return z * 10; }))
    {
        result.pushEnd(x);
    }

    ASSERT_EQ(result.size(), 2_size);
    EXPECT_EQ(result[0_size], 20);
    EXPECT_EQ(result[1_size], 40);
}

namespace
{
    struct ThrowOnNth {
        static inline int counter = 0;
        static inline int throw_on = -1;

        int value{};

        ThrowOnNth(const int v = 0) : value(v) { // NOLINT
            if (++counter == throw_on)
                throw std::runtime_error("ctor throw");
        }

        ThrowOnNth(const ThrowOnNth& other) : value(other.value) {
            if (++counter == throw_on)
                throw std::runtime_error("copy ctor throw");
        }

        ThrowOnNth(ThrowOnNth&& other) noexcept(false) : value(other.value) {
            if (++counter == throw_on)
                throw std::runtime_error("move ctor throw");
        }

        ThrowOnNth& operator=(const ThrowOnNth&) = default;
        ThrowOnNth& operator=(ThrowOnNth&&) = default;

        static void reset(const int throw_at) {
            counter = 0;
            throw_on = throw_at;
        }
    };
}

TEST(ListExceptionSafety, EmplaceEndStrongGuarantee) {
    List<ThrowOnNth> lst;
    lst.reserve(2_size);

    lst.emplaceEnd(1);
    lst.emplaceEnd(2);

    const auto snapshot = lst;

    ThrowOnNth::reset(3);

    try {
        lst.emplaceEnd(3);
        FAIL();
    } catch (const std::runtime_error&) {}

    EXPECT_EQ(lst.size(), snapshot.size());
    EXPECT_TRUE(original::algorithms::equal(
        lst, snapshot,
        [](auto& a, auto& b){ return a.value == b.value; }));
}

TEST(ListExceptionSafety, ReserveStrongGuarantee) {
    List<ThrowOnNth> lst{1, 2, 3};
    const auto snapshot = lst;

    ThrowOnNth::reset(1);

    try {
        lst.reserve(100_size);
        FAIL();
    } catch (...) {}

    EXPECT_TRUE(original::algorithms::equal(
        lst, snapshot,
        [](auto& a, auto& b){ return a.value == b.value; }));
}

namespace {
    struct MoveThrowsCopyOK {
        int value{};

        MoveThrowsCopyOK(int v = 0) : value(v) {} // NOLINT

        MoveThrowsCopyOK(const MoveThrowsCopyOK& other) noexcept
            : value(other.value) {}

        MoveThrowsCopyOK(MoveThrowsCopyOK&& other) noexcept(false)
            : value(other.value) {
            throw std::runtime_error("move throws");
        }
    };
}

TEST(ListExceptionSafety, ReallocateUsesCopyIfMoveMayThrow) {
    List<MoveThrowsCopyOK> lst;
    lst.reserve(1_size);
    lst.emplaceEnd(1);
    lst.emplaceEnd(2);

    EXPECT_NO_THROW({
        lst.reserve(10_size);
    });

    EXPECT_EQ(lst.size(), 2_size);
    EXPECT_EQ(lst[0_size].value, 1);
    EXPECT_EQ(lst[1_size].value, 2);
}