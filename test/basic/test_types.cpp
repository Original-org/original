#include <gtest/gtest.h>
import original.basic.types;


TEST(TypeTraitsTest, StdArithmeticConcept) {
    // Test integer types
    EXPECT_TRUE(original::StdArithmetic<int>);
    EXPECT_TRUE(original::StdArithmetic<short>);
    EXPECT_TRUE(original::StdArithmetic<long>);
    EXPECT_TRUE(original::StdArithmetic<char>);
    EXPECT_TRUE(original::StdArithmetic<bool>);
    EXPECT_TRUE(original::StdArithmetic<unsigned int>);

    // Test floating-point types
    EXPECT_TRUE(original::StdArithmetic<float>);
    EXPECT_TRUE(original::StdArithmetic<double>);
    EXPECT_TRUE(original::StdArithmetic<long double>);

    // Test non-arithmetic types
    EXPECT_FALSE(original::StdArithmetic<std::string>);
    EXPECT_FALSE(original::StdArithmetic<void>);
    EXPECT_FALSE(original::StdArithmetic<int*>);
    EXPECT_FALSE(original::StdArithmetic<std::vector<int>>);
}

TEST(TypeTraitsTest, StdIntegralConcept) {
    // Test integer types
    EXPECT_TRUE(original::StdIntegral<int>);
    EXPECT_TRUE(original::StdIntegral<short>);
    EXPECT_TRUE(original::StdIntegral<long>);
    EXPECT_TRUE(original::StdIntegral<char>);
    EXPECT_TRUE(original::StdIntegral<bool>);
    EXPECT_TRUE(original::StdIntegral<unsigned int>);
    EXPECT_TRUE(original::StdIntegral<std::int8_t>);
    EXPECT_TRUE(original::StdIntegral<std::uint64_t>);

    // Test non-integral types
    EXPECT_FALSE(original::StdIntegral<float>);
    EXPECT_FALSE(original::StdIntegral<double>);
    EXPECT_FALSE(original::StdIntegral<void>);
    EXPECT_FALSE(original::StdIntegral<int*>);
}

TEST(TypeTraitsTest, StdFloatingConcept) {
    // Test floating-point types
    EXPECT_TRUE(original::StdFloating<float>);
    EXPECT_TRUE(original::StdFloating<double>);
    EXPECT_TRUE(original::StdFloating<long double>);

    // Test non-floating-point types
    EXPECT_FALSE(original::StdFloating<int>);
    EXPECT_FALSE(original::StdFloating<char>);
    EXPECT_FALSE(original::StdFloating<bool>);
    EXPECT_FALSE(original::StdFloating<void>);
}

TEST(TypeTraitsTest, ConceptRelationships) {
    // Verify relationships between concepts
    static_assert(original::StdFloating<float>);
    static_assert(original::StdArithmetic<float>);

    static_assert(original::StdIntegral<int>);
    static_assert(original::StdArithmetic<int>);

    // Floating-point is not integral
    static_assert(!original::StdIntegral<float>);

    // Integral is not floating-point
    static_assert(!original::StdFloating<int>);

    // Ensure compilation succeeds
    EXPECT_TRUE(true);
}