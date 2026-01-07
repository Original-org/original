#include <gtest/gtest.h>
#include <tuple>
import original.basic.array;
import original.basic.structural;
import original.basic.number;
import original.basic.range;
import original.basic.types;


using namespace original;
using namespace original::range;

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

TEST(StructuralTraits, StructuralConceptTest)
{
    using ArrayType1 = Array<int, 2>;
    using ArrayType2 = Array<int, 0>;
    using StdTuple = std::tuple<int, float, int>;
    static_assert(HasStructuralSize<ArrayType1>);
    static_assert(HasStructuralElements<ArrayType1>);
    static_assert(Structural<ArrayType1>);
    static_assert(HasStructuralSize<StdTuple>);
    static_assert(HasStructuralElements<StdTuple>);
    static_assert(Structural<StdTuple>);
    static_assert(Structural<ArrayType2>);
    static_assert(!CoupleLike<ArrayType2>);
    static_assert(!TupleLike<ArrayType2>);
    static_assert(CoupleLike<ArrayType1>);
    static_assert(TupleLike<StdTuple>);

    static_assert(SameType<StructuralTraits<StdTuple>::Type, StdTuple>);
    static_assert(SameType<StructuralTraits<StdTuple>::ElementType<0>, int>);
    static_assert(SameType<StructuralTraits<StdTuple>::ElementType<1>, float>);
    static_assert(SameType<StructuralTraits<StdTuple>::ElementType<2>, int>);
    static_assert(StructuralTraits<StdTuple>::SIZE == 3);
}

TEST(StructuralTraits, StructuralForEach)
{
    Array<int, 5> arr1 {-1, 0, 1, 2, 3};
    forEach(arr1, [](auto&& e)
    {
        e += 1;
    });
    for (const auto view = arr1 | enumerate();
         auto&& [index, val]: view)
    {
        EXPECT_EQ(index.value(), val);
    }
}

TEST(Couple, DefaultConstructor)
{
    constexpr Couple<int, double> cp;
    EXPECT_EQ(cp.first, 0);
    EXPECT_EQ(cp.second, 0.0);
}

TEST(Couple, ParameterizedConstructor)
{
    static constexpr int a = 1;
    static constexpr double b = 2.5;
    constexpr Couple cp(a, b);
    EXPECT_EQ(cp.first, 1);
    EXPECT_EQ(cp.second, 2.5);
}

TEST(Couple, CopyAndMoveSemantics)
{
    Couple<std::string, int> cp1("hello", 42);
    const auto cp2 = cp1; // copy
    const auto cp3 = std::move(cp1); // move

    EXPECT_EQ(cp2.first, "hello");
    EXPECT_EQ(cp2.second, 42);
    EXPECT_EQ(cp3.first, "hello");
    EXPECT_EQ(cp3.second, 42);
}

TEST(Couple, EqualityComparison)
{
    constexpr Couple cp1(1, 2.0f);
    constexpr Couple cp2(1, 2.0f);
    constexpr Couple cp3(2, 2.0f);
    constexpr Couple cp4{2.0f, 2.0f};

    EXPECT_TRUE(cp1 == cp2);
    EXPECT_FALSE(cp1 == cp3);
    EXPECT_TRUE(cp3 == cp4);
}

TEST(Couple, ThreeWayComparison)
{
    constexpr Couple cp1(1, 'a');
    constexpr Couple cp2(1, 'b');
    constexpr Couple cp3(2, 'a');
    constexpr Couple cp4{2, static_cast<int>('a')};

    EXPECT_LT(cp1, cp2);
    EXPECT_LT(cp1, cp3);
    EXPECT_TRUE((cp1 <=> cp4) < 0);
}

TEST(Couple, GetFunction)
{
    Couple cp1(42L, true);
    EXPECT_EQ(original::get<0>(cp1), 42L);
    EXPECT_EQ(original::get<1>(cp1), true);
    const auto& cref = cp1;
    EXPECT_EQ(original::get<0>(cref), 42L);
    EXPECT_EQ(original::get<1>(cref), true);
    EXPECT_EQ(original::get<0>(std::move(cp1)), 42L); // NOLINT
    EXPECT_EQ(original::get<1>(std::move(cp1)), true); // NOLINT
}

TEST(Couple, StructuralBinding)
{
    constexpr Couple cp1(1, 2.0f);
    auto&& [i, f] = cp1;
    EXPECT_EQ(i, 1);
    EXPECT_EQ(f, 2.0f);
}

TEST(Couple, StructuralTraits)
{
    using CpType = Couple<int, float>;
    static_assert(HasStructuralSize<CpType>);
    static_assert(HasStructuralElements<CpType>);
    static_assert(Structural<CpType>);
    static_assert(CoupleLike<CpType>);
    static_assert(TupleLike<CpType>);
    using Traits = StructuralTraits<const CpType>;
    EXPECT_EQ(Traits::SIZE, 2);
    static_assert(std::same_as<Traits::Type, CpType>);
    static_assert(std::same_as<Traits::ElementType<0>, int>);
    static_assert(std::same_as<Traits::ElementType<1>, float>);
}

TEST(Tuple, DefaultConstructorAndGet)
{
    using TupleType = Tuple<int, double, float>;
    constexpr TupleType tp{};
    EXPECT_EQ(tp.get<0>(), 0);
    EXPECT_EQ(tp.get<1>(), 0.0);
    EXPECT_EQ(tp.get<2>(), 0.0f);

    constexpr TupleType tp_constexpr{};
    static_assert(tp_constexpr.get<0>() == 0);
    static_assert(tp_constexpr.get<1>() == 0.0);
    static_assert(tp_constexpr.get<2>() == 0.0f);
}

