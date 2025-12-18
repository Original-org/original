#include <cmath>
#include <gtest/gtest.h>
#include <cstdint>
#include <limits>
#include <type_traits>
import original.basic.number;

using namespace original;
using namespace original::literals;

// Test type aliases
TEST(NumberTest, TypeAliases) {
    // Check if types are correct
    static_assert(std::is_same_v<i8::Type, std::int8_t>);
    static_assert(std::is_same_v<i16::Type, std::int16_t>);
    static_assert(std::is_same_v<i32::Type, std::int32_t>);
    static_assert(std::is_same_v<i64::Type, std::int64_t>);

    static_assert(std::is_same_v<u8::Type, std::uint8_t>);
    static_assert(std::is_same_v<u16::Type, std::uint16_t>);
    static_assert(std::is_same_v<u32::Type, std::uint32_t>);
    static_assert(std::is_same_v<u64::Type, std::uint64_t>);

    static_assert(std::is_same_v<f32::Type, float>);
    static_assert(std::is_same_v<f64::Type, double>);
    static_assert(std::is_same_v<f80::Type, long double>);

    EXPECT_TRUE(true);
}

// Test construction and value retrieval
TEST(NumberTest, ConstructionAndValue) {
    // Integer construction
    constexpr auto int32 = 42_i32;
    EXPECT_EQ(int32.value(), 42);

    constexpr auto uint64 = 100_u64;
    EXPECT_EQ(uint64.value(), 100ULL);

    // Floating-point construction
    constexpr f32 float32{3.14f};
    EXPECT_FLOAT_EQ(float32.value(), 3.14f);

    constexpr f64 float64{2.71828};
    EXPECT_DOUBLE_EQ(float64.value(), 2.71828);

    // Construction via conversion from smaller types
    constexpr i32 from_i16{i16{100}.value()};
    EXPECT_EQ(from_i16.value(), 100);

    constexpr f64 from_f32{f32{3.14f}.value()};
    EXPECT_FLOAT_EQ(from_f32.value(), 3.14f);
}

// Test conversion operators
TEST(NumberTest, ConversionOperators) {
    constexpr i32 int_val{123};

    // Explicit conversion to underlying type
    constexpr auto val = static_cast<std::int32_t>(int_val);
    EXPECT_EQ(val, 123);

    // Conversion to bool
    constexpr i32 zero{0};
    constexpr i32 nonzero{42};
    EXPECT_FALSE(static_cast<bool>(zero));
    EXPECT_TRUE(static_cast<bool>(nonzero));

    // Floating-point conversion
    constexpr f32 float_val{1.5f};
    constexpr auto f = static_cast<float>(float_val);
    EXPECT_FLOAT_EQ(f, 1.5f);
}

// Test comparison operators
TEST(NumberTest, ComparisonOperators) {
    constexpr i32 a{10};
    constexpr i32 b{20};
    constexpr i32 c{10};

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a >= c);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a != b);

    // Comparison with underlying types
    EXPECT_TRUE(a == 10);
    EXPECT_TRUE(a < 20);
    EXPECT_TRUE(a <= 10);
    EXPECT_TRUE(a != 15);

    // Floating-point comparison
    constexpr f32 f1{1.0f};
    constexpr f32 f2{2.0f};
    constexpr f32 f3{1.0f};

    EXPECT_TRUE(f1 < f2);
    EXPECT_TRUE(f1 <= f2);
    EXPECT_TRUE(f1 <= f3);
    EXPECT_TRUE(f1 == f3);
}

// Test arithmetic operators
TEST(NumberTest, ArithmeticOperators) {
    constexpr i32 a{10};
    constexpr i32 b{3};

    // Addition
    i32 sum = a + b;
    EXPECT_EQ(sum.value(), 13);

    // Subtraction
    i32 diff = a - b;
    EXPECT_EQ(diff.value(), 7);

    // Multiplication
    constexpr i32 prod = a * b;
    EXPECT_EQ(prod.value(), 30);

    // Division
    constexpr i32 quot = a / b;
    EXPECT_EQ(quot.value(), 3);

    // Modulo
    constexpr i32 mod = a % b;
    EXPECT_EQ(mod.value(), 1);

    // Operations with underlying types
    sum = a + 5;
    EXPECT_EQ(sum.value(), 15);

    diff = a - 2;
    EXPECT_EQ(diff.value(), 8);
}

// Test compound assignment operators
TEST(NumberTest, CompoundAssignmentOperators) {
    i32 a{10};

    a += 5;
    EXPECT_EQ(a.value(), 15);

    a -= 3;
    EXPECT_EQ(a.value(), 12);

    a *= 2;
    EXPECT_EQ(a.value(), 24);

    a /= 4;
    EXPECT_EQ(a.value(), 6);

    a %= 4;
    EXPECT_EQ(a.value(), 2);
}

