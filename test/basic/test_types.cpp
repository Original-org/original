#include <gtest/gtest.h>
import original.basic.types;


TEST(TypeTraitsTest, StdArithmeticConcept) {
    // 测试整数类型
    EXPECT_TRUE(original::StdArithmetic<int>);
    EXPECT_TRUE(original::StdArithmetic<short>);
    EXPECT_TRUE(original::StdArithmetic<long>);
    EXPECT_TRUE(original::StdArithmetic<char>);
    EXPECT_TRUE(original::StdArithmetic<bool>);
    EXPECT_TRUE(original::StdArithmetic<unsigned int>);
    
    // 测试浮点类型
    EXPECT_TRUE(original::StdArithmetic<float>);
    EXPECT_TRUE(original::StdArithmetic<double>);
    EXPECT_TRUE(original::StdArithmetic<long double>);
    
    // 测试非算术类型
    EXPECT_FALSE(original::StdArithmetic<std::string>);
    EXPECT_FALSE(original::StdArithmetic<void>);
    EXPECT_FALSE(original::StdArithmetic<int*>);
    EXPECT_FALSE(original::StdArithmetic<std::vector<int>>);
}

TEST(TypeTraitsTest, StdIntegralConcept) {
    // 测试整数类型
    EXPECT_TRUE(original::StdIntegral<int>);
    EXPECT_TRUE(original::StdIntegral<short>);
    EXPECT_TRUE(original::StdIntegral<long>);
    EXPECT_TRUE(original::StdIntegral<char>);
    EXPECT_TRUE(original::StdIntegral<bool>);
    EXPECT_TRUE(original::StdIntegral<unsigned int>);
    EXPECT_TRUE(original::StdIntegral<std::int8_t>);
    EXPECT_TRUE(original::StdIntegral<std::uint64_t>);
    
    // 测试非整数类型
    EXPECT_FALSE(original::StdIntegral<float>);
    EXPECT_FALSE(original::StdIntegral<double>);
    EXPECT_FALSE(original::StdIntegral<void>);
    EXPECT_FALSE(original::StdIntegral<int*>);
}

TEST(TypeTraitsTest, StdFloatingConcept) {
    // 测试浮点类型
    EXPECT_TRUE(original::StdFloating<float>);
    EXPECT_TRUE(original::StdFloating<double>);
    EXPECT_TRUE(original::StdFloating<long double>);
    
    // 测试非浮点类型
    EXPECT_FALSE(original::StdFloating<int>);
    EXPECT_FALSE(original::StdFloating<char>);
    EXPECT_FALSE(original::StdFloating<bool>);
    EXPECT_FALSE(original::StdFloating<void>);
}

TEST(TypeTraitsTest, IsStdArithmeticConstant) {
    EXPECT_TRUE(original::IsStdArithmetic<int>);
    EXPECT_TRUE(original::IsStdArithmetic<double>);
    EXPECT_FALSE(original::IsStdArithmetic<std::string>);
    EXPECT_FALSE(original::IsStdArithmetic<void*>);
}

TEST(TypeTraitsTest, IsStdIntegralConstant) {
    EXPECT_TRUE(original::IsStdIntegral<int>);
    EXPECT_TRUE(original::IsStdIntegral<unsigned long>);
    EXPECT_FALSE(original::IsStdIntegral<float>);
    EXPECT_FALSE(original::IsStdIntegral<void>);
}

TEST(TypeTraitsTest, IsStdFloatingConstant) {
    EXPECT_TRUE(original::IsStdFloating<float>);
    EXPECT_TRUE(original::IsStdFloating<long double>);
    EXPECT_FALSE(original::IsStdFloating<int>);
    EXPECT_FALSE(original::IsStdFloating<char>);
}

TEST(TypeTraitsTest, ConceptRelationships) {
    // 验证概念之间的关系
    static_assert(original::StdFloating<float>);
    static_assert(original::StdArithmetic<float>);
    
    static_assert(original::StdIntegral<int>);
    static_assert(original::StdArithmetic<int>);
    
    // 浮点不是整数
    static_assert(!original::StdIntegral<float>);
    
    // 整数不是浮点
    static_assert(!original::StdFloating<int>);
    
    // 确保编译通过
    EXPECT_TRUE(true);
}