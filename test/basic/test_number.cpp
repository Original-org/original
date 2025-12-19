#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <type_traits>
import original.basic.number;

using namespace original;
using namespace original::literals;

// Test type aliases
TEST(NumberTest, TypeAliases) {
    // Check if types are correct
    static_assert(std::is_same_v<I8::Type, std::int8_t>);
    static_assert(std::is_same_v<I16::Type, std::int16_t>);
    static_assert(std::is_same_v<I32::Type, std::int32_t>);
    static_assert(std::is_same_v<I64::Type, std::int64_t>);

    static_assert(std::is_same_v<U8::Type, std::uint8_t>);
    static_assert(std::is_same_v<U16::Type, std::uint16_t>);
    static_assert(std::is_same_v<U32::Type, std::uint32_t>);
    static_assert(std::is_same_v<U64::Type, std::uint64_t>);

    static_assert(std::is_same_v<F32::Type, float>);
    static_assert(std::is_same_v<F64::Type, double>);
    static_assert(std::is_same_v<F80::Type, long double>);

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
    constexpr F32 float32{3.14f};
    EXPECT_FLOAT_EQ(float32.value(), 3.14f);

    constexpr F64 float64{2.71828};
    EXPECT_DOUBLE_EQ(float64.value(), 2.71828);

    // Construction via conversion from smaller types
    constexpr U32 from_byte{100_byte .value()};
    EXPECT_EQ(from_byte.value(), 100);

    constexpr F64 from_f32{F32{3.14f}.value()};
    EXPECT_FLOAT_EQ(from_f32.value(), 3.14f);
}

// Test conversion operators
TEST(NumberTest, ConversionOperators) {
    constexpr I32 int_val{123};

    // Explicit conversion to underlying type
    constexpr auto val = static_cast<std::int32_t>(int_val);
    EXPECT_EQ(val, 123);

    // Conversion to bool
    constexpr I32 zero{0};
    constexpr I32 nonzero{42};
    EXPECT_FALSE(static_cast<bool>(zero));
    EXPECT_TRUE(static_cast<bool>(nonzero));

    // Floating-point conversion
    constexpr F32 float_val{1.5f};
    constexpr auto f = static_cast<float>(float_val);
    EXPECT_FLOAT_EQ(f, 1.5f);
}

// Test comparison operators
TEST(NumberTest, ComparisonOperators) {
    constexpr I32 a{10};
    constexpr I32 b{20};
    constexpr I32 c{10};

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
    constexpr F32 f1{1.0f};
    constexpr F32 f2{2.0f};
    constexpr F32 f3{1.0f};

    EXPECT_TRUE(f1 < f2);
    EXPECT_TRUE(f1 <= f2);
    EXPECT_TRUE(f1 <= f3);
    EXPECT_TRUE(f1 == f3);
}

// Test arithmetic operators
TEST(NumberTest, ArithmeticOperators) {
    constexpr I32 a{10};
    constexpr I32 b{3};

    // Addition
    I32 sum = a + b;
    EXPECT_EQ(sum.value(), 13);

    // Subtraction
    I32 diff = a - b;
    EXPECT_EQ(diff.value(), 7);

    // Multiplication
    constexpr I32 prod = a * b;
    EXPECT_EQ(prod.value(), 30);

    // Division
    constexpr I32 quot = a / b;
    EXPECT_EQ(quot.value(), 3);

    // Modulo
    constexpr I32 mod = a % b;
    EXPECT_EQ(mod.value(), 1);

    // Operations with underlying types
    sum = a + 5;
    EXPECT_EQ(sum.value(), 15);

    diff = a - 2;
    EXPECT_EQ(diff.value(), 8);
}