// Test increment and decrement operators
TEST(NumberTest, IncrementDecrementOperators) {
    i32 a{10};

    EXPECT_EQ((++a).value(), 11);
    EXPECT_EQ(a.value(), 11);

    EXPECT_EQ((a++).value(), 11);
    EXPECT_EQ(a.value(), 12);

    EXPECT_EQ((--a).value(), 11);
    EXPECT_EQ(a.value(), 11);

    EXPECT_EQ((a--).value(), 11);
    EXPECT_EQ(a.value(), 10);
}

// Test bitwise operators
TEST(NumberTest, BitwiseOperators) {
    u8 a{0b1100};  // 12
    u8 b{0b1010};  // 10

    // Bitwise AND
    u8 and_result = a & b;
    EXPECT_EQ(and_result.value(), 0b1000);  // 8

    // Bitwise OR
    u8 or_result = a | b;
    EXPECT_EQ(or_result.value(), 0b1110);  // 14

    // Bitwise XOR
    u8 xor_result = a ^ b;
    EXPECT_EQ(xor_result.value(), 0b0110);  // 6

    // Bitwise NOT
    u8 not_result = ~a;
    EXPECT_EQ(not_result.value(), static_cast<std::uint8_t>(~12));

    // Shift operations
    u8 shift_left = a << 1;
    EXPECT_EQ(shift_left.value(), 0b11000);  // 24

    u8 shift_right = a >> 1;
    EXPECT_EQ(shift_right.value(), 0b0110);  // 6

    // Compound assignment
    u8 c{a};
    c &= b;
    EXPECT_EQ(c.value(), 0b1000);

    c = a;
    c |= b;
    EXPECT_EQ(c.value(), 0b1110);

    c = a;
    c ^= b;
    EXPECT_EQ(c.value(), 0b0110);

    c = a;
    c <<= 1;
    EXPECT_EQ(c.value(), 0b11000);

    c = a;
    c >>= 1;
    EXPECT_EQ(c.value(), 0b0110);
}

// Test floating-point operations
TEST(NumberTest, FloatingPointOperations) {
    f32 a{2.5f};
    f32 b{1.5f};

    // Basic operations
    EXPECT_FLOAT_EQ((a + b).value(), 4.0f);
    EXPECT_FLOAT_EQ((a - b).value(), 1.0f);
    EXPECT_FLOAT_EQ((a * b).value(), 3.75f);
    EXPECT_FLOAT_EQ((a / b).value(), 2.5f / 1.5f);

    // Operations with underlying types
    EXPECT_FLOAT_EQ((a + 1.0f).value(), 3.5f);
    EXPECT_FLOAT_EQ((a - 1.0f).value(), 1.5f);
    EXPECT_FLOAT_EQ((a * 2.0f).value(), 5.0f);
    EXPECT_FLOAT_EQ((a / 2.0f).value(), 1.25f);

    // Compound assignment
    f32 c{a};
    c += b;
    EXPECT_FLOAT_EQ(c.value(), 4.0f);

    c = a;
    c -= b;
    EXPECT_FLOAT_EQ(c.value(), 1.0f);

    c = a;
    c *= b;
    EXPECT_FLOAT_EQ(c.value(), 3.75f);

    c = a;
    c /= b;
    EXPECT_FLOAT_EQ(c.value(), 2.5f / 1.5f);
}

// Test utility functions
TEST(NumberTest, UtilityFunctions) {
    // Test narrowCast
    constexpr i64 big_num{1000};
    constexpr i32 small_num = narrowCast<std::int32_t>(big_num);
    EXPECT_EQ(small_num.value(), 1000);

    // Test fromStdSize and toStdSize
    constexpr std::size_t size = 100;
    constexpr u64 u64_val = fromStdSize(size);
    EXPECT_EQ(u64_val.value(), 100ULL);

    constexpr std::size_t back_to_size = toStdSize(u64_val);
    EXPECT_EQ(back_to_size, 100ULL);
}

