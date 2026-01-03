#include <gtest/gtest.h>
import original.basic.tuple;
import original.basic.number;
import original.basic.array;

using namespace original;

TEST(TupleUtility, ArgsTraits)
{
    using Traits = ArgsTraits<int, double, float>;
    EXPECT_EQ(Traits::SIZE, 3);
    
    static_assert(std::same_as<Traits::ArgsAt<0>, int>);
    static_assert(std::same_as<Traits::ArgsAt<1>, double>);
    static_assert(std::same_as<Traits::ArgsAt<2>, float>);
}

TEST(TupleUtility, IntegralSequenceTraits)
{
    using Seq = IntegralSequence<int, 1, 2, 3>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 3);
    static_assert(std::same_as<Traits::NumberType, int>);
    static_assert(std::same_as<Traits::ValueType, IntegralConstant<int, 0>::ValueType>);
}

TEST(TupleUtility, MakeIntegralSequence)
{
    using Seq = MakeIntegralSequence<int, 4>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 4);
}

TEST(TupleUtility, ForAllWithMakeSequence)
{
    int sum = 0;
    auto accumulator = [&sum](auto... indices) {
        ((sum += numberLikeValue(indices)), ...);
        return sum;
    };

    const auto result = forAll(accumulator, MakeIntegralSequence<int, 5>());

    // 0 + 1 + 2 + 3 + 4 = 10
    EXPECT_EQ(result, 10);
}

TEST(TupleUtility, ForAllMultipleInvocations)
{
    int call_count = 0;
    auto counter = [&call_count](auto&&...) {
        call_count++;
        return call_count;
    };

    auto result1 = forAll(counter, MakeIntegralSequence<int, 0>());
    EXPECT_EQ(result1, 1);
    auto result2 = forAll(counter, MakeIntegralSequence<int, 1>());
    EXPECT_EQ(result2, 2);
}

TEST(TupleUtility, ArgsTraitsEmpty)
{
    using Traits = ArgsTraits<>;
    EXPECT_EQ(Traits::SIZE, 0);
}

TEST(TupleUtility, ArgsTraitsSingle)
{
    using Traits = ArgsTraits<std::string>;
    EXPECT_EQ(Traits::SIZE, 1);
    static_assert(std::same_as<Traits::ArgsAt<0>, std::string>);
}

TEST(TupleUtility, IntegralSequenceEmpty)
{
    using Seq = IntegralSequence<int>;
    using Traits = IntegralSequenceTraits<Seq>;
    EXPECT_EQ(Traits::SIZE, 0);
}

TEST(TupleUtility, ForAllWithSingleElement)
{
    auto getter = [](auto x) {
        return numberLikeValue(x);
    };
    
    auto result = forAll(getter, MakeIntegralSequence<int, 1>());
    EXPECT_EQ(result, 0);
}

TEST(TupleUtility, ForAllWithEmptySequence)
{
    int count = 0;
    auto noop = [&count](auto...) {
        count++;
        return count;
    };
    
    auto result = forAll(noop, MakeIntegralSequence<int, 0>());
    EXPECT_EQ(result, 1);
}

TEST(TupleUtility, ForAllWithLargeSequence)
{
    // Test with larger sequence (10 elements)
    int sum = 0;
    auto add_all = [&sum](auto... values) {
        ((sum += numberLikeValue(values)), ...);
        return sum;
    };
    
    auto result = forAll(add_all, MakeIntegralSequence<int, 10>());
    // 0+1+2+3+4+5+6+7+8+9 = 45
    EXPECT_EQ(result, 45);
}

TEST(TupleUtility, ForAllWithUnsignedIntSequence)
{
    unsigned int product = 1;
    auto multiply = [&product](auto... values) {
        ((product *= (numberLikeValue(values) + 1)), ...);
        return product;
    };
    
    auto result = forAll(multiply, MakeIntegralSequence<unsigned int, 5>());
    // (0+1) * (1+1) * (2+1) * (3+1) * (4+1) = 1*2*3*4*5 = 120
    EXPECT_EQ(result, 120u);
}

TEST(TupleUtility, ForAllConstExpr)
{
    // Test that apply can be used in constexpr context
    constexpr auto compute = []
    {
        int sum = 0;
        auto add = [&sum](auto... vals) {
            ((sum += numberLikeValue(vals)), ...);
            return sum;
        };
        
        forAll(add, MakeIntegralSequence<int, 4>());
        return sum;
    };
    
    const int result = compute();
    EXPECT_EQ(result, 6);  // 0+1+2+3 = 6
}

TEST(TupleUtility, ForAllWithComplexLambda)
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
    
    forAll(complex_op, MakeIntegralSequence<int, 4>());
    forAll(complex_op2, MakeIntegralSequence<I32, 4>());
    
    EXPECT_EQ(acc1.sum, 6);      // 0+1+2+3 = 6
    EXPECT_EQ(acc1.product, 24);  // 1*2*3*4 = 24
    EXPECT_EQ(acc1.count, 4);

    EXPECT_EQ(acc2.sum, 6);
    EXPECT_EQ(acc2.product, 24);
    EXPECT_EQ(acc2.count, 4);
}

TEST(TupleUtility, IntegralSequenceTraitsLarge)
{
    using Seq = MakeIntegralSequence<long long, 8>;
    using Traits = IntegralSequenceTraits<Seq>;
    
    EXPECT_EQ(Traits::SIZE, 8);
    static_assert(std::same_as<Traits::NumberType, long long>);
}
