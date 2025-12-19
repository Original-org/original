module;
#include <type_traits>
export module original.basic.number.traits;
import original.basic.types;
import original.basic.number.numberImpl;

/**
 * @brief Namespace containing type traits for the strongly-typed number wrappers.
 */
export namespace original
{
    /**
     * @struct NumberTrait
     * @brief Trait to detect whether a type is one of the strongly-typed number wrappers.
     * Specializations exist for Integer<T> and Floating<T>.
     * @tparam T Type to test.
     *
     * @note Primary template returns false for all types
     * @note Specializations provide value = true for number wrapper types
     * @note Provides Type alias to underlying arithmetic type
     *
     * @example
     * @code
     * static_assert(NumberTrait<Integer<int>>::value);      // true
     * static_assert(NumberTrait<Floating<double>>::value);  // true
     * static_assert(!NumberTrait<int>::value);             // false
     * static_assert(!NumberTrait<std::string>::value);     // false
     *
     * using Underlying = NumberTrait<Integer<long>>::Type;  // long
     * @endcode
     */
    template<typename T>
    struct NumberTrait : std::false_type {};

    /**
     * @brief Specialization for Integer types.
     * @tparam T Standard integral type wrapped by Integer
     */
    template<StdIntegral T>
    struct NumberTrait<Integer<T>> : std::true_type
    {
        using Type = T;  ///< Underlying integral type
    };

    /**
     * @brief Specialization for Floating types.
     * @tparam T Standard floating-point type wrapped by Floating
     */
    template<StdFloating T>
    struct NumberTrait<Floating<T>> : std::true_type
    {
        using Type = T;  ///< Underlying floating-point type
    };

    /**
     * @brief Compile-time constant indicating whether T is a strongly-typed number wrapper.
     * @tparam T The type to check.
     *
     * @see NumberTrait
     *
     * @example
     * @code
     * constexpr bool test1 = IS_NUMBER<Integer<short>>;     // true
     * constexpr bool test2 = IS_NUMBER<Floating<float>>;    // true
     * constexpr bool test3 = IS_NUMBER<double>;            // false
     * constexpr bool test4 = IS_NUMBER<MyClass>;           // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_NUMBER = NumberTrait<T>::value;

    /**
     * @brief Compile-time constant indicating whether T is either a strongly-typed number
     * or a standard arithmetic type.
     * @tparam T The type to check.
     *
     * @note Useful for writing functions that accept both wrapped and unwrapped numbers
     *
     * @example
     * @code
     * constexpr bool test1 = IS_NUMBER_LIKE<Integer<int>>;  // true
     * constexpr bool test2 = IS_NUMBER_LIKE<double>;       // true
     * constexpr bool test3 = IS_NUMBER_LIKE<std::string>;  // false
     * constexpr bool test4 = IS_NUMBER_LIKE<void>;         // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_NUMBER_LIKE = IS_NUMBER<T> || IS_STD_ARITHMETIC<T>;

    /**
     * @brief Concept requiring T to be a strongly-typed number wrapper.
     * @tparam T The type to constrain.
     *
     * @see IS_NUMBER
     */
    template<typename T>
    concept Number = IS_NUMBER<T>;

    /**
     * @brief Concept requiring T to be either a strongly-typed number wrapper
     * or a standard arithmetic type.
     * @tparam T The type to constrain.
     *
     * @see IS_NUMBER_LIKE
     *
     * @example
     * @code
     * template<NumberLike T>
     * auto add(T a, T b) { return a + b; }
     *
     * add(Integer<int>{1}, Integer<int>{2});  // OK
     * add(1.0, 2.0);                         // OK
     * add("hello", "world");                 // Error: strings not NumberLike
     * @endcode
     */
    template<typename T>
    concept NumberLike = IS_NUMBER_LIKE<T>;

