module;
#include <type_traits>
#include <compare>
#include <concepts>
export module original.basic.types;

/**
 * @brief Main namespace for the Original library
 */
export namespace original
{
    /**
     * @addtogroup TypeTraits
     * @{
     */

    /**
     * @brief Concept that constrains a type to be a standard object type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is not a function type, reference type, or void.
     * It corresponds to `std::is_object_v<T>`.
     *
     * @note Useful for ensuring a type can be instantiated as an object.
     *
     * @code
     * static_assert(IsObject<int>);          // true
     * static_assert(!IsObject<void>);        // false
     * static_assert(!IsObject<int&>);        // false
     * static_assert(!IsObject<int()>);       // false (function type)
     * @endcode
     */
    template<typename T>
    concept IsObject = std::is_object_v<T>;

    /**
     * @brief Concept that constrains a type to be an empty class type.
     *
     * @tparam T The type to check
     *
     * A type satisfies this concept if it's a class type with:
     * - No non-static data members
     * - No virtual functions
     * - No virtual base classes
     *
     * @note This is particularly useful for Empty Base Optimization (EBO),
     *       allowing compilers to optimize storage for empty base classes.
     *
     * @see https://en.cppreference.com/w/cpp/types/is_empty
     *
     * @code
     * struct Empty {};
     * struct NonEmpty { int x; };
     *
     * static_assert(IsEmpty<Empty>);         // true
     * static_assert(!IsEmpty<NonEmpty>);     // false
     * static_assert(!IsEmpty<int>);          // false (not a class)
     * @endcode
     */
    template<typename T>
    concept IsEmpty = std::is_empty_v<T>;

    /**
     * @brief Concept that constrains a type to be dereferenceable.
     *
     * @tparam T The type to check
     *
     * A type satisfies this concept if the expression `*t` is valid for an instance `t`.
     * This includes pointers, smart pointers, and any type with overloaded `operator*`.
     *
     * @code
     * static_assert(Dereferenceable<int*>);          // true
     * static_assert(Dereferenceable<std::unique_ptr<int>>); // true
     * static_assert(!Dereferenceable<int>);          // false
     * @endcode
     */
    template<typename T>
    concept Dereferenceable = requires(T t) { *t; };

    /**
     * @brief Concept that constrains a type to be a raw pointer type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is a raw pointer type (e.g., `int*`, `const char*`).
     * It corresponds to `std::is_pointer_v<T>`.
     *
     * @code
     * static_assert(IsPointer<int*>);        // true
     * static_assert(IsPointer<const void*>); // true
     * static_assert(!IsPointer<int>);        // false
     * static_assert(!IsPointer<int&>);       // false
     * @endcode
     */
    template<typename T>
    concept IsPointer = std::is_pointer_v<T>;

    /**
     * @brief Concept that constrains a type to be a reference type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is either a lvalue reference or rvalue reference.
     * It corresponds to `std::is_reference_v<T>`.
     *
     * @code
     * static_assert(IsReference<int&>);      // true (lvalue reference)
     * static_assert(IsReference<int&&>);     // true (rvalue reference)
     * static_assert(!IsReference<int>);      // false
     * @endcode
     */
    template<typename T>
    concept IsReference = std::is_reference_v<T>;

    /**
     * @brief Concept for type convertibility.
     *
     * @tparam From Source type
     * @tparam To Target type
     *
     * This concept is satisfied if `From` is implicitly convertible to `To`.
     * It corresponds to `std::is_convertible_v<From, To>`.
     *
     * @note This checks for implicit conversions only. Explicit conversions
     *       (constructors marked `explicit` or `static_cast`) are not considered.
     *
     * @code
     * static_assert(Convertible<int, double>);       // true
     * static_assert(Convertible<double, int>);       // true (with truncation)
     * static_assert(Convertible<Derived*, Base*>);   // true (derived to base)
     * static_assert(!Convertible<int*, double*>);    // false
     * @endcode
     */
    template<typename From, typename To>
    concept Convertible = std::is_convertible_v<From, To>;

