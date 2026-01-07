module;
#include <limits>
export module original.basic.number.traits;
import original.basic.types;
import original.basic.number.impl;


namespace original::details
{
    /**
     * @brief Primary template: not a number wrapper.
     * @tparam T Type to test.
     * @note Intentionally left incomplete to enforce explicit specializations.
     */
    template<typename T>
    struct NumberTrait;

    /**
     * @brief Integer<T> specialization.
     * @tparam T Underlying integral type.
     * @note Provides Type alias to the underlying integral type.
     */
    template<StdIntegral T>
    struct NumberTrait<Integer<T>>
    {
        using Type = T;  ///< Underlying integral type.
    };

    /**
     * @brief Floating<T> specialization.
     * @tparam T Underlying floating-point type.
     * @note Provides Type alias to the underlying floating-point type.
     */
    template<StdFloating T>
    struct NumberTrait<Floating<T>>
    {
        using Type = T;  ///< Underlying floating-point type.
    };

    /**
     * @brief Implementation helper for NumberLikeType.
     * @tparam T Type to convert.
     * @tparam IsNumber Whether T is a number wrapper.
     */
    template<typename T, bool IsNumber>
    struct NumberLikeTypeImpl;

    /**
     * @brief Number wrapper specialization.
     * @tparam T Number wrapper type.
     */
    template<typename T>
    struct NumberLikeTypeImpl<T, true>
    {
        using type = NumberTrait<T>::Type;  ///< Underlying type of the number wrapper.
    };

    /**
     * @brief Arithmetic type specialization.
     * @tparam T Standard arithmetic type.
     */
    template<typename T>
    struct NumberLikeTypeImpl<T, false>
    {
        using type = T;  ///< The arithmetic type itself.
    };
}

export namespace std
{
    /**
     * @brief Numeric limits for Integer wrapper types.
     * @tparam T Underlying integral type.
     * @note Inherits all properties from the underlying type's numeric_limits.
     */
    template<original::StdIntegral T>
    class numeric_limits<original::Integer<T>> // NOLINT
        : public std::numeric_limits<T> {};

    /**
     * @brief Numeric limits for Floating wrapper types.
     * @tparam T Underlying floating-point type.
     * @note Inherits all properties from the underlying type's numeric_limits.
     */
    template<original::StdFloating T>
    class numeric_limits<original::Floating<T>> // NOLINT
        : public std::numeric_limits<T> {};
}

export namespace original
{
    /**
     * @brief Strongly-typed number wrapper concept.
     * @tparam T Type to test.
     * @note True for Integer<T> and Floating<T> specializations of NumberTrait.
     */
    template<typename T>
    concept Number =
        requires
        {
            typename details::NumberTrait<T>::Type;
        };

    /**
     * @brief Extract underlying arithmetic type from Number.
     * @tparam T Type satisfying Number concept.
     * @note Returns the standard arithmetic type wrapped by Integer<T> or Floating<T>.
     */
    template<Number T>
    using NumberType = details::NumberTrait<T>::Type;

    /**
     * @brief Accepts either Number wrappers or standard arithmetic types.
     * @tparam T Type to test.
     * @note Useful for functions that should accept both wrapped and unwrapped numbers.
     */
    template<typename T>
    concept NumberLike =
        Number<T> || StdArithmetic<T>;

    /**
     * @brief Extract arithmetic type from NumberLike.
     * @tparam T Type satisfying NumberLike concept.
     * @note Returns the standard arithmetic type, whether wrapped or unwrapped.
     */
    template<NumberLike T>
    using NumberLikeType = details::NumberLikeTypeImpl<T, Number<T>>::type;

    /**
     * @brief Signed integer wrapper concept.
     * @tparam T Type to test.
     * @note True for Integer<T> where T is a signed integral type.
     */
    template<typename T>
    concept SignedInteger =
        Number<T> &&
        StdSignedIntegral<NumberType<T>>;

    /**
     * @brief Unsigned integer wrapper concept.
     * @tparam T Type to test.
     * @note True for Integer<T> where T is an unsigned integral type.
     */
    template<typename T>
    concept UnsignedInteger =
        Number<T> &&
        StdUnsignedIntegral<NumberType<T>>;

    /**
     * @brief Floating-point wrapper concept.
     * @tparam T Type to test.
     * @note True for Floating<T> where T is a floating-point type.
     */
    template<typename T>
    concept FloatingPoint =
        Number<T> &&
        StdFloating<NumberType<T>>;

    /**
     * @brief Signed integral-like concept.
     * @tparam T Type to test.
     * @note True for signed integral types or signed integer wrappers.
     */
    template<typename T>
    concept SignedIntegralLike =
        StdSignedIntegral<T> ||
        SignedInteger<T>;

    /**
     * @brief Unsigned integral-like concept.
     * @tparam T Type to test.
     * @note True for unsigned integral types or unsigned integer wrappers.
     */
    template<typename T>
    concept UnsignedIntegralLike =
        StdUnsignedIntegral<T> ||
        UnsignedInteger<T>;