    /**
     * @brief Extracts the underlying type from a strongly-typed number wrapper.
     * @tparam T A type satisfying the Number concept.
     *
     * @note Returns the standard arithmetic type wrapped by Integer<T> or Floating<T>
     *
     * @example
     * @code
     * using T1 = NumberType<Integer<long>>;      // long
     * using T2 = NumberType<Floating<float>>;    // float
     *
     * // template usage:
     * template<Number T>
     * void func(T value) {
     *     using Underlying = NumberType<T>;      // Gets int, double, etc.
     *     // ...
     * }
     * @endcode
     */
    template<Number T>
    using NumberType = NumberTrait<T>::Type;

    /**
     * @struct NumberLikeTrait
     * @brief Trait to obtain the underlying arithmetic type from Number
     * or standard arithmetic types.
     * @tparam T Type to query.
     *
     * @note Unspecialized template is incomplete (cannot be instantiated)
     * @note Provides Type alias to the underlying arithmetic type
     *
     * @example
     * @code
     * using T1 = NumberLikeTrait<Integer<int>>::Type;    // int
     * using T2 = NumberLikeTrait<double>::Type;          // double
     * // using T3 = NumberLikeTrait<std::string>::Type; // Error: no specialization
     * @endcode
     */
    template<typename T>
    struct NumberLikeTrait {};

    /**
     * @brief Specialization for strongly-typed number wrappers.
     * @tparam T Type satisfying Number concept
     */
    template<Number T>
    struct NumberLikeTrait<T>
    {
        using Type = NumberType<T>;  ///< Underlying type of the number wrapper
    };

    /**
     * @brief Specialization for plain standard arithmetic types.
     * @tparam T Standard arithmetic type
     */
    template<StdArithmetic T>
    struct NumberLikeTrait<T>
    {
        using Type = T;  ///< The arithmetic type itself
    };

    /**
     * @brief Alias template to retrieve the underlying arithmetic type
     * for NumberLike types.
     * @tparam T A type satisfying the NumberLike concept.
     *
     * @note Returns the standard arithmetic type, whether wrapped or unwrapped
     *
     * @example
     * @code
     * using T1 = NumberLikeType<Integer<long>>;   // long
     * using T2 = NumberLikeType<float>;          // float
     * using T3 = NumberLikeType<short>;          // short
     *
     * // In templates:
     * template<NumberLike T>
     * auto convert(T value) {
     *     using ArithType = NumberLikeType<T>;   // Gets the arithmetic type
     *     return static_cast<ArithType>(value);
     * }
     * @endcode
     */
    template<NumberLike T>
    using NumberLikeType = NumberLikeTrait<T>::Type;

    /**
     * @brief Compile-time constant indicating whether T is an unsigned integer wrapper.
     * @tparam T Type to check
     *
     * @note Only true for Integer<U> where U is unsigned integral type
     *
     * @example
     * @code
     * constexpr bool test1 = IS_UNSIGNED_INTEGER<Integer<unsigned>>;  // true
     * constexpr bool test2 = IS_UNSIGNED_INTEGER<Integer<int>>;       // false
     * constexpr bool test3 = IS_UNSIGNED_INTEGER<Floating<float>>;    // false
     * constexpr bool test4 = IS_UNSIGNED_INTEGER<unsigned>;           // false (not wrapped)
     * @endcode
     */
    template<typename T>
    constexpr bool IS_UNSIGNED_INTEGER = IS_NUMBER<T> && IS_STD_UNSIGNED_INTEGRAL<NumberType<T>>;

    /**
     * @brief Compile-time constant indicating whether T is a signed integer wrapper.
     * @tparam T Type to check
     *
     * @note Only true for Integer<T> where T is signed integral type
     *
     * @example
     * @code
     * constexpr bool test1 = IS_SIGNED_INTEGER<Integer<int>>;        // true
     * constexpr bool test2 = IS_SIGNED_INTEGER<Integer<long long>>;  // true
     * constexpr bool test3 = IS_SIGNED_INTEGER<Integer<unsigned>>;   // false
     * constexpr bool test4 = IS_SIGNED_INTEGER<Floating<double>>;    // false
     * @endcode
     */
    template<typename T>
    constexpr bool IS_SIGNED_INTEGER = IS_NUMBER<T> && IS_STD_SIGNED_INTEGRAL<NumberType<T>>;