    /**
     * @brief Concept for identical types.
     *
     * @tparam T First type
     * @tparam U Second type
     *
     * This concept is satisfied if `T` and `U` are exactly the same type,
     * considering cv-qualifiers. It corresponds to `std::same_as<T, U>`.
     *
     * @code
     * static_assert(SameType<int, int>);         // true
     * static_assert(!SameType<int, const int>);  // false (cv-qualifiers differ)
     * static_assert(!SameType<int, double>);     // false (different types)
     * @endcode
     */
    template<typename T, typename U>
    concept SameType = std::same_as<T, U>;

    /**
     * @brief Removes cv-qualifiers and reference from a type.
     *
     * @tparam T The type to transform
     *
     * This type alias provides the decayed type of `T` with all cv-qualifiers
     * and reference removed. Equivalent to `std::remove_cvref_t<T>`.
     *
     * @code
     * using T1 = RemoveCVRefType<const int&>;    // int
     * using T2 = RemoveCVRefType<volatile int&&>; // int
     * using T3 = RemoveCVRefType<int>;           // int
     * @endcode
     */
    template<typename T>
    using RemoveCVRefType = std::remove_cvref_t<T>;

    /**
     * @brief Conditionally adds const qualification to a type.
     *
     * @tparam Test Boolean condition
     * @tparam T The type to potentially make const
     *
     * If `Test` is true, the resulting type is `const T`. Otherwise, it's `T`.
     * Useful for creating const-correct generic code.
     *
     * @code
     * using T1 = MaybeConstType<true, int>;   // const int
     * using T2 = MaybeConstType<false, int>;  // int
     * @endcode
     */
    template<bool Test, typename T>
    using MaybeConstType = std::conditional_t<Test, const T, T>;

    /** @} */ // end of TypeTraits group

    /**
     * @addtogroup ArithmeticTypes
     * @{
     */

    /**
     * @brief Concept that constrains a type to be a standard arithmetic type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is either:
     * - An integral type (signed or unsigned integers, bool, char)
     * - A floating-point type (float, double, long double)
     *
     * It corresponds to `std::is_arithmetic_v<T>`.
     *
     * @code
     * static_assert(StdArithmetic<int>);      // true
     * static_assert(StdArithmetic<double>);   // true
     * static_assert(StdArithmetic<bool>);     // true
     * static_assert(!StdArithmetic<void*>);   // false
     * @endcode
     */
    template<typename T>
    concept StdArithmetic = std::is_arithmetic_v<T>;

    /**
     * @brief Concept that constrains a type to be a standard integral type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is:
     * - bool, char, char8_t, char16_t, char32_t, wchar_t
     * - signed or unsigned integer types (short, int, long, long-long)
     *
     * It corresponds to `std::is_integral_v<T>`.
     *
     * @code
     * static_assert(StdIntegral<int>);        // true
     * static_assert(StdIntegral<bool>);       // true
     * static_assert(StdIntegral<unsigned>);   // true
     * static_assert(!StdIntegral<float>);     // false
     * @endcode
     */
    template<typename T>
    concept StdIntegral = std::is_integral_v<T>;

    /**
     * @brief Concept that constrains a type to be a signed standard integral type.
     *
     * @tparam T The type to check
     *
     * This concept requires `T` to satisfy `StdIntegral` and be signed.
     *
     * @code
     * static_assert(StdSignedIntegral<int>);      // true
     * static_assert(StdSignedIntegral<signed char>); // true
     * static_assert(!StdSignedIntegral<unsigned>); // false
     * static_assert(!StdSignedIntegral<float>);    // false
     * @endcode
     */
    template<typename T>
    concept StdSignedIntegral =
        StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Concept that constrains a type to be an unsigned standard integral type.
     *
     * @tparam T The type to check
     *
     * This concept requires `T` to satisfy `StdIntegral` and be unsigned.
     *
     * @code
     * static_assert(StdUnsignedIntegral<unsigned>);   // true
     * static_assert(StdUnsignedIntegral<bool>);       // true
     * static_assert(!StdUnsignedIntegral<int>);       // false
     * @endcode
     */
    template<typename T>
    concept StdUnsignedIntegral =
        StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Concept that constrains two integral types to have the same signedness.
     *
     * @tparam T First integral type
     * @tparam U Second integral type
     *
     * This concept is satisfied if both types are signed integrals or both are
     * unsigned integrals. Useful for operations where signedness must match.
     *
     * @code
     * static_assert(StdSameSignIntegral<int, long>);          // true (both signed)
     * static_assert(StdSameSignIntegral<unsigned, size_t>);   // true (both unsigned)
     * static_assert(!StdSameSignIntegral<int, unsigned>);     // false (mixed)
     * @endcode
     */
    template<typename T, typename U>
    concept StdSameSignIntegral =
        (StdSignedIntegral<T> && StdSignedIntegral<U>) ||
        (StdUnsignedIntegral<T> && StdUnsignedIntegral<U>);