TEST(Tuple, ParameterizedConstructor)
{
    static constexpr int i = 42;
    static constexpr double d = 3.14;
    static constexpr char c = 'A';
    constexpr Tuple tp(i, d, c);

    EXPECT_EQ(tp.get<0>(), 42);
    EXPECT_EQ(tp.get<1>(), 3.14);
    EXPECT_EQ(tp.get<2>(), 'A');

    static_assert(tp.get<0>() == 42);
    static_assert(tp.get<1>() == 3.14);
    static_assert(tp.get<2>() == 'A');
    static_assert(SameType<decltype(tp.get<0>()), const int&>);
}

TEST(Tuple, CopyAndMoveSemantics)
{
    Tuple<std::string, int, bool> tp1("test", 100, true);

    auto tp2 = tp1;
    EXPECT_EQ(tp2.get<0>(), "test");
    EXPECT_EQ(tp2.get<1>(), 100);
    EXPECT_EQ(tp2.get<2>(), true);

    auto tp3 = std::move(tp1);
    EXPECT_EQ(tp3.get<0>(), "test");
    EXPECT_EQ(tp3.get<1>(), 100);
    EXPECT_EQ(tp3.get<2>(), true);

    Tuple<std::string, int, bool> tp4;
    tp4 = tp3;
    EXPECT_EQ(tp4.get<0>(), "test");

    Tuple<std::string, int, bool> tp5;
    tp5 = std::move(tp3);
    EXPECT_EQ(tp5.get<0>(), "test");
}

TEST(Tuple, EqualityComparison)
{
    constexpr Tuple tp1(1, 2.0f, 'x');
    constexpr Tuple tp2(1, 2.0f, 'x');
    constexpr Tuple tp3(1, 2.0f, 'y');
    constexpr Tuple tp4(2, 2.0f, 'x');

    EXPECT_TRUE(tp1 == tp2);
    EXPECT_FALSE(tp1 == tp3);
    EXPECT_FALSE(tp1 == tp4);

    constexpr Tuple<long, float, int> tp5(1L, 2.0f, 'x');
    EXPECT_TRUE(tp1 == tp5);
}

TEST(Tuple, ThreeWayComparison)
{
    constexpr Tuple tp1(1, 'a', 10.0);
    constexpr Tuple tp2(1, 'b', 10.0);
    constexpr Tuple tp3(2, 'a', 10.0);
    constexpr Tuple tp4(1, 'a', 20.0);

    EXPECT_TRUE((tp1 <=> tp2) < 0);  // 'a' < 'b'
    EXPECT_TRUE((tp1 <=> tp3) < 0);  // 1 < 2
    EXPECT_TRUE((tp1 <=> tp4) < 0);  // 10.0 < 20.0
    EXPECT_TRUE((tp2 <=> tp1) > 0);
    EXPECT_TRUE((tp1 <=> tp1) == 0);

    constexpr Tuple tp5(1L, 'a', 10.0);
    EXPECT_TRUE((tp1 <=> tp5) == 0);
}

TEST(Tuple, GetFunction)
{
    Tuple<long, bool, std::string> tp(42L, true, "hello");

    EXPECT_EQ(original::get<0>(tp), 42L);
    EXPECT_EQ(original::get<1>(tp), true);
    EXPECT_EQ(original::get<2>(tp), "hello");

    const auto& cref = tp;
    EXPECT_EQ(original::get<0>(cref), 42L);

    EXPECT_EQ(original::get<0>(std::move(tp)), 42L);
    EXPECT_EQ(original::get<2>(std::move(tp)), "hello");
}

TEST(Tuple, StructuredBinding)
{
    constexpr Tuple tp(10, 3.14f, "text");
    auto&& [i, f, s] = tp;

    EXPECT_EQ(i, 10);
    EXPECT_EQ(f, 3.14f);
    EXPECT_STREQ(s, "text");

    const auto& ctp = tp;
    auto&& [ci, cf, cs] = ctp;
    EXPECT_EQ(ci, 10);

    auto&& [mi, mf, ms] = Tuple(1, 2.0, "move");
    EXPECT_EQ(mi, 1);
}

TEST(Tuple, EmptyTuple)
{
    constexpr Tuple<> empty1;
    constexpr Tuple<> empty2;

    EXPECT_TRUE(empty1 == empty2);

    EXPECT_TRUE((empty1 <=> empty2) == 0);
}

TEST(Tuple, SingleElementTuple)
{
    constexpr Tuple single1(42);
    constexpr Tuple single2(42);
    constexpr Tuple single3(42L);

    EXPECT_EQ(single1.get<0>(), 42);
    EXPECT_TRUE(single1 == single2);
    EXPECT_TRUE(single1 == single3);

    auto&& [v] = single1;
    EXPECT_EQ(v, 42);
}

TEST(Tuple, StructuralTraits)
{
    using TpType = Tuple<int, float, char, double>;
    static_assert(HasStructuralSize<TpType>);
    static_assert(HasStructuralElements<TpType>);
    static_assert(Structural<TpType>);
    static_assert(TupleLike<TpType>);

    using Traits = StructuralTraits<TpType>;
    static_assert(Traits::SIZE == 4);
    static_assert(std::same_as<Traits::ElementType<0>, int>);
    static_assert(std::same_as<Traits::ElementType<1>, float>);
    static_assert(std::same_as<Traits::ElementType<2>, char>);
    static_assert(std::same_as<Traits::ElementType<3>, double>);
}