// Test compound assignment operators
TEST(NumberTest, CompoundAssignmentOperators) {
    I32 a{10};

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
    I32 a{10};

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
    constexpr auto a = 12_u8;  // 12
    constexpr auto b = 10_u8;  // 10

    // Bitwise AND
    constexpr U8 and_result = a & b;
    EXPECT_EQ(and_result.value(), 0b1000);  // 8

    // Bitwise OR
    constexpr U8 or_result = a | b;
    EXPECT_EQ(or_result.value(), 0b1110);  // 14

    // Bitwise XOR
    constexpr U8 xor_result = a ^ b;
    EXPECT_EQ(xor_result.value(), 0b0110);  // 6

    // Bitwise NOT
    constexpr U8 not_result = ~a;
    EXPECT_EQ(not_result.value(), static_cast<std::uint8_t>(~12));

    // Shift operations
    constexpr U8 shift_left = a << 1;
    EXPECT_EQ(shift_left.value(), 0b11000);  // 24

    constexpr U8 shift_right = a >> 1;
    EXPECT_EQ(shift_right.value(), 0b0110);  // 6

    // Compound assignment
    U8 c{a};
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
    constexpr F32 a{2.5f};
    constexpr F32 b{1.5f};

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
    F32 c{a};
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

TEST(NumberTest, NarrowCastIntegralSameSign)
{
    constexpr I64 a{123};
    constexpr I32 b = narrowCast<I32>(a);

    EXPECT_EQ(b.value(), 123);

    constexpr auto c = 456_u64;
    constexpr auto d = narrowCast<U32>(c);

    EXPECT_EQ(d.value(), 456);
}

TEST(NumberTest, NarrowCastFloating)
{
    constexpr F64 a{3.25};
    constexpr F32 b = narrowCast<F32>(a);

    EXPECT_FLOAT_EQ(b.value(), 3.25f);
}

TEST(NumberTest, SignCastSameWidth)
{
    constexpr I32 a{-1};
    constexpr U32 b = signCast<U32>(a);

    EXPECT_EQ(b.value(), static_cast<std::uint32_t>(-1));

    constexpr U32 c{0xFFFFFFFFu};
    constexpr I32 d = signCast<I32>(c);

    EXPECT_EQ(d.value(), static_cast<std::int32_t>(0xFFFFFFFFu));
}

TEST(NumberTest, NumericCastIntegral)
{
    constexpr I64 a{-42};
    constexpr auto b = numericCast<U32>(a);

    EXPECT_EQ(b.value(), static_cast<std::uint32_t>(-42));

    constexpr F32 c{1.5f};
    constexpr auto d = numericCast<F64>(c);

    EXPECT_DOUBLE_EQ(d.value(), 1.5);

    constexpr I32 e{-10};
    constexpr auto f = numericCast<F64>(e);

    EXPECT_DOUBLE_EQ(f.value(), -10.0);

    constexpr F64 g{3.9};
    constexpr auto h = numericCast<I32>(g);

    EXPECT_EQ(h.value(), 3);  // truncation
}

TEST(NumberTest, StdSizeBridge)
{
    using namespace original;

    constexpr size_t n = 12345;
    constexpr auto u = fromStdSize(n);

    EXPECT_EQ(u.value(), 12345u);

    constexpr size_t m = toStdSize(u);
    EXPECT_EQ(m, n);
    EXPECT_EQ(m, u);
    EXPECT_EQ(n, u);
}

// Test traits
TEST(NumberTest, NumberTraits) {
    // NumberTrait test
    EXPECT_TRUE(NumberTrait<I32>::value);
    EXPECT_TRUE(NumberTrait<F32>::value);
    EXPECT_FALSE(NumberTrait<int>::value);
    EXPECT_FALSE(NumberTrait<std::string>::value);

    static_assert(std::is_same_v<NumberTrait<I32>::Type, std::int32_t>);
    static_assert(std::is_same_v<NumberTrait<F64>::Type, double>);

    // IsNumber test
    EXPECT_TRUE(IS_NUMBER<I32>);
    EXPECT_TRUE(IS_NUMBER<F64>);
    EXPECT_FALSE(IS_NUMBER<int>);
    EXPECT_FALSE(IS_NUMBER<float>);

    // IsNumberLike test
    EXPECT_TRUE(IS_NUMBER_LIKE<I32>);
    EXPECT_TRUE(IS_NUMBER_LIKE<F32>);
    EXPECT_TRUE(IS_NUMBER_LIKE<int>);
    EXPECT_TRUE(IS_NUMBER_LIKE<float>);
    EXPECT_FALSE(IS_NUMBER_LIKE<std::string>);

    // Concept tests
    static_assert(Number<I32>);
    static_assert(!Number<int>);
    static_assert(NumberLike<I32>);
    static_assert(NumberLike<int>);
    static_assert(!NumberLike<std::string>);

    // NumberType test
    static_assert(std::is_same_v<NumberType<I32>, std::int32_t>);
    static_assert(std::is_same_v<NumberType<F64>, double>);

    // NumberLikeType test
    static_assert(std::is_same_v<NumberLikeType<I32>, std::int32_t>);
    static_assert(std::is_same_v<NumberLikeType<int>, int>);
    static_assert(std::is_same_v<NumberLikeType<F32>, float>);
    static_assert(std::is_same_v<NumberLikeType<float>, float>);

    EXPECT_TRUE(true);
}

// Test numeric algorithms
TEST(NumberTest, NumericAlogorithms) {
    // Maximum value test
    constexpr auto max_int = maximum<I32>();
    EXPECT_EQ(max_int.value(), std::numeric_limits<std::int32_t>::max());

    constexpr auto min_int = minimum<I32>();
    EXPECT_EQ(min_int.value(), std::numeric_limits<std::int32_t>::min());

    constexpr auto max_uint = maximum<U32>();
    EXPECT_EQ(max_uint.value(), std::numeric_limits<std::uint32_t>::max());

    constexpr auto min_uint = minimum<U32>();
    EXPECT_EQ(min_uint.value(), std::numeric_limits<std::uint32_t>::min());

    // Floating-point special values
    constexpr auto inf_val = infinity<F32>();
    EXPECT_FALSE(isFinite(inf_val));

    constexpr auto nan_val = nan<F64>();
    EXPECT_TRUE(isNaN(nan_val));

    constexpr auto neg_int = -1_i32;
    EXPECT_EQ(abs(neg_int), 1_i32);

    constexpr auto neg_float = -1.0_f32;
    EXPECT_EQ(abs(neg_float), 1.0_f32);
}

// Test mixed-type operations
TEST(NumberTest, MixedTypeOperations) {
    constexpr I32 a{10};
    constexpr I32 b{3};

    // These should all work normally
    EXPECT_EQ(a + b, 13);
    EXPECT_EQ(a * b, 30);
    EXPECT_EQ(a / b, 3);

    // Mixed operations with underlying types
    EXPECT_EQ(a + 5, 15);
    EXPECT_EQ(a - 2, 8);
    EXPECT_EQ(a * 3, 30);
}

TEST(NumberUnaryOperator, UnaryPlus)
{
    // signed integer
    {
        constexpr I32 a{42};
        auto b = +a;

        EXPECT_EQ(b, 42);
        EXPECT_EQ(a, 42);
        static_assert(std::is_same_v<decltype(b), I32>);
    }

    // unsigned integer
    {
        constexpr auto a = 42_u32;
        auto b = +a;

        EXPECT_EQ(b, 42u);
        EXPECT_EQ(a, 42u);
        static_assert(std::is_same_v<decltype(b), U32>);
    }

    // floating
    {
        constexpr F64 a{3.14};
        auto b = +a;

        EXPECT_DOUBLE_EQ(static_cast<double>(b), 3.14);
        static_assert(std::is_same_v<decltype(b), F64>);
    }
}

TEST(NumberUnaryOperator, UnaryMinusSignedInteger)
{
    constexpr I32 a{42};
    auto b = -a;

    EXPECT_EQ(b, -42);
    EXPECT_EQ(a, 42);
    static_assert(std::is_same_v<decltype(b), I32>);
}

TEST(NumberUnaryOperator, UnaryMinusFloating)
{
    constexpr F64 a{2.5};
    auto b = -a;

    EXPECT_DOUBLE_EQ(static_cast<double>(b), -2.5);
    static_assert(std::is_same_v<decltype(b), F64>);
}

namespace
{
    template<typename T>
    concept HasUnaryMinus = requires(T v) { -v; };
}

TEST(NumberUnaryOperator, UnaryMinusUnsignedIntegerNotAllowed)
{
    static_assert(!HasUnaryMinus<U8>);
    static_assert(!HasUnaryMinus<U16>);
    static_assert(!HasUnaryMinus<U32>);
    static_assert(!HasUnaryMinus<U64>);
}