    /**
     * @brief Concept for integral-like types (signed or unsigned).
     *
     * @tparam T Type to test
     *
     * This concept is satisfied if `T` is either:
     * - A standard signed integral type
     * - A standard unsigned integral type
     * - A signed integer wrapper (`SignedInteger`)
     * - An unsigned integer wrapper (`UnsignedInteger`)
     *
     * Useful for generic algorithms that work with both raw integrals and wrapped integers.
     *
     * @code
     * static_assert(IntegralLike<int>);                     // true
     * static_assert(IntegralLike<Integer<int>>);           // true
     * static_assert(IntegralLike<unsigned long>);          // true
     * static_assert(!IntegralLike<double>);                // false
     * static_assert(!IntegralLike<Floating<float>>);       // false
     * @endcode
     */
    template<typename T>
    concept IntegralLike = SignedIntegralLike<T> || UnsignedIntegralLike<T>;

    /**
     * @brief Require two wrapped integers to have same signedness.
     * @tparam T First integer wrapper type.
     * @tparam U Second integer wrapper type.
     * @note Both must be signed integer wrappers or both must be unsigned integer wrappers.
     */
    template<typename T, typename U>
    concept SameSignIntegral =
        (SignedInteger<T> && SignedInteger<U>) ||
        (UnsignedInteger<T> && UnsignedInteger<U>);

    /**
     * @brief Concept for floating-point-like types.
     *
     * @tparam T Type to test
     *
     * This concept is satisfied if `T` is either:
     * - A standard floating-point type (`float`, `double`, `long double`)
     * - A floating-point wrapper (`FloatingPoint`)
     *
     * Useful for generic numerical code that accepts both raw and wrapped floating-point types.
     *
     * @code
     * static_assert(FloatingPointLike<float>);             // true
     * static_assert(FloatingPointLike<Floating<double>>);  // true
     * static_assert(!FloatingPointLike<int>);              // false
     * static_assert(!FloatingPointLike<Integer<int>>);     // false
     * @endcode
     */
    template<typename T>
    concept FloatingPointLike = FloatingPoint<T> || StdFloating<T>;

    /**
     * @brief Compile-time number constant wrapper.
     *
     * @tparam Num  Number-like type (wrapper or arithmetic)
     * @tparam V    Compile-time value
     *
     * This structure encapsulates a compile-time constant value of a number-like type.
     * It provides type-safe compile-time constants that preserve the distinction between
     * wrapped numbers and raw arithmetic types.
     *
     * @note The value `V` must be a compile-time constant expression.
     *
     * @code
     * // Create a compile-time integer constant
     * using MyIntConst = NumberConstant<Integer<int>, 42>;
     * static_assert(MyIntConst::VAL == 42);
     *
     * // Create a compile-time floating constant
     * using MyFloatConst = NumberConstant<Floating<double>, 3.14159>;
     * static_assert(MyFloatConst::VAL == 3.14159);
     * @endcode
     */
    template<NumberLike Num, NumberLikeType<Num> V>
    struct NumberConstant
    {
        using NumberType = Num;
        using ValueType  = NumberLikeType<Num>;

        static constexpr ValueType VAL = V;
    };

    /**
     * @brief Specialized compile-time integral constant wrapper.
     *
     * @tparam Num  Integral-like type
     * @tparam V    Compile-time integral value
     *
     * Alias template for `NumberConstant` specialized for integral-like types.
     * Provides clearer intent when working specifically with integral constants.
     *
     * @see NumberConstant
     *
     * @code
     * using Answer = IntegralConstant<Integer<int>, 42>;
     * static_assert(Answer::VAL == 42);
     *
     * using MaxSize = IntegralConstant<size_t, 1024>;
     * static_assert(MaxSize::VAL == 1024);
     * @endcode
     */
    template<IntegralLike Num, NumberLikeType<Num> V>
    using IntegralConstant = NumberConstant<Num, V>;

    /**
     * @brief Specialized compile-time floating-point constant wrapper.
     *
     * @tparam Num  Floating-point-like type
     * @tparam V    Compile-time floating-point value
     *
     * Alias template for `NumberConstant` specialized for floating-point-like types.
     * Provides clearer intent when working specifically with floating-point constants.
     *
     * @see NumberConstant
     *
     * @code
     * using Pi = FloatingConstant<Floating<double>, 3.141592653589793>;
     * static_assert(Pi::VAL > 3.14159);
     *
     * using Epsilon = FloatingConstant<float, 1e-6f>;
     * static_assert(Epsilon::VAL == 1e-6f);
     * @endcode
     */
    template<FloatingPointLike Num, NumberLikeType<Num> V>
    using FloatingConstant = NumberConstant<Num, V>;

    /**
     * @brief Specialized compile-time index/size constant wrapper.
     *
     * @tparam V Compile-time size value
     *
     * Alias template for `NumberConstant` specialized for `Size` type.
     * Provides type-safe compile-time constants for sizes and indices.
     *
     * @note Assumes the existence of a `Size` type (likely an alias for `std::size_t` wrapper).
     *
     * @see NumberConstant
     *
     * @code
     * using BufferSize = IndexConstant<1024>;
     * static_assert(BufferSize::VAL == 1024);
     *
     * using DefaultCapacity = IndexConstant<256>;
     * static_assert(DefaultCapacity::VAL == 256);
     * @endcode
     */
    template<Size::Type V>
    using IndexConstant = NumberConstant<Size, V>;
}