    /**
     * @brief Concept that constrains a type to be a standard floating-point type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied for `float`, `double`, and `long double`.
     * It corresponds to `std::is_floating_point_v<T>`.
     *
     * @code
     * static_assert(StdFloating<float>);      // true
     * static_assert(StdFloating<double>);     // true
     * static_assert(!StdFloating<int>);       // false
     * @endcode
     */
    template<typename T>
    concept StdFloating = std::is_floating_point_v<T>;

    /** @} */ // end of ArithmeticTypes group

    /**
     * @addtogroup ComparisonTypes
     * @{
     */

    /**
     * @brief Concept that constrains a type to be a valid three-way comparison result.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied for the standard comparison category types:
     * - `std::strong_ordering` (equality implies substitutability)
     * - `std::weak_ordering` (equality is transitive but not substitutable)
     * - `std::partial_ordering` (allows incomparable values)
     *
     * @see https://en.cppreference.com/w/cpp/utility/compare
     *
     * @code
     * static_assert(StdThreeWayCompareResult<std::strong_ordering>);  // true
     * static_assert(StdThreeWayCompareResult<std::partial_ordering>); // true
     * static_assert(!StdThreeWayCompareResult<int>);                  // false
     * @endcode
     */
    template<typename T>
    concept StdThreeWayCompareResult =
        SameType<T, std::strong_ordering> ||
        SameType<T, std::weak_ordering>   ||
        SameType<T, std::partial_ordering>;

    /** @} */ // end of ComparisonTypes group

    /**
     * @addtogroup InvocableTypes
     * @{
     */

    /**
     * @brief Concept that constrains a type to be invocable.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is:
     * - A function type
     * - A function pointer type
     * - A type with an `operator()` (callable object)
     *
     * @note This is a basic check. For more precise constraints on arguments
     *       and return types, use `InvokableWith`.
     *
     * @code
     * static_assert(Invokable<void()>);               // true (function type)
     * static_assert(Invokable<void(*)()>);            // true (function pointer)
     * static_assert(Invokable<std::function<void()>>); // true (callable object)
     * static_assert(!Invokable<int>);                 // false
     * @endcode
     */
    template<typename T>
    concept Invokable = requires {
        std::is_function_v<std::remove_pointer_t<T>> ||
        requires(T t) { t(); };
    };

    /**
     * @brief Concept that constrains a type to be invocable with specific arguments.
     *
     * @tparam T The callable type
     * @tparam Args Argument types for the invocation
     *
     * This concept is satisfied if `T` is invocable and can be called with
     * arguments of types `Args...`. It corresponds to `std::is_invocable_v<T, Args...>`.
     *
     * @code
     * auto lambda = [](int, double) { return 0; };
     * static_assert(InvokableWith<decltype(lambda), int, double>); // true
     * static_assert(!InvokableWith<decltype(lambda), std::string>); // false
     * @endcode
     */
    template<typename T, typename... Args>
    concept InvokableWith = Invokable<T> && std::is_invocable_v<T, Args...>;

    /**
     * @brief Alias template for the result type of invocation.
     *
     * @tparam T The callable type
     * @tparam Args Argument types for the invocation
     *
     * This provides the return type of invoking `T` with arguments `Args...`.
     * Equivalent to `std::invoke_result_t<T, Args...>`.
     *
     * @code
     * using Result = InvokeResult<decltype([](int x) { return x * 2; }), int>;
     * static_assert(SameType<Result, int>); // true
     * @endcode
     */
    template<typename T, typename... Args>
    using InvokeResult = std::invoke_result_t<T, Args...>;

    /** @} */ // end of InvocableTypes group
} // namespace original