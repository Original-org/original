#include <gtest/gtest.h>
#include <tuple>
#include <vector>
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
    
    static_assert(std::same_as<Traits::ArgAt<0>, int>);
    static_assert(std::same_as<Traits::ArgAt<1>, double>);
    static_assert(std::same_as<Traits::ArgAt<2>, float>);
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

TEST(StructuralAlgorithms, ForAllWithMakeSequence)
{
    int sum = 0;
    auto accumulator = [&sum](auto... indices) {
        ((sum += numberLikeValue(indices)), ...);
        return sum;
    };

    const auto result = structural::forAll<int, 5>(accumulator);

    // 0 + 1 + 2 + 3 + 4 = 10
    EXPECT_EQ(result, 10);
}

TEST(StructuralAlgorithms, ForAllMultipleInvocations)
{
    int call_count = 0;
    auto counter = [&call_count](auto&&...) {
        call_count++;
        return call_count;
    };

    const auto result1 = structural::forAll(counter, makeIntegralSequence<int, 0>());
    EXPECT_EQ(result1, 1);
    const auto result2 = structural::forAll<I32, 1>(counter);
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
    static_assert(std::same_as<Traits::ArgAt<0>, std::string>);
}

TEST(StructuralUtility, IntegralSequenceEmpty)
{
    using Seq = IntegralSequence<int>;
    using Traits = IntegralSequenceTraits<Seq>;
    EXPECT_EQ(Traits::SIZE, 0);
}

TEST(StructuralAlgorithms, ForAllWithSingleElement)
{
    auto getter = [](auto x) {
        return numberLikeValue(x);
    };

    constexpr auto result = structural::forAll(getter, makeIntegralSequence<int, 1>());
    EXPECT_EQ(result, 0);
}

TEST(StructuralAlgorithms, ForAllWithEmptySequence)
{
    int count = 0;
    auto noop = [&count](auto...) {
        count++;
        return count;
    };

    const auto result = structural::forAll(noop, makeIntegralSequence<int, 0>());
    EXPECT_EQ(result, 1);
}

TEST(StructuralAlgorithms, ForAllWithLargeSequence)
{
    // Test with larger sequence (10 elements)
    int sum = 0;
    auto add_all = [&sum](auto... values) {
        ((sum += numberLikeValue(values)), ...);
        return sum;
    };

    const auto result = structural::forAll(add_all, makeIntegralSequence<int, 10>());
    // 0+1+2+3+4+5+6+7+8+9 = 45
    EXPECT_EQ(result, 45);
}

TEST(StructuralAlgorithms, ForAllWithUnsignedIntSequence)
{
    unsigned int product = 1;
    auto multiply = [&product](auto... values) {
        ((product *= (numberLikeValue(values) + 1)), ...);
        return product;
    };

    const auto result = structural::forAll<U32, 5>(multiply);
    // (0+1) * (1+1) * (2+1) * (3+1) * (4+1) = 1*2*3*4*5 = 120
    EXPECT_EQ(result, 120u);
}

TEST(StructuralAlgorithms, ForAllConstExpr)
{
    // Test that apply can be used in constexpr context
    constexpr auto compute = []
    {
        int sum = 0;
        auto add = [&sum](auto... vals) {
            ((sum += numberLikeValue(vals)), ...);
            return sum;
        };

        structural::forAll(add, makeIntegralSequence<int, 4>());
        return sum;
    };

    constexpr int result = compute();
    EXPECT_EQ(result, 6);  // 0+1+2+3 = 6
}

