#include <gtest/gtest.h>
#include <memory>
#include <type_traits>

import original.basic.types;

namespace {
    struct DRegular : original::Regular {
    };
    struct DMoveOnly : original::MoveOnly {
    };
    struct DCopyOnly : original::CopyOnly {
    };
    struct DImmobile : original::Immobile {
    };
    struct DStatic : original::Static {
    };
} // namespace

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
    EXPECT_FALSE(original::StdArithmetic<int *>);
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

TEST(TypeTraitsTest, TraitPolicyCopyMoveInstantiate) {
    using namespace original;

    // Regular: copyable and movable
    EXPECT_TRUE(std::is_copy_constructible_v<Regular>);
    EXPECT_TRUE(std::is_copy_assignable_v<Regular>);
    EXPECT_TRUE(std::is_move_constructible_v<Regular>);
    EXPECT_TRUE(std::is_move_assignable_v<Regular>);

    // MoveOnly: not copyable, movable
    EXPECT_FALSE(std::is_copy_constructible_v<MoveOnly>);
    EXPECT_FALSE(std::is_copy_assignable_v<MoveOnly>);
    EXPECT_TRUE(std::is_move_constructible_v<MoveOnly>);
    EXPECT_TRUE(std::is_move_assignable_v<MoveOnly>);

    // CopyOnly: copyable, move disabled but rvalue-copy allowed
    EXPECT_TRUE(std::is_copy_constructible_v<CopyOnly>);
    EXPECT_TRUE(std::is_copy_assignable_v<CopyOnly>);
    EXPECT_TRUE(std::is_move_constructible_v<CopyOnly>);   // rvalue binds to const&

    // Immobile: neither copyable nor movable
    EXPECT_FALSE(std::is_copy_constructible_v<Immobile>);
    EXPECT_FALSE(std::is_copy_assignable_v<Immobile>);
    EXPECT_FALSE(std::is_move_constructible_v<Immobile>);
    EXPECT_FALSE(std::is_move_assignable_v<Immobile>);

    // Static: immobile and not instantiable
    EXPECT_FALSE(std::is_default_constructible_v<Static>);
    EXPECT_FALSE(std::is_copy_constructible_v<Static>);
    EXPECT_FALSE(std::is_move_constructible_v<Static>);

    // Compile-time guarantees
    static_assert(!std::is_copy_constructible_v<MoveOnly>);
    static_assert(!std::is_default_constructible_v<Static>);
}

TEST(TypeTraitsTest, TraitPolicyUsage) {
    using namespace original;

    // Derived types to test actual usage (construct/copy/move)
    constexpr DRegular r1;
    DRegular r2 = r1;            // copy
    DRegular r3;
    DRegular r4 = std::move(r3); // move
    EXPECT_TRUE(std::is_copy_constructible_v<DRegular>);
    EXPECT_TRUE(std::is_move_constructible_v<DRegular>);

    DMoveOnly m1;
    DMoveOnly m2 = std::move(m1);
    EXPECT_FALSE(std::is_copy_constructible_v<DMoveOnly>);
    EXPECT_TRUE(std::is_move_constructible_v<DMoveOnly>);

    constexpr DCopyOnly c1;
    DCopyOnly c2 = c1;
    EXPECT_TRUE(std::is_copy_constructible_v<DCopyOnly>);

    DImmobile im;
    EXPECT_FALSE(std::is_copy_constructible_v<DImmobile>);
    EXPECT_FALSE(std::is_move_constructible_v<DImmobile>);

    static_assert(!std::is_default_constructible_v<DStatic>);

    // Move-only can be managed by unique_ptr
    const auto up = std::make_unique<DMoveOnly>();
    EXPECT_NE(up.get(), nullptr);
}