// Test traits
TEST(NumberTest, NumberTraits) {
    // NumberTrait test
    EXPECT_TRUE(NumberTrait<i32>::value);
    EXPECT_TRUE(NumberTrait<f32>::value);
    EXPECT_FALSE(NumberTrait<int>::value);
    EXPECT_FALSE(NumberTrait<std::string>::value);

    static_assert(std::is_same_v<NumberTrait<i32>::Type, std::int32_t>);
    static_assert(std::is_same_v<NumberTrait<f64>::Type, double>);

    // IsNumber test
    EXPECT_TRUE(IS_NUMBER<i32>);
    EXPECT_TRUE(IS_NUMBER<f64>);
    EXPECT_FALSE(IS_NUMBER<int>);
    EXPECT_FALSE(IS_NUMBER<float>);

    // IsNumberLike test
    EXPECT_TRUE(IS_NUMBER_LIKE<i32>);
    EXPECT_TRUE(IS_NUMBER_LIKE<f32>);
    EXPECT_TRUE(IS_NUMBER_LIKE<int>);
    EXPECT_TRUE(IS_NUMBER_LIKE<float>);
    EXPECT_FALSE(IS_NUMBER_LIKE<std::string>);

    // Concept tests
    static_assert(Number<i32>);
    static_assert(!Number<int>);
    static_assert(NumberLike<i32>);
    static_assert(NumberLike<int>);
    static_assert(!NumberLike<std::string>);

    // NumberType test
    static_assert(std::is_same_v<NumberType<i32>, std::int32_t>);
    static_assert(std::is_same_v<NumberType<f64>, double>);

    // NumberLikeType test
    static_assert(std::is_same_v<NumberLikeType<i32>, std::int32_t>);
    static_assert(std::is_same_v<NumberLikeType<int>, int>);
    static_assert(std::is_same_v<NumberLikeType<f32>, float>);
    static_assert(std::is_same_v<NumberLikeType<float>, float>);

    EXPECT_TRUE(true);
}

// Test edge cases
TEST(NumberTest, EdgeCases) {
    // Maximum value test
    constexpr i32 max_int{std::numeric_limits<std::int32_t>::max()};
    EXPECT_EQ(max_int.value(), std::numeric_limits<std::int32_t>::max());

    constexpr i32 min_int{std::numeric_limits<std::int32_t>::min()};
    EXPECT_EQ(min_int.value(), std::numeric_limits<std::int32_t>::min());

    // Floating-point special values
    constexpr f32 inf_val{std::numeric_limits<float>::infinity()};
    EXPECT_TRUE(std::isinf(inf_val.value()));

    constexpr f32 nan_val{std::numeric_limits<float>::quiet_NaN()};
    EXPECT_TRUE(std::isnan(nan_val.value()));
}

// Test mixed-type operations
TEST(NumberTest, MixedTypeOperations) {
    constexpr i32 a{10};
    constexpr i32 b{3};

    // These should all work normally
    EXPECT_EQ((a + b).value(), 13);
    EXPECT_EQ((a * b).value(), 30);
    EXPECT_EQ((a / b).value(), 3);

    // Mixed operations with underlying types
    EXPECT_EQ((a + 5).value(), 15);
    EXPECT_EQ((a - 2).value(), 8);
    EXPECT_EQ((a * 3).value(), 30);
}

TEST(NumberUnaryOperator, UnaryPlus)
{
    // signed integer
    {
        constexpr i32 a{42};
        auto b = +a;

        EXPECT_EQ(b, 42);
        EXPECT_EQ(a, 42);
        static_assert(std::is_same_v<decltype(b), i32>);
    }

    // unsigned integer
    {
        constexpr u32 a{42};
        auto b = +a;

        EXPECT_EQ(b, 42u);
        EXPECT_EQ(a, 42u);
        static_assert(std::is_same_v<decltype(b), u32>);
    }

    // floating
    {
        constexpr f64 a{3.14};
        auto b = +a;

        EXPECT_DOUBLE_EQ(static_cast<double>(b), 3.14);
        static_assert(std::is_same_v<decltype(b), f64>);
    }
}

TEST(NumberUnaryOperator, UnaryMinusSignedInteger)
{
    constexpr i32 a{42};
    auto b = -a;

    EXPECT_EQ(b, -42);
    EXPECT_EQ(a, 42);
    static_assert(std::is_same_v<decltype(b), i32>);
}

TEST(NumberUnaryOperator, UnaryMinusFloating)
{
    constexpr f64 a{2.5};
    auto b = -a;

    EXPECT_DOUBLE_EQ(static_cast<double>(b), -2.5);
    static_assert(std::is_same_v<decltype(b), f64>);
}

namespace
{
    template<typename T>
    concept HasUnaryMinus = requires(T v) { -v; };
}

TEST(NumberUnaryOperator, UnaryMinusUnsignedIntegerNotAllowed)
{
    static_assert(!HasUnaryMinus<u8>);
    static_assert(!HasUnaryMinus<u16>);
    static_assert(!HasUnaryMinus<u32>);
    static_assert(!HasUnaryMinus<u64>);
}