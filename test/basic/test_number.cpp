#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
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
    constexpr U8 shift_left = a << 1u;
    EXPECT_EQ(shift_left.value(), 0b11000);  // 24

    constexpr U8 shift_right = a >> 1_size;
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
    c <<= 1u;
    EXPECT_EQ(c.value(), 0b11000);

    c = a;
    c >>= 1_u8;
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

// Test traits
TEST(NumberTest, NumberTraits) {
    static_assert(std::is_same_v<NumberType<I32>, std::int32_t>);
    static_assert(std::is_same_v<NumberType<F64>, double>);

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
    static_assert(std::is_same_v<NumberLikeType<F32>, float>);

    EXPECT_EQ(std::numeric_limits<I64>::digits, std::numeric_limits<std::int64_t>::digits);
    EXPECT_EQ(std::numeric_limits<I64>::max(), std::numeric_limits<std::int64_t>::max());
    EXPECT_EQ(std::numeric_limits<I64>::has_infinity,
              std::numeric_limits<std::int64_t>::has_infinity);

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

TEST(NumberLiterals, NormalRangeIntegral) {
    constexpr auto v1 = 42_i8;
    static_assert(v1.value() == 42);
    EXPECT_EQ(static_cast<int8_t>(v1), 42);

    constexpr auto v2 = 1000_i16;
    EXPECT_EQ(static_cast<int16_t>(v2), 1000);

    constexpr auto v3 = 255_byte;
    EXPECT_EQ(static_cast<uint8_t>(v3), 255);

    constexpr auto v4 = 1234567890123_i64;
    EXPECT_EQ(static_cast<int64_t>(v4), 1234567890123LL);
}

TEST(NumberLiterals, NormalRangeFloating) {
    constexpr auto f32 = 3.14_f32;
    EXPECT_FLOAT_EQ(static_cast<float>(f32), 3.14f);

    constexpr auto f64 = 2.718281828459045_f64;
    EXPECT_DOUBLE_EQ(static_cast<double>(f64), 2.718281828459045);

    constexpr auto f80 = 1.4142135623730951_f80;
    EXPECT_EQ(static_cast<long double>(f80), 1.4142135623730951L);
}

TEST(NumberLiteralsBoundaries, NormalBoundaries) {
    // constexpr I8 i8_min = -128_i8;  // No allowed
    // static_assert(i8_min.value() == minimum<I8>());
    // EXPECT_EQ(static_cast<std::int8_t>(i8_min), -128);

    constexpr auto i8_max = 127_i8;
    static_assert(i8_max == maximum<I8>());
    EXPECT_EQ(static_cast<std::int8_t>(i8_max), 127);

    constexpr auto u8_max = 255_u8;
    static_assert(u8_max == maximum<U8>());
    EXPECT_EQ(static_cast<std::uint8_t>(u8_max), 255);

    constexpr auto byte_max = 255_byte;
    static_assert(byte_max.value() == 255);

    constexpr auto i16_max = 32767_i16;
    EXPECT_EQ(static_cast<std::int16_t>(i16_max),  32767);

    constexpr auto i32_max = 2147483647_i32;
    EXPECT_EQ(static_cast<std::int32_t>(i32_max),  2147483647);

    constexpr auto i64_max = 9223372036854775807_i64;
    EXPECT_EQ(static_cast<std::int64_t>(i64_max), INT64_MAX);

    constexpr auto u64_max = 18446744073709551615_u64;
    EXPECT_EQ(static_cast<std::uint64_t>(u64_max), UINT64_MAX);
}

// Additional tests for width-promoting binary operator free functions
TEST(NumberTest, WidthPromotionBinaryOperators) {
    // Same signedness, different widths: result is the wider type
    constexpr auto a8  = 42_i8;
    constexpr auto a16 = 100_i16;
    auto sum1 = a8 + a16;
    static_assert(std::is_same_v<decltype(sum1), I16>);
    EXPECT_EQ(sum1, 142_i16);

    auto prod1 = a8 * a16;
    static_assert(std::is_same_v<decltype(prod1), I16>);
    EXPECT_EQ(prod1, 4200_i16);

    // Wider on left
    constexpr auto a32 = 200000_i32;
    auto sum2 = a32 + a16;
    static_assert(std::is_same_v<decltype(sum2), I32>);
    EXPECT_EQ(sum2, 200100_i32);

    // Wider on right
    auto sum3 = a16 + a32;
    static_assert(std::is_same_v<decltype(sum3), I32>);
    EXPECT_EQ(sum3, 200100_i32);

    // Unsigned examples
    constexpr auto u8_val  = 200_u8;
    constexpr auto u32_val = 100000_u32;
    auto diff_u = u32_val - u8_val;
    static_assert(std::is_same_v<decltype(diff_u), U32>);
    EXPECT_EQ(diff_u.value(), 99800u);

    // Floating-point promotion
    constexpr auto f32_val = 3.14_f32;
    constexpr auto f64_val = 2.71828_f64;
    auto float_sum = f32_val + f64_val;
    static_assert(std::is_same_v<decltype(float_sum), F64>);
    EXPECT_NEAR(float_sum.value(), (3.14_f64 + 2.71828_f64).value(), 1e-6);

    auto float_prod = f64_val * f32_val;
    static_assert(std::is_same_v<decltype(float_prod), F64>);
    EXPECT_NEAR(float_prod.value(), 2.71828 * 3.14, 1e-6);
}

// Additional tests for std::numeric_limits specializations
TEST(NumberTest, NumericLimitsSpecializations) {
    // Integer limits
    static_assert(std::numeric_limits<I8>::min() == std::numeric_limits<std::int8_t>::min());
    static_assert(std::numeric_limits<I8>::max() == std::numeric_limits<std::int8_t>::max());
    static_assert(std::numeric_limits<I8>::digits == std::numeric_limits<std::int8_t>::digits);  // NOLINT
    static_assert(std::numeric_limits<I8>::is_signed == std::numeric_limits<std::int8_t>::is_signed);  // NOLINT
    static_assert(std::numeric_limits<I8>::is_integer == std::numeric_limits<std::int8_t>::is_integer);  // NOLINT

    static_assert(std::numeric_limits<U64>::min() == std::numeric_limits<std::uint64_t>::min());
    static_assert(std::numeric_limits<U64>::max() == std::numeric_limits<std::uint64_t>::max());
    static_assert(std::numeric_limits<U64>::digits == std::numeric_limits<std::uint64_t>::digits);  // NOLINT
    static_assert(std::numeric_limits<U64>::is_signed == std::numeric_limits<std::uint64_t>::is_signed);  // NOLINT

    // Floating-point limits
    static_assert(std::numeric_limits<F32>::min() == std::numeric_limits<float>::min());
    static_assert(std::numeric_limits<F32>::max() == std::numeric_limits<float>::max());
    static_assert(std::numeric_limits<F32>::digits == std::numeric_limits<float>::digits);  // NOLINT
    static_assert(std::numeric_limits<F32>::is_signed == std::numeric_limits<float>::is_signed);  // NOLINT
    static_assert(std::numeric_limits<F32>::has_infinity == std::numeric_limits<float>::has_infinity);  // NOLINT
    static_assert(std::numeric_limits<F32>::has_quiet_NaN == std::numeric_limits<float>::has_quiet_NaN);  // NOLINT

    static_assert(std::numeric_limits<F80>::min() == std::numeric_limits<long double>::min());
    static_assert(std::numeric_limits<F80>::max() == std::numeric_limits<long double>::max());
    static_assert(std::numeric_limits<F80>::digits == std::numeric_limits<long double>::digits);  // NOLINT

    // Runtime checks for epsilon and infinity
    EXPECT_EQ(std::numeric_limits<F64>::epsilon(), std::numeric_limits<double>::epsilon());
    EXPECT_EQ(std::numeric_limits<F64>::infinity(), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::numeric_limits<I32>::has_infinity == false);  // Integers have no infinity
}

// Additional tests for std::hash specializations
TEST(NumberTest, HashSpecializations) {
    // Hash for integers should match underlying type
    constexpr std::hash<I32> hash_int{};
    constexpr I32 int_val{42};
    EXPECT_EQ(hash_int(int_val), std::hash<std::int32_t>{}(42));

    constexpr std::hash<U64> hash_uint{};
    constexpr auto uint_val = 123456789_u64;
    EXPECT_EQ(hash_uint(uint_val), std::hash<std::uint64_t>{}(123456789ULL));

    // Hash for floating-point should match underlying type
    constexpr std::hash<F32> hash_float{};
    constexpr auto float_val = 3.14_f32;
    EXPECT_EQ(hash_float(float_val), std::hash<float>{}(3.14f));

    constexpr std::hash<F64> hash_double{};
    constexpr auto double_val = 2.718_f64;
    EXPECT_EQ(hash_double(double_val), std::hash<double>{}(2.718));

    // Ensure different values produce different hashes (probabilistically)
    constexpr I32 int_val2{43};
    EXPECT_NE(hash_int(int_val), hash_int(int_val2));

    // Edge cases: zero, min, max
    constexpr I32 zero{0};
    EXPECT_EQ(hash_int(zero), std::hash<std::int32_t>{}(0));

    constexpr I32 min_val{std::numeric_limits<std::int32_t>::min()};
    EXPECT_EQ(hash_int(min_val), std::hash<std::int32_t>{}(std::numeric_limits<std::int32_t>::min()));

    constexpr F32 inf_val{std::numeric_limits<float>::infinity()};
    EXPECT_EQ(hash_float(inf_val), std::hash<float>{}(std::numeric_limits<float>::infinity()));
}

// Boundary checks that should throw from checked integer operations.
TEST(NumberCheckedBoundaries, DivisionMinByNegativeOne) {
    constexpr I32 min_val{std::numeric_limits<std::int32_t>::min()};
    constexpr I32 neg_one{-1};

    // Control: division path already has dedicated overflow protection.
    EXPECT_THROW(
        {
        const auto _ = min_val / neg_one;
        (void)_;
        },
        std::overflow_error
    );
}

TEST(NumberCheckedBoundaries, ModuloMinByNegativeOne) {
    constexpr I32 min_val{std::numeric_limits<std::int32_t>::min()};
    constexpr I32 neg_one{-1};

    EXPECT_THROW(
        {
        const auto _ = min_val % neg_one;
        (void)_;
        },
        std::overflow_error
    );
}

TEST(NumberCheckedBoundaries, ShiftLeftShouldRejectOverflowAndOutOfRangeCount) {
    auto max_u8 = 255_u8;

    // Regression target: left shift should reject arithmetic overflow for U8.
    EXPECT_THROW(
        {
        max_u8 <<= 1u;
        },
        std::overflow_error
    );

    auto one = 1_u8;

    // Regression target: shift count equal to width (8 for uint8_t) should throw.
    EXPECT_THROW(
        {
        one <<= 8u;
        },
        std::overflow_error
    );
}
