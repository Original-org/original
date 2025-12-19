module;
#include <type_traits>
export module original.basic.types;

/**
 * @brief Namespace containing type traits and concepts for standard arithmetic types.
 */
export namespace original
{
    /**
     * @brief Concept that constrains a type to be a standard arithmetic type.
     * This concept is true for any type T where std::is_arithmetic_v<T> is true.
     * It includes integral and floating-point types.
     * @tparam T The type to check.
     *
     * @note Includes bool, character types, integers, and floating-point types
     * @note Excludes class types, enum types, and void
     *
     * @example
     * @code
     * static_assert(StdArithmetic<int>);        // true
     * static_assert(StdArithmetic<double>);     // true
     * static_assert(StdArithmetic<bool>);       // true
     * static_assert(StdArithmetic<char>);       // true
     * static_assert(!StdArithmetic<std::string>); // false
     * static_assert(!StdArithmetic<void>);      // false
     * @endcode
     */
    template<typename T>
    concept StdArithmetic = std::is_arithmetic_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard arithmetic type.
     * Equivalent to std::is_arithmetic_v<T>.
     * @tparam T The type to check.
     *
     * @see StdArithmetic
     *
     * @example
     * @code
     * constexpr bool test1 = IS_STD_ARITHMETIC<float>;     // true
     * constexpr bool test2 = IS_STD_ARITHMETIC<long>;      // true
     * constexpr bool test3 = IS_STD_ARITHMETIC<MyClass>;   // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_STD_ARITHMETIC = std::is_arithmetic_v<T>;

    /**
     * @brief Concept that constrains a type to be a standard integral type.
     * This concept is true for any type T where std::is_integral_v<T> is true
     * (including bool and character types).
     * @tparam T The type to check.
     *
     * @note Includes bool, char, wchar_t, char8_t, char16_t, char32_t
     * @note Includes signed and unsigned integer types
     * @note Excludes floating-point types and enumeration types
     *
     * @example
     * @code
     * static_assert(StdIntegral<int>);           // true
     * static_assert(StdIntegral<unsigned long>); // true
     * static_assert(StdIntegral<bool>);          // true
     * static_assert(StdIntegral<char>);          // true
     * static_assert(!StdIntegral<float>);        // false
     * static_assert(!StdIntegral<double>);       // false
     * @endcode
     */
    template<typename T>
    concept StdIntegral = std::is_integral_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard integral type.
     * Equivalent to std::is_integral_v<T>.
     * @tparam T The type to check.
     *
     * @see StdIntegral
     *
     * @example
     * @code
     * constexpr bool test1 = IS_STD_INTEGRAL<short>;      // true
     * constexpr bool test2 = IS_STD_INTEGRAL<wchar_t>;    // true
     * constexpr bool test3 = IS_STD_INTEGRAL<float>;      // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_STD_INTEGRAL = std::is_integral_v<T>;