TEST(StructuralAlgorithms, ForAllWithComplexLambda)
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

    structural::forAll<int, 4>(complex_op);
    structural::forAll(complex_op2, makeIntegralSequence<I32, 4>());
    
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
    structural::forEach(arr1, []<Size::Type I>(IndexConstant<I>, auto&& e)
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
    using CpType2 = Couple<int, std::string>;
    static_assert(Structural<CpType2>);
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

    const auto& [mi, mf, ms] = Tuple(1, 2.0, "move");
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

namespace
{
    struct A{};
}

TEST(Tuple, StructuralTraits)
{
    using TpType = Tuple<int, float, char, double>;
    static_assert(HasStructuralSize<TpType>);
    static_assert(Structural<std::remove_reference_t<TpType>&>);
    static_assert(HasStructuralElements<TpType>);
    static_assert(Structural<TpType>);
    static_assert(TupleLike<TpType>);

    using Traits = StructuralTraits<TpType>;
    static_assert(Traits::SIZE == 4);
    static_assert(std::same_as<Traits::ElementType<0>, int>);
    static_assert(std::same_as<Traits::ElementType<1>, float>);
    static_assert(std::same_as<Traits::ElementType<2>, char>);
    static_assert(std::same_as<Traits::ElementType<3>, double>);

    using TpType2 = Tuple<int, std::vector<int>>;
    static_assert(Structural<TpType2>);
    using TpType3 = Tuple<int, A>;
    static_assert(Structural<TpType3>);
    using TpType4 = Tuple<int, A, int, std::vector<int>, char, int>;
    static_assert(Structural<TpType4>);
    using TpType5 = std::tuple<int, A, int, std::vector<int>, char, int>;
    static_assert(Structural<TpType5>);
    using TpType6 = const TpType5&;
    static_assert(Structural<TpType6>);
}

TEST(Casts, ToTupleFull)
{
    constexpr Tuple src(42, 3.14, 'A');
    constexpr auto dst = toTuple(src);

    static_assert(std::same_as<decltype(dst), const Tuple<int, double, char>>);
    EXPECT_EQ(get<0>(dst), 42);
    EXPECT_EQ(get<1>(dst), 3.14);
    EXPECT_EQ(get<2>(dst), 'A');
}

TEST(Casts, ToTupleFullRValue)
{
    constexpr auto src = Tuple(42, 3.14, 'A');
    constexpr auto dst = toTuple(src);

    static_assert(std::same_as<decltype(dst), const Tuple<int, double, char>>);
    EXPECT_EQ(get<0>(dst), 42);
    EXPECT_EQ(get<1>(dst), 3.14);
    EXPECT_EQ(get<2>(dst), 'A');
}

TEST(Casts, ToTuplePrefixCnt)
{
    constexpr Tuple src(1, 2.0, 'x', 4.0f);
    constexpr auto dst = toTuple<2>(src);

    static_assert(std::same_as<decltype(dst), const Tuple<int, double>>);
    EXPECT_EQ(get<0>(dst), 1);
    EXPECT_EQ(get<1>(dst), 2.0);
}

TEST(Casts, ToTuplePrefixCntRValue)
{
    auto src = Tuple(10, 20.0, 'z');
    auto dst = toTuple<2>(std::move(src)); // NOLINT

    static_assert(std::same_as<decltype(dst), Tuple<int, double>>);
    EXPECT_EQ(get<0>(dst), 10);
    EXPECT_EQ(get<1>(dst), 20.0);
}

TEST(Casts, ToTupleRangeStartCnt)
{
    constexpr Array<int, 5> src{0, 1, 2, 3, 4};
    constexpr auto dst = toTuple<1, 3>(src);

    static_assert(std::same_as<decltype(dst), const Tuple<int, int, int>>);
    EXPECT_EQ(get<0>(dst), 1);
    EXPECT_EQ(get<1>(dst), 2);
    EXPECT_EQ(get<2>(dst), 3);
}

TEST(Casts, ToTupleRangeStartCntRValue)
{
    Array<char, 4> src{'a', 'b', 'c', 'd'};
    auto dst = toTuple<2, 2>(std::move(src)); // NOLINT

    static_assert(std::same_as<decltype(dst), Tuple<char, char>>);
    EXPECT_EQ(get<0>(dst), 'c');
    EXPECT_EQ(get<1>(dst), 'd');
}

TEST(Casts, ToCoupleDefault)
{
    constexpr std::pair src(100, 200.5);
    constexpr auto dst = toCouple(src);

    static_assert(std::same_as<decltype(dst), const Couple<int, double>>);
    EXPECT_EQ(dst.first, 100);
    EXPECT_EQ(dst.second, 200.5);
}

TEST(Casts, ToCoupleDefaultRValue)
{
    auto src = Couple<long, std::string>(42L, "test");
    auto dst = toCouple(std::move(src));

    static_assert(std::same_as<decltype(dst), Couple<long, std::string>>);
    EXPECT_EQ(dst.first, 42L);
    EXPECT_EQ(dst.second, "test");
}

TEST(Casts, ToCoupleStart)
{
    constexpr Tuple src(1, 2.0f, 'c', true);
    constexpr auto dst = toCouple<2>(src);

    static_assert(std::same_as<decltype(dst), const Couple<char, bool>>);
    EXPECT_EQ(dst.first, 'c');
    EXPECT_EQ(dst.second, true);
}

TEST(Casts, ToCoupleStartRValue)
{
    Tuple<std::string, int, double> src("first", 10, 20.0);
    auto dst = toCouple<1>(std::move(src));

    static_assert(std::same_as<decltype(dst), Couple<int, double>>);
    EXPECT_EQ(dst.first, 10);
    EXPECT_EQ(dst.second, 20.0);
}

TEST(Casts, ToTupleEmptySource)
{
    constexpr Tuple<> empty;
    constexpr auto dst = toTuple(empty);

    static_assert(std::same_as<decltype(dst), const Tuple<>>);
    EXPECT_TRUE(dst == Tuple{});
}

TEST(Casts, ToTupleCntZero)
{
    constexpr Tuple src(1, 2.0);
    constexpr auto dst = toTuple<0>(src);

    static_assert(std::same_as<decltype(dst), const Tuple<>>);
    EXPECT_TRUE(dst == Tuple{});
}

TEST(Casts, ToTupleStartCntZero)
{
    constexpr Tuple src(5);
    [[maybe_unused]] constexpr auto dst = toTuple<0, 0>(src);

    static_assert(std::same_as<decltype(dst), const Tuple<>>);
}

TEST(Tuple, ReferenceMembersLValue)
{
    int x = 42;
    double y = 3.14;
    Tuple<int&, double&> tp(x, y);

    EXPECT_EQ(get<0>(tp), 42);
    EXPECT_EQ(get<1>(tp), 3.14);

    // Modify through reference
    get<0>(tp) = 100;
    EXPECT_EQ(x, 100);
    EXPECT_EQ(get<0>(tp), 100);

    // Const access
    const auto& ctp = tp;
    EXPECT_EQ(get<0>(ctp), 100);
    EXPECT_EQ(get<1>(ctp), 3.14);
}

TEST(Tuple, ReferenceMembersConstLValue)
{
    const int a = 10;
    const std::string b = "const ref";
    Tuple<const int&, const std::string&> tp(a, b);

    EXPECT_EQ(std::get<0>(tp), 10);
    EXPECT_EQ(std::get<1>(tp), "const ref");

    // Ensure no modification possible
    static_assert(!std::is_assignable_v<decltype(get(tp, IndexConstant<0>{})), int>, "Const reference should not be assignable");
}

TEST(Tuple, ReferenceMembersRValue)
{
    int val = 5;
    auto make_temp = [&]() -> Tuple<int&&, double> { return Tuple<int&&, double>(std::move(val), 2.5); }; // NOLINT

    auto tp = make_temp();  // Captures rvalue reference (but bound to temporary)
    EXPECT_EQ(std::get<0>(tp), 5);
    EXPECT_EQ(std::get<1>(tp), 2.5);

    // Move from rvalue reference
    int moved = std::move(std::get<0>(tp)); // NOLINT
    EXPECT_EQ(moved, 5);  // val is now moved-from, but test avoids UB by not accessing val post-move
}

TEST(Tuple, StructuredBindingWithReferences)
{
    int p = 1;
    char q = 'z';
    const Tuple<int&, char&> tp(p, q);

    auto& [ref_p, ref_q] = tp;
    EXPECT_EQ(ref_p, 1);
    EXPECT_EQ(ref_q, 'z');

    ref_p = 99;
    EXPECT_EQ(p, 99);

    const auto& [cref_p, cref_q] = tp;
    EXPECT_EQ(cref_p, 99);

    static_assert(std::same_as<decltype(cref_p), int&>,
        "const Tuple<int&> does NOT make the referenced object const");
}

TEST(Couple, ReferenceMembers)
{
    long l = 100L;
    bool b = true;
    const Couple<long&, bool&> cp(l, b);

    EXPECT_EQ(cp.first, 100L);
    EXPECT_EQ(cp.second, true);

    cp.first = 200L;
    EXPECT_EQ(l, 200L);

    const auto& ccp = cp;
    EXPECT_EQ(ccp.first, 200L);
}

TEST(Casts, ToTupleWithReferences)
{
    float f = 1.23f;
    std::string s = "ref";
    const Tuple<float&, std::string&> src(f, s);

    Tuple<float, std::string> dst = toTuple(src);  // Copies values, as toTuple uses RemoveCVRefType
    static_assert(std::same_as<decltype(dst), Tuple<float, std::string>>, "Should remove references");

    EXPECT_EQ(std::get<0>(dst), 1.23f);
    EXPECT_EQ(std::get<1>(dst), "ref");

    // Original references unaffected by dst modifications
    auto&& ref = std::get<0>(dst);
    ref = 4.56f;
    EXPECT_EQ(f, 1.23f);  // No change to original
}

TEST(Casts, ToCoupleWithReferencesRValue)
{
    int m = 7;
    double n = 8.9;
    auto src = Couple<int&, double&>(m, n);

    auto dst = toCouple(std::move(src));  // Moves values after removing references // NOLINT
    static_assert(std::same_as<decltype(dst), Couple<int, double>>);

    EXPECT_EQ(dst.first, 7);
    EXPECT_EQ(dst.second, 8.9);
}

TEST(StructuralAlgorithms, EqualWithReferenceMembers)
{
    int val1 = 42, val2 = 42;
    double d1 = 3.14, d2 = 3.14;
    Tuple<int&, double&> tp1(val1, d1);
    Tuple<int&, double&> tp2(val2, d2);

    EXPECT_TRUE(structural::equal(tp1, tp2));

    val2 = 43;
    EXPECT_FALSE(structural::equal(tp1, tp2));
}

TEST(StructuralAlgorithms, LexicographicalCompareWithReferences)
{
    char c1 = 'a', c2 = 'a';
    int i1 = 10, i2 = 20;
    Couple<char&, int&> cp1(c1, i1);
    Couple<char&, int&> cp2(c2, i2);

    EXPECT_TRUE((cp1 <=> cp2) < 0);  // 10 < 20

    i2 = 5;
    EXPECT_TRUE((cp1 <=> cp2) > 0);  // 10 > 5
}

TEST(StructuralTraits, TraitsWithReferenceTypes)
{
    using TpRef = Tuple<int&, const double&>;
    static_assert(Structural<TpRef>);
    static_assert(TupleLike<TpRef>);

    using Traits = StructuralTraits<TpRef>;
    static_assert(Traits::SIZE == 2);
    static_assert(std::same_as<Traits::ElementType<0>, int&>);
    static_assert(std::same_as<Traits::ElementType<1>, const double&>);
}

TEST(Casts, ConcatLValueLValue)
{
    constexpr Couple<int, double> lhs(1, 2.5);
    constexpr Tuple<char, bool> rhs('a', true);
    constexpr auto result = concat(lhs, rhs);
    static_assert(std::same_as<decltype(result), const Tuple<int, double, char, bool>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
    EXPECT_EQ(std::get<2>(result), 'a');
    EXPECT_EQ(std::get<3>(result), true);

    constexpr std::pair lhs2{1, 2.5};
    constexpr std::tuple rhs2{'a', true};
    constexpr auto result2 = concat(lhs2, rhs2);
    static_assert(std::same_as<decltype(result2), const Tuple<int, double, char, bool>>);
    EXPECT_EQ(std::get<0>(result2), 1);
    EXPECT_EQ(std::get<1>(result2), 2.5);
    EXPECT_EQ(std::get<2>(result2), 'a');
    EXPECT_EQ(std::get<3>(result2), true);
}

TEST(Casts, ConcatLValueRValue)
{
    constexpr Couple<long, float> lhs(10L, 3.14f);
    auto rhs = Tuple<std::string, int>("test", 42);
    auto result = concat(lhs, std::move(rhs));

    static_assert(std::same_as<decltype(result), Tuple<long, float, std::string, int>>);
    EXPECT_EQ(std::get<0>(result), 10L);
    EXPECT_EQ(std::get<1>(result), 3.14f);
    EXPECT_EQ(std::get<2>(result), "test");
    EXPECT_EQ(std::get<3>(result), 42);
}

TEST(Casts, ConcatRValueLValue)
{
    auto lhs = Tuple<bool, char>(false, 'b');
    constexpr Couple<double, long> rhs(4.5, 20L);
    auto result = concat(std::move(lhs), rhs); // NOLINT

    static_assert(std::same_as<decltype(result), Tuple<bool, char, double, long>>);
    EXPECT_EQ(std::get<0>(result), false);
    EXPECT_EQ(std::get<1>(result), 'b');
    EXPECT_EQ(std::get<2>(result), 4.5);
    EXPECT_EQ(std::get<3>(result), 20L);
}

TEST(Casts, ConcatRValueRValue)
{
    auto lhs = Couple<int, std::string>(5, "left");
    auto rhs = Tuple<float, bool>(6.7f, true);
    auto result = concat(std::move(lhs), std::move(rhs)); // NOLINT

    static_assert(std::same_as<decltype(result), Tuple<int, std::string, float, bool>>);
    EXPECT_TRUE(lhs.get<1>().empty());
    EXPECT_EQ(std::get<0>(result), 5);
    EXPECT_EQ(std::get<1>(result), "left");
    EXPECT_EQ(std::get<2>(result), 6.7f);
    EXPECT_EQ(std::get<3>(result), true);
}

TEST(Casts, ConcatEmptyStructures)
{
    constexpr Tuple<> empty1;
    constexpr Couple<int, double> non_empty(1, 2.0);
    constexpr Tuple<> empty2;

    constexpr auto result1 = concat(empty1, non_empty);
    static_assert(std::same_as<decltype(result1), const Tuple<int, double>>);
    EXPECT_EQ(std::get<0>(result1), 1);
    EXPECT_EQ(std::get<1>(result1), 2.0);

    constexpr auto result2 = concat(non_empty, empty2);
    static_assert(std::same_as<decltype(result2), const Tuple<int, double>>);
    EXPECT_EQ(std::get<0>(result2), 1);
    EXPECT_EQ(std::get<1>(result2), 2.0);

    constexpr auto result3 = concat(empty1, empty2);
    static_assert(std::same_as<decltype(result3), const Tuple<>>);
    EXPECT_TRUE(result3 == Tuple<>());
}

TEST(Casts, ConcatWithReferences)
{
    int a = 10;
    double b = 20.5;
    char c = 'c';
    bool d = false;

    Couple<int&, double&> lhs(a, b);
    Tuple<char&, bool&> rhs(c, d);

    auto result = concat(lhs, rhs);
    static_assert(std::same_as<decltype(result), Tuple<int, double, char, bool>>);
    EXPECT_EQ(std::get<0>(result), 10);
    EXPECT_EQ(std::get<1>(result), 20.5);
    EXPECT_EQ(std::get<2>(result), 'c');
    EXPECT_EQ(std::get<3>(result), false);

    // Modifications to result do not affect originals
    std::get<0>(result) = 99;
    EXPECT_EQ(a, 10);
}

TEST(Casts, OperatorPlusCoupleCouple)
{
    constexpr Couple<int, double> lhs(1, 2.5);
    constexpr Couple<char, bool> rhs('a', true);
    constexpr auto result = lhs + rhs;

    static_assert(std::same_as<decltype(result), const Tuple<int, double, char, bool>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
    EXPECT_EQ(std::get<2>(result), 'a');
    EXPECT_EQ(std::get<3>(result), true);
}

TEST(Casts, OperatorPlusTupleTuple)
{
    constexpr Tuple<long, float> lhs(10L, 3.14f);
    const Tuple<std::string, int> rhs("test", 42);
    const auto result = lhs + rhs;

    static_assert(std::same_as<decltype(result), const Tuple<long, float, std::string, int>>);
    EXPECT_EQ(std::get<0>(result), 10L);
    EXPECT_EQ(std::get<1>(result), 3.14f);
    EXPECT_EQ(std::get<2>(result), "test");
    EXPECT_EQ(std::get<3>(result), 42);
}

TEST(Casts, OperatorPlusCoupleTuple)
{
    constexpr Couple<bool, char> lhs(false, 'b');
    constexpr Tuple<double, long> rhs(4.5, 20L);
    constexpr auto result = lhs + rhs;

    static_assert(std::same_as<decltype(result), const Tuple<bool, char, double, long>>);
    EXPECT_EQ(std::get<0>(result), false);
    EXPECT_EQ(std::get<1>(result), 'b');
    EXPECT_EQ(std::get<2>(result), 4.5);
    EXPECT_EQ(std::get<3>(result), 20L);
}

TEST(Casts, OperatorPlusTupleCouple)
{
    const Tuple<int, std::string> lhs(5, "left");
    constexpr Couple<float, bool> rhs(6.7f, true);
    const auto result = lhs + rhs;

    static_assert(std::same_as<decltype(result), const Tuple<int, std::string, float, bool>>);
    EXPECT_EQ(std::get<0>(result), 5);
    EXPECT_EQ(std::get<1>(result), "left");
    EXPECT_EQ(std::get<2>(result), 6.7f);
    EXPECT_EQ(std::get<3>(result), true);
}

TEST(Casts, OperatorPlusRValueSemantics)
{
    auto lhs = Couple<int, double>(1, 2.5);
    auto rhs = Tuple<char, bool>('a', true);
    auto result = std::move(lhs) + std::move(rhs); // NOLINT

    static_assert(std::same_as<decltype(result), Tuple<int, double, char, bool>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
    EXPECT_EQ(std::get<2>(result), 'a');
    EXPECT_EQ(std::get<3>(result), true);
}

TEST(Casts, OperatorPlusWithEmpty)
{
    constexpr Couple<int, double> non_empty(1, 2.0);
    constexpr Tuple<> empty;

    constexpr auto result1 = non_empty + empty;
    static_assert(std::same_as<decltype(result1), const Tuple<int, double>>);
    EXPECT_EQ(std::get<0>(result1), 1);
    EXPECT_EQ(std::get<1>(result1), 2.0);

    constexpr auto result2 = empty + non_empty;
    static_assert(std::same_as<decltype(result2), const Tuple<int, double>>);
    EXPECT_EQ(std::get<0>(result2), 1);
    EXPECT_EQ(std::get<1>(result2), 2.0);
}

TEST(Casts, OperatorPlusChainThreeLValues)
{
    constexpr Couple<int, double> first(1, 2.5);
    constexpr Tuple<char, bool> second('a', true);
    constexpr Couple<long, float> third(10L, 3.14f);
    constexpr auto result = first + second + third;

    static_assert(std::same_as<decltype(result), const Tuple<int, double, char, bool, long, float>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
    EXPECT_EQ(std::get<2>(result), 'a');
    EXPECT_EQ(std::get<3>(result), true);
    EXPECT_EQ(std::get<4>(result), 10L);
    EXPECT_EQ(std::get<5>(result), 3.14f);
}

TEST(Casts, OperatorPlusChainMixedRValues)
{
    auto first = Couple<std::string, int>("left", 42);
    constexpr Tuple<double, char> second(4.5, 'b');
    auto third = Couple<bool, long>(false, 100L);
    auto result = std::move(first) + second + std::move(third); // NOLINT

    static_assert(std::same_as<decltype(result), Tuple<std::string, int, double, char, bool, long>>);
    EXPECT_EQ(std::get<0>(result), "left");
    EXPECT_EQ(std::get<1>(result), 42);
    EXPECT_EQ(std::get<2>(result), 4.5);
    EXPECT_EQ(std::get<3>(result), 'b');
    EXPECT_EQ(std::get<4>(result), false);
    EXPECT_EQ(std::get<5>(result), 100L);
}

TEST(Casts, OperatorPlusChainWithEmpty)
{
    constexpr Tuple<> empty1;
    constexpr Couple<int, double> middle(1, 2.5);
    constexpr Tuple<> empty2;
    constexpr auto result = empty1 + middle + empty2;

    static_assert(std::same_as<decltype(result), const Tuple<int, double>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
}

TEST(Casts, OperatorPlusChainFourMixedTypes)
{
    constexpr Couple<int, bool> a(1, true);
    auto b = Tuple<double, char>(3.14, 'c');
    const Couple<long, std::string> c(5L, "mid");
    auto d = Tuple<float>(6.7f);
    const auto result = a + std::move(b) + c + std::move(d); // NOLINT

    static_assert(std::same_as<decltype(result), const Tuple<int, bool, double, char, long, std::string, float>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), true);
    EXPECT_EQ(std::get<2>(result), 3.14);
    EXPECT_EQ(std::get<3>(result), 'c');
    EXPECT_EQ(std::get<4>(result), 5L);
    EXPECT_EQ(std::get<5>(result), "mid");
    EXPECT_EQ(std::get<6>(result), 6.7f);
}

TEST(Casts, OperatorPlusChainWithReferences)
{
    int x = 10;
    double y = 20.5;
    char z = 'z';
    bool w = false;
    long v = 30L;
    float u = 4.2f;

    Couple<int&, double&> first(x, y);
    Tuple<char&, bool&> second(z, w);
    Couple<long&, float&> third(v, u);

    auto result = first + second + third;
    static_assert(std::same_as<decltype(result), Tuple<int, double, char, bool, long, float>>);
    EXPECT_EQ(std::get<0>(result), 10);
    EXPECT_EQ(std::get<1>(result), 20.5);
    EXPECT_EQ(std::get<2>(result), 'z');
    EXPECT_EQ(std::get<3>(result), false);
    EXPECT_EQ(std::get<4>(result), 30L);
    EXPECT_EQ(std::get<5>(result), 4.2f);

    // Modifications to result do not affect originals
    std::get<0>(result) = 99;
    EXPECT_EQ(x, 10);
}

TEST(Casts, OperatorPlusChainMixedReferencesAndValues)
{
    int p = 1;
    const Couple<int&, double> first(p, 2.5);
    bool w = true;
    float u = 3.14f;
    auto second = Tuple<char, bool&>( 'a', w);
    const Couple<long, float&> third(3L, u);

    auto result = first + std::move(second) + third; // NOLINT
    static_assert(std::same_as<decltype(result), Tuple<int, double, char, bool, long, float>>);
    EXPECT_EQ(std::get<0>(result), 1);
    EXPECT_EQ(std::get<1>(result), 2.5);
    EXPECT_EQ(std::get<2>(result), 'a');
    EXPECT_EQ(std::get<3>(result), true);
    EXPECT_EQ(std::get<4>(result), 3L);
    EXPECT_EQ(std::get<5>(result), 3.14f);

    // Check reference effects
    p = 100;
    EXPECT_EQ(std::get<0>(first), 100);  // first holds reference
    EXPECT_EQ(std::get<0>(result), 1);   // result copied value
}

TEST(BuiltinArray, BasicTraits) {
    using BuiltinArrType = int[3];
    static_assert(Structural<BuiltinArrType>);
    static_assert(!CoupleLike<BuiltinArrType>);
    static_assert(TupleLike<BuiltinArrType>);
    constexpr BuiltinArrType a = {1, 2, 3};
    EXPECT_EQ(get<0>(a), 1);
    EXPECT_EQ(get<1>(a), 2);
    EXPECT_EQ(get<2>(a), 3);
}

TEST(BuiltinArray, StructuralAlgorithmCompatibility) {
    using BuiltinArrType = int[5];
    constexpr BuiltinArrType a = {0, 1, 2, 3, 4};
    structural::forEach(a,
        []<Size::Type I>(IndexConstant<I>, const auto& e)
        {
            EXPECT_EQ(I, e);
        }
    );
}