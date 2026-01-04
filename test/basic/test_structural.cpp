#include <gtest/gtest.h>
import original.basic.structural;
import original.basic.number;
import original.basic.array;

using namespace original;

TEST(StructuralUtility, ArgsTraits)
{
    using Traits = ArgsTraits<int, double, float>;
    EXPECT_EQ(Traits::SIZE, 3);
    
    static_assert(std::same_as<Traits::ArgsAt<0>, int>);
    static_assert(std::same_as<Traits::ArgsAt<1>, double>);
    static_assert(std::same_as<Traits::ArgsAt<2>, float>);
}

TEST(StructuralUtility, IntegralSequenceTraits)
{
    using Seq = IntegralSequence<int, 1, 2, 3>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 3);
    static_assert(std::same_as<Traits::NumberType, int>);
    static_assert(std::same_as<Traits::ValueType, IntegralConstant<int, 0>::ValueType>);
}

TEST(StructuralUtility, MakeIntegralSequence)
{
    using Seq = IntegralSequenceType<int, 0, 4>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 4);
}

TEST(StructuralUtility, ForAllWithMakeSequence)
{
    int sum = 0;
    auto accumulator = [&sum](auto... indices) {
        ((sum += numberLikeValue(indices)), ...);
        return sum;
    };

    const auto result = forAll<int, 5>(accumulator);

    // 0 + 1 + 2 + 3 + 4 = 10
    EXPECT_EQ(result, 10);
}

TEST(StructuralUtility, ForAllMultipleInvocations)
{
    int call_count = 0;
    auto counter = [&call_count](auto&&...) {
        call_count++;
        return call_count;
    };

    const auto result1 = forAll(counter, makeIntegralSequence<int, 0>());
    EXPECT_EQ(result1, 1);
    const auto result2 = forAll<I32, 1>(counter);
    EXPECT_EQ(result2, 2);
}

TEST(StructuralUtility, ArgsTraitsEmpty)
{
    using Traits = ArgsTraits<>;
    EXPECT_EQ(Traits::SIZE, 0);
}

TEST(StructuralUtility, ArgsTraitsSingle)
{
    using Traits = ArgsTraits<std::string>;
    EXPECT_EQ(Traits::SIZE, 1);
    static_assert(std::same_as<Traits::ArgsAt<0>, std::string>);
}

TEST(StructuralUtility, IntegralSequenceEmpty)
{
    using Seq = IntegralSequence<int>;
    using Traits = IntegralSequenceTraits<Seq>;
    EXPECT_EQ(Traits::SIZE, 0);
}

TEST(StructuralUtility, ForAllWithSingleElement)
{
    auto getter = [](auto x) {
        return numberLikeValue(x);
    };

    constexpr auto result = forAll(getter, makeIntegralSequence<int, 1>());
    EXPECT_EQ(result, 0);
}

TEST(StructuralUtility, ForAllWithEmptySequence)
{
    int count = 0;
    auto noop = [&count](auto...) {
        count++;
        return count;
    };

    const auto result = forAll(noop, makeIntegralSequence<int, 0>());
    EXPECT_EQ(result, 1);
}

TEST(StructuralUtility, ForAllWithLargeSequence)
{
    // Test with larger sequence (10 elements)
    int sum = 0;
    auto add_all = [&sum](auto... values) {
        ((sum += numberLikeValue(values)), ...);
        return sum;
    };

    const auto result = forAll(add_all, makeIntegralSequence<int, 10>());
    // 0+1+2+3+4+5+6+7+8+9 = 45
    EXPECT_EQ(result, 45);
}

TEST(StructuralUtility, ForAllWithUnsignedIntSequence)
{
    unsigned int product = 1;
    auto multiply = [&product](auto... values) {
        ((product *= (numberLikeValue(values) + 1)), ...);
        return product;
    };

    const auto result = forAll<U32, 5>(multiply);
    // (0+1) * (1+1) * (2+1) * (3+1) * (4+1) = 1*2*3*4*5 = 120
    EXPECT_EQ(result, 120u);
}

TEST(StructuralUtility, ForAllConstExpr)
{
    // Test that apply can be used in constexpr context
    constexpr auto compute = []
    {
        int sum = 0;
        auto add = [&sum](auto... vals) {
            ((sum += numberLikeValue(vals)), ...);
            return sum;
        };
        
        forAll(add, makeIntegralSequence<int, 4>());
        return sum;
    };

    constexpr int result = compute();
    EXPECT_EQ(result, 6);  // 0+1+2+3 = 6
}

TEST(StructuralUtility, ForAllWithComplexLambda)
{
    // Test with a more complex lambda that combines multiple operations
    struct Accumulator {
        int sum = 0;
        int product = 1;
        int count = 0;
    };
    
    Accumulator acc1;
    auto complex_op = [&acc1](auto... indices) {
        ((acc1.sum += numberLikeValue(indices),
          acc1.product *= (numberLikeValue(indices) + 1),
          acc1.count++), ...);
    };

    Accumulator acc2;
    auto complex_op2 = [&acc2](auto... indices)
    {
        ((acc2.sum += numberLikeValue(indices),
        acc2.product *= (numberLikeValue(indices) + 1),
        acc2.count++), ...);
    };
    
    forAll<int, 4>(complex_op);
    forAll(complex_op2, makeIntegralSequence<I32, 4>());
    
    EXPECT_EQ(acc1.sum, 6);      // 0+1+2+3 = 6
    EXPECT_EQ(acc1.product, 24);  // 1*2*3*4 = 24
    EXPECT_EQ(acc1.count, 4);

    EXPECT_EQ(acc2.sum, 6);
    EXPECT_EQ(acc2.product, 24);
    EXPECT_EQ(acc2.count, 4);
}

TEST(StructuralUtility, IntegralSequenceTraitsLarge)
{
    using Seq = DefaultIntegralSequenceType<long long, 8>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 8);
    static_assert(std::same_as<Traits::NumberType, long long>);
}