    /**
     * @brief Concept that constrains a type to be a signed standard integral type.
     * Requires StdIntegral<T> and that T is signed.
     * @tparam T The type to check.
     *
     * @note Includes signed char, short, int, long, long long
     * @note Excludes unsigned types, bool, and character types (implementation-defined signedness)
     *
     * @example
     * @code
     * static_assert(StdSignedIntegral<int>);        // true
     * static_assert(StdSignedIntegral<signed char>); // true
     * static_assert(StdSignedIntegral<long long>);  // true
     * static_assert(!StdSignedIntegral<unsigned>);  // false
     * static_assert(!StdSignedIntegral<bool>);      // false
     * // char may or may not satisfy depending on implementation
     * @endcode
     */
    template<typename T>
    concept StdSignedIntegral = StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a signed standard integral type.
     * Requires IS_STD_INTEGRAL<T> and that T is signed.
     * @tparam T The type to check.
     *
     * @see StdSignedIntegral
     *
     * @example
     * @code
     * constexpr bool test1 = IS_STD_SIGNED_INTEGRAL<int>;          // true
     * constexpr bool test2 = IS_STD_SIGNED_INTEGRAL<signed char>;  // true
     * constexpr bool test3 = IS_STD_SIGNED_INTEGRAL<unsigned int>; // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_STD_SIGNED_INTEGRAL = StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Concept that constrains a type to be an unsigned standard integral type.
     * Requires StdIntegral<T> and that T is unsigned.
     * @tparam T The type to check.
     *
     * @note Includes unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
     * @note Includes bool and character types with implementation-defined unsignedness
     *
     * @example
     * @code
     * static_assert(StdUnsignedIntegral<unsigned int>);    // true
     * static_assert(StdUnsignedIntegral<bool>);           // true
     * static_assert(StdUnsignedIntegral<unsigned char>);  // true
     * static_assert(!StdUnsignedIntegral<int>);           // false
     * static_assert(!StdUnsignedIntegral<signed char>);   // false
     * @endcode
     */
    template<typename T>
    concept StdUnsignedIntegral = StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is an unsigned standard integral type.
     * Requires IS_STD_INTEGRAL<T> and that T is unsigned.
     * @tparam T The type to check.
     *
     * @see StdUnsignedIntegral
     *
     * @example
     * @code
     * constexpr bool test1 = IS_STD_UNSIGNED_INTEGRAL<unsigned>;  // true
     * constexpr bool test2 = IS_STD_UNSIGNED_INTEGRAL<bool>;      // true
     * constexpr bool test3 = IS_STD_UNSIGNED_INTEGRAL<int>;       // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_STD_UNSIGNED_INTEGRAL = StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Compile-time constant indicating whether two types have same integral signedness.
     * True if both T and U are signed integrals or both are unsigned integrals.
     * @tparam T First type to compare
     * @tparam U Second type to compare
     *
     * @note Returns false if either type is not an integral type
     * @note Character types may have implementation-defined signedness
     *
     * @example
     * @code
     * constexpr bool test1 = STD_HAS_SAME_SIGN_INTEGRAL<int, long>;           // true (both signed)
     * constexpr bool test2 = STD_HAS_SAME_SIGN_INTEGRAL<unsigned, size_t>;    // true (both unsigned)
     * constexpr bool test3 = STD_HAS_SAME_SIGN_INTEGRAL<int, unsigned>;       // false (different signs)
     * constexpr bool test4 = STD_HAS_SAME_SIGN_INTEGRAL<int, float>;          // false (float not integral)
     * @endcode
     */
    template<typename T, typename U>
    constexpr bool STD_HAS_SAME_SIGN_INTEGRAL
        = (IS_STD_SIGNED_INTEGRAL<T> && IS_STD_SIGNED_INTEGRAL<U>) ||
          (IS_STD_UNSIGNED_INTEGRAL<T> && IS_STD_UNSIGNED_INTEGRAL<U>);

    /**
     * @brief Concept that constrains two types to have same integral signedness.
     * Requires that both types are integrals with the same signedness.
     * @tparam T First type to compare
     * @tparam U Second type to compare
     *
     * @see STD_HAS_SAME_SIGN_INTEGRAL
     */
    template<typename T, typename U>
    concept StdSameSignIntegral = STD_HAS_SAME_SIGN_INTEGRAL<T, U>;

    /**
     * @brief Concept that constrains a type to be a standard floating-point type.
     * This concept is true for any type T where std::is_floating_point_v<T> is true.
     * @tparam T The type to check.
     *
     * @note Includes float, double, and long double
     * @note Excludes integer types and extended floating-point types (C++23)
     *
     * @example
     * @code
     * static_assert(StdFloating<float>);       // true
     * static_assert(StdFloating<double>);      // true
     * static_assert(StdFloating<long double>); // true
     * static_assert(!StdFloating<int>);        // false
     * static_assert(!StdFloating<decimal32>);  // false (C++23, not standard floating-point)
     * @endcode
     */
    template<typename T>
    concept StdFloating = std::is_floating_point_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard floating-point type.
     * Equivalent to std::is_floating_point_v<T>.
     * @tparam T The type to check.
     *
     * @see StdFloating
     *
     * @example
     * @code
     * constexpr bool test1 = IS_STD_FLOATING<float>;        // true
     * constexpr bool test2 = IS_STD_FLOATING<double>;       // true
     * constexpr bool test3 = IS_STD_FLOATING<long double>;  // true
     * constexpr bool test4 = IS_STD_FLOATING<int>;          // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_STD_FLOATING = std::is_floating_point_v<T>;
}