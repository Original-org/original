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
    class numeric_limits<original::Integer<T>>
        : public std::numeric_limits<T> {};

    /**
     * @brief Numeric limits for Floating wrapper types.
     * @tparam T Underlying floating-point type.
     * @note Inherits all properties from the underlying type's numeric_limits.
     */
    template<original::StdFloating T>
    class numeric_limits<original::Floating<T>>
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
     * @brief Require two wrapped integers to have same signedness.
     * @tparam T First integer wrapper type.
     * @tparam U Second integer wrapper type.
     * @note Both must be signed integer wrappers or both must be unsigned integer wrappers.
     */
    template<typename T, typename U>
    concept SameSignIntegral =
        (SignedInteger<T> && SignedInteger<U>) ||
        (UnsignedInteger<T> && UnsignedInteger<U>);
}