    /**
     * @brief Compile-time constant indicating whether T is a floating-point wrapper.
     * @tparam T Type to check
     *
     * @note Only true for Floating<T> where T is floating-point type
     *
     * @example
     * @code
     * constexpr bool test1 = IS_FLOATING_POINT<Floating<float>>;     // true
     * constexpr bool test2 = IS_FLOATING_POINT<Floating<double>>;    // true
     * constexpr bool test3 = IS_FLOATING_POINT<Integer<int>>;        // false
     * constexpr bool test4 = IS_FLOATING_POINT<long double>;         // false (not wrapped)
     * @endcode
     */
    template<typename T>
    constexpr bool IS_FLOATING_POINT = IS_NUMBER<T> && IS_STD_FLOATING<NumberType<T>>;

    /**
     * @brief Concept requiring T to be an unsigned integer wrapper.
     * @tparam T The type to constrain.
     *
     * @see IS_UNSIGNED_INTEGER
     *
     * @example
     * @code
     * template<UnsignedInteger T>
     * T safeDecrement(T value) {
     *     // Safe for unsigned, won't go negative
     *     return value > T{0} ? value - T{1} : T{0};
     * }
     * @endcode
     */
    template<typename T>
    concept UnsignedInteger = IS_UNSIGNED_INTEGER<T>;

    /**
     * @brief Concept requiring T to be a signed integer wrapper.
     * @tparam T The type to constrain.
     *
     * @see IS_SIGNED_INTEGER
     *
     * @example
     * @code
     * template<SignedInteger T>
     * T absoluteValue(T value) {
     *     return value < T{0} ? -value : value;
     * }
     * @endcode
     */
    template<typename T>
    concept SignedInteger = IS_SIGNED_INTEGER<T>;

    /**
     * @brief Concept requiring T to be a floating-point wrapper.
     * @tparam T The type to constrain.
     *
     * @see IS_FLOATING_POINT
     *
     * @example
     * @code
     * template<FloatingPoint T>
     * bool isApproximatelyZero(T value, T epsilon) {
     *     return abs(value) < epsilon;
     * }
     * @endcode
     */
    template<typename T>
    concept FloatingPoint = IS_FLOATING_POINT<T>;

    /**
     * @brief Compile-time constant indicating whether two number wrappers
     * have the same integral signedness.
     * @tparam T First number wrapper type
     * @tparam U Second number wrapper type
     *
     * @note Both types must be Number wrappers
     * @note Both underlying types must be integral
     * @note Both must be signed or both must be unsigned
     *
     * @example
     * @code
     * constexpr bool test1 = HAS_SAME_SIGN_INTEGRAL<Integer<int>, Integer<long>>;   // true
     * constexpr bool test2 = HAS_SAME_SIGN_INTEGRAL<Integer<unsigned>, Integer<size_t>>; // true
     * constexpr bool test3 = HAS_SAME_SIGN_INTEGRAL<Integer<int>, Integer<unsigned>>; // false
     * constexpr bool test4 = HAS_SAME_SIGN_INTEGRAL<Floating<float>, Integer<int>>; // false
     * @endcode
     */
    template<typename T, typename U>
    constexpr bool HAS_SAME_SIGN_INTEGRAL
        = IS_NUMBER<T> &&
          IS_NUMBER<U> &&
          STD_HAS_SAME_SIGN_INTEGRAL<NumberType<T>, NumberType<U>>;

    /**
     * @brief Concept requiring two number wrappers to have same integral signedness.
     * @tparam T First number wrapper type
     * @tparam U Second number wrapper type
     *
     * @see HAS_SAME_SIGN_INTEGRAL
     */
    template<typename T, typename U>
    concept SameSignIntegral = HAS_SAME_SIGN_INTEGRAL<T, U>;
}