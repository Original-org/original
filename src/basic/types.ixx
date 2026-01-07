module;
#include <type_traits>
#include <compare>
#include <concepts>
export module original.basic.types;

namespace original::details {
    template <typename... Args>
    struct HasCommonTypeTrait : std::common_type<Args...> {};

    template <typename... Args>
    inline constexpr bool HasCommonTypeTraitVal =
        requires { typename HasCommonTypeTrait<Args...>::type; };

    template <typename... Args>
    struct HasCommonRefTrait : std::common_reference<Args...> {};

    template <typename... Args>
    inline constexpr bool HasCommonRefTraitVal =
        requires { typename HasCommonRefTrait<Args...>::type; };
} // namespace original::details

/**
 * @brief Main namespace for the Original library
 */
export namespace original {
    /**
     * @addtogroup TypeTraits
     * @{
     */

    /**
     * @brief Concept that constrains a type to be a standard object type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is not a function type, reference type, or
     * void. It corresponds to `std::is_object_v<T>`.
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
    template <typename T>
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
    template <typename T>
    concept IsEmpty = std::is_empty_v<T>;

    /**
     * @brief Concept that constrains a type to be dereferenceable.
     *
     * @tparam T The type to check
     *
     * A type satisfies this concept if the expression `*t` is valid for an instance
     * `t`. This includes pointers, smart pointers, and any type with overloaded
     * `operator*`.
     *
     * @code
     * static_assert(Dereferenceable<int*>);          // true
     * static_assert(Dereferenceable<std::unique_ptr<int>>); // true
     * static_assert(!Dereferenceable<int>);          // false
     * @endcode
     */
    template <typename T>
    concept Dereferenceable = requires(T t) { *t; };

    /**
     * @brief Concept that constrains a type to be a raw pointer type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is a raw pointer type (e.g., `int*`, `const
     * char*`). It corresponds to `std::is_pointer_v<T>`.
     *
     * @code
     * static_assert(IsPointer<int*>);        // true
     * static_assert(IsPointer<const void*>); // true
     * static_assert(!IsPointer<int>);        // false
     * static_assert(!IsPointer<int&>);       // false
     * @endcode
     */
    template <typename T>
    concept IsPointer = std::is_pointer_v<T>;

    /**
     * @brief Concept that constrains a type to be a reference type.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if `T` is either a lvalue reference or rvalue
     * reference. It corresponds to `std::is_reference_v<T>`.
     *
     * @code
     * static_assert(IsReference<int&>);      // true (lvalue reference)
     * static_assert(IsReference<int&&>);     // true (rvalue reference)
     * static_assert(!IsReference<int>);      // false
     * @endcode
     */
    template <typename T>
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
    template <typename From, typename To>
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
    template <typename T, typename U>
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
    template <typename T>
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
    template <bool Test, typename T>
    using MaybeConstType = std::conditional_t<Test, const T, T>;

    /**
     * @brief Concept that constrains a pack of types to have a well-defined common
     * type.
     *
     * @tparam Args The types to check (at least two types recommended)
     *
     * This concept is satisfied if `std::common_type_t<Args...>` is well-formed,
     * meaning there exists a common type to which all `Args...` can be implicitly
     * converted. It is useful for generic code that relies on mixed-type operations
     * requiring a common result type.
     *
     * @note If the pack is empty or no common type exists, the concept is false.
     *       This directly detects the availability of `std::common_type_t` without
     * invoking the stronger requirements of `std::common_with`.
     *
     * @code
     * static_assert(HasCommonType<int, double>);          // true
     * static_assert(HasCommonType<int, long, float>);     // true
     * static_assert(HasCommonType<int, std::string>);     // false (no common type)
     * static_assert(!HasCommonType<>);                    // false (empty pack)
     * @endcode
     */
    template <typename... Args>
    concept HasCommonType = details::HasCommonTypeTraitVal<Args...>;

    /**
     * @brief Concept that constrains a pack of types to have a well-defined common
     * reference type.
     *
     * @tparam Args The types to check
     *
     * This concept is satisfied if `std::common_reference_t<Args...>` is well-formed.
     * It has the same requirements as `HasCommonType` in this implementation
     * (both detect the existence of the corresponding type trait).
     *
     * @note Useful when generic code needs to deduce a common reference type
     *       that preserves reference semantics across multiple types.
     *
     * @code
     * static_assert(HasCommonRef<int&, const int&>);      // true
     * static_assert(!HasCommonRef<int, std::string>);    // false
     * @endcode
     */
    template <typename... Args>
    concept HasCommonRef = HasCommonType<Args...>;

    /**
     * @brief Alias for the common type of parameter pack.
     *
     * @tparam Args The types to find a common type for
     *
     * Equivalent to `std::common_type_t<Args...>`. The type to which all `Args...`
     * can be implicitly converted, if such a type exists.
     *
     * @code
     * using CT = CommonType<int, long, float>;  // typically float or double
     * @endcode
     */
    template <typename... Args>
    using CommonType = std::common_type_t<Args...>;

    /**
     * @brief Alias for the common reference type of a parameter pack.
     *
     * @tparam Args The types to find a common reference type for
     *
     * Equivalent to `std::common_reference_t<Args...>`. Provides a reference type
     * that is common to all `Args...` while preserving reference semantics where
     * possible.
     *
     * @code
     * using CR = CommonRefType<int&, const int&, int&&>;  // typically const int&
     * @endcode
     */
    template <typename... Args>
    using CommonRefType = std::common_reference_t<Args...>;

    template <typename T, typename... Args>
    concept Constructible = std::is_constructible_v<T, Args...>;

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
    template <typename T>
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
    template <typename T>
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
    template <typename T>
    concept StdSignedIntegral = StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Concept that constrains a type to be an unsigned standard integral
     * type.
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
    template <typename T>
    concept StdUnsignedIntegral = StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Concept that constrains two integral types to have the same
     * signedness.
     *
     * @tparam T First integral type
     * @tparam U Second integral type
     *
     * This concept is satisfied if both types are signed integrals or both are
     * unsigned integrals. Useful for operations where signedness must match.
     *
     * @code
     * static_assert(StdSameSignIntegral<int, long>);          // true (both signed)
     * static_assert(StdSameSignIntegral<unsigned, size_t>);   // true (both
     * unsigned) static_assert(!StdSameSignIntegral<int, unsigned>);     // false
     * (mixed)
     * @endcode
     */
    template <typename T, typename U>
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
    template <typename T>
    concept StdFloating = std::is_floating_point_v<T>;

    /** @} */ // end of ArithmeticTypes group

    /**
     * @addtogroup ComparisonTypes
     * @{
     */

    /**
     * @brief Concept that constrains a type to be a valid three-way comparison
     * result.
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
    template <typename T>
    concept StdThreeWayCompareResult =
        SameType<T, std::strong_ordering> || SameType<T, std::weak_ordering> ||
        SameType<T, std::partial_ordering>;

    template<typename... Ts>
    using CommonComparisonCategory = std::common_comparison_category_t<Ts...>;

    /**
     * @brief Concept that constrains a type to support equality comparison.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if expressions `t1 == t2` and `t1 != t2` are valid
     * and yield results convertible to `bool`. It corresponds to
     * `std::equality_comparable<T>`.
     *
     * @note Useful for generic code requiring only equality testing.
     *
     * @code
     * static_assert(EqualityComparable<int>);                 // true
     * static_assert(EqualityComparable<std::string>);         // true
     * struct NoEq {};                                         // no operator== defined
     * static_assert(!EqualityComparable<NoEq>);               // false
     * @endcode
     */
    template <typename T>
    concept EqualityComparable = std::equality_comparable<T>;

    /**
     * @brief Concept that constrains a type to support less-than comparison.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if the expression `t1 < t2` is valid and its result
     * is implicitly convertible to `bool`. It enables use of `<` for ordering.
     *
     * @note This is a minimal requirement for strict weak ordering components.
     *
     * @code
     * static_assert(LessComparable<int>);                     // true
     * static_assert(LessComparable<std::string>);             // true
     * struct NoLess {};                                       // no operator< defined
     * static_assert(!LessComparable<NoLess>);                 // false
     * @endcode
     */
    template <typename T>
    concept LessComparable = requires(T t1, T t2) { {t1 < t2} -> Convertible<bool>; };

    /**
     * @brief Concept that constrains a type to support greater-than comparison.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if the expression `t1 > t2` is valid and its result
     * is implicitly convertible to `bool`. It enables use of `>` for ordering.
     *
     * @note Often used in conjunction with `LessComparable` for symmetric ordering.
     *
     * @code
     * static_assert(GreaterComparable<int>);                  // true
     * static_assert(GreaterComparable<double>);               // true
     * struct NoGreater {};                                    // no operator> defined
     * static_assert(!GreaterComparable<NoGreater>);           // false
     * @endcode
     */
    template <typename T>
    concept GreaterComparable = requires(T t1, T t2) { {t1 > t2} -> Convertible<bool>; };

    /**
     * @brief Concept that constrains a type to support three-way comparison.
     *
     * @tparam T The type to check
     *
     * This concept is satisfied if the expression `t1 <=> t2` is valid and yields a
     * type that satisfies `std::three_way_comparable`. It corresponds to
     * `std::three_way_comparable<T>`.
     *
     * @note Preferred for modern C++ code utilizing the spaceship operator.
     *
     * @code
     * static_assert(ThreeWayComparable<int>);                 // true
     * static_assert(ThreeWayComparable<float>);               // true (partial ordering)
     * struct NoSpaceship {};                                  // no operator<=> defined
     * static_assert(!ThreeWayComparable<NoSpaceship>);        // false
     * @endcode
     */
    template <typename T>
    concept ThreeWayComparable = std::three_way_comparable<T>;

    /**
     * @brief Concept that constrains a type to be fully comparable.
     *
     * @tparam T The type to check
     *
     * This concept requires both equality comparison and three-way comparison
     * support, providing a complete set of relational operators (==, !=, <, >, <=, >=,
     * <=>).
     *
     * @note Useful for generic algorithms requiring total ordering or equality.
     *
     * @code
     * static_assert(Comparable<int>);                         // true
     * static_assert(Comparable<std::string>);                 // true
     * struct Partial {};                                      // only equality, no ordering
     * static_assert(!Comparable<Partial>);                    // false
     * @endcode
     */
    template <typename T>
    concept Comparable = EqualityComparable<T> && ThreeWayComparable<T>;

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
    template <typename T>
    concept Invokable = requires {
      std::is_function_v<std::remove_pointer_t<T>> || requires(T t) { t(); };
    };

    /**
     * @brief Concept that constrains a type to be invocable with specific
     * arguments.
     *
     * @tparam T The callable type
     * @tparam Args Argument types for the invocation
     *
     * This concept is satisfied if `T` is invocable and can be called with
     * arguments of types `Args...`. It corresponds to `std::is_invocable_v<T,
     * Args...>`.
     *
     * @code
     * auto lambda = [](int, double) { return 0; };
     * static_assert(InvokableWith<decltype(lambda), int, double>); // true
     * static_assert(!InvokableWith<decltype(lambda), std::string>); // false
     * @endcode
     */
    template <typename T, typename... Args>
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
    template <typename T, typename... Args>
    using InvokeResultType = std::invoke_result_t<T, Args...>;

    /**
     * @brief Concept that constrains a callable to be invocable with given
     * arguments and return a type exactly matching R.
     *
     * @tparam T   The callable type
     * @tparam R   The exact expected return type
     * @tparam Args The argument types
     *
     * This concept requires that T is invocable with Args... and that the
     * invocation result type is identical to R (including cv-qualifiers).
     *
     * @code
     * auto f = [](int) -> double { return 0.0; };
     * static_assert(InvokableReturns<decltype(f), double, int>);     // true
     * static_assert(!InvokableReturns<decltype(f), const double, int>); // false
     * @endcode
     */
    template <typename T, typename R, typename... Args>
    concept InvokableReturns =
        InvokableWith<T, Args...> && SameType<InvokeResultType<T, Args...>, R>;

    /**
     * @brief Concept that constrains a callable to be invocable with given
     * arguments and return a type implicitly convertible to R.
     *
     * @tparam T   The callable type
     * @tparam R   The target return type (convertible-to)
     * @tparam Args The argument types
     *
     * This concept is satisfied if the invocation result is implicitly convertible
     * to R. Useful for cases where exact type match is not required.
     *
     * @code
     * auto g = [](int) -> int { return 42; };
     * static_assert(InvokableReturnsConvertible<decltype(g), double, int>); // true
     * static_assert(!InvokableReturnsConvertible<decltype(g), std::string, int>);
     * // false
     * @endcode
     */
    template <typename T, typename R, typename... Args>
    concept InvokableReturnsConvertible =
        InvokableWith<T, Args...> && Convertible<InvokeResultType<T, Args...>, R>;

    /**
     * @brief Concept that constrains a type to be a predicate with specific arguments.
     *
     * @tparam F    The predicate type (callable)
     * @tparam Args Argument types for the predicate
     *
     * A type satisfies this concept if:
     * It is invocable with arguments of types `Args...`
     * The result is either:
     * - Convertible to `bool` (traditional predicate)
     * - A standard three-way comparison result type (`std::strong_ordering`, etc.)
     *
     * @note This concept supports both traditional boolean predicates and modern
     *       three-way comparison predicates used with the spaceship operator.
     *
     * @code
     * // Traditional boolean predicate
     * auto is_even = [](int x) -> bool { return x % 2 == 0; };
     * static_assert(Predicate<decltype(is_even), int>); // true
     *
     * // Three-way comparison predicate
     * auto compare = [](int a, int b) { return a <=> b; };
     * static_assert(Predicate<decltype(compare), int, int>); // true (returns ordering)
     * @endcode
     */
    template <typename F, typename... Args>
    concept Predicate = InvokableWith<F, Args...> &&
                        (Convertible<InvokeResultType<F, Args...>, bool> ||
                         StdThreeWayCompareResult<InvokeResultType<F, Args...>>);

    /**
     * @brief Concept that constrains a type to be a unary predicate.
     *
     * @tparam F   The unary predicate type
     * @tparam Arg The argument type
     *
     * This is a specialization of `Predicate` for single-argument predicates.
     * Satisfied for callables that take one argument and return a boolean or
     * comparison result.
     *
     * @see Predicate
     *
     * @code
     * auto is_positive = [](double x) -> bool { return x > 0; };
     * static_assert(UnaryPredicate<decltype(is_positive), double>); // true
     *
     * // Negation functor from standard library
     * static_assert(UnaryPredicate<std::negate<int>, int>); // true
     * @endcode
     */
    template <typename F, typename Arg>
    concept UnaryPredicate = Predicate<F, Arg>;

    /**
     * @brief Concept that constrains a type to be a binary predicate.
     *
     * @tparam F    The binary predicate type
     * @tparam Arg1 First argument type
     * @tparam Arg2 Second argument type
     *
     * This is a specialization of `Predicate` for two-argument predicates.
     * Commonly used for comparison operations, sorting criteria, and equivalence tests.
     *
     * @see Predicate
     *
     * @code
     * auto less_than = [](int a, int b) -> bool { return a < b; };
     * static_assert(BinaryPredicate<decltype(less_than), int, int>); // true
     *
     * // Standard comparator
     * static_assert(BinaryPredicate<std::less<int>, int, int>); // true
     *
     * // Three-way comparison
     * auto three_way = [](int a, int b) { return a <=> b; };
     * static_assert(BinaryPredicate<decltype(three_way), int, int>); // true
     * @endcode
     */
    template <typename F, typename Arg1, typename Arg2>
    concept BinaryPredicate = Predicate<F, Arg1, Arg2>;

    /**
     * @brief Concept that constrains a type to be a function object (functor).
     *
     * @tparam F The type to check
     *
     * A type satisfies this concept if:
     * - It is invocable (has `operator()` or is a function type)
     * - It is a class/struct type (excluding function pointers and plain function types)
     * - Is not a reference type (after reference removal)
     *
     * This concept distinguishes between:
     * - Function objects (classes with `operator()`) - matches `Functor`
     * - Function pointers and plain function types - does NOT match `Functor`
     *
     * @note Useful for template metaprogramming where you need to handle
     *       function objects differently from function pointers.
     *
     * @code
     * struct MyFunctor {
     *     int operator()(int x) const { return x * 2; }
     * };
     * static_assert(Functor<MyFunctor>); // true
     *
     * auto lambda = []() { return 42; };
     * static_assert(Functor<decltype(lambda)>); // true
     *
     * // Function pointers and plain functions are NOT functors
     * static_assert(!Functor<int(*)(int)>); // false
     * static_assert(!Functor<void()>);      // false
     *
     * // References to function objects ARE functors
     * static_assert(Functor<MyFunctor&>);   // true
     * static_assert(Functor<MyFunctor&&>);  // true
     * @endcode
     */
    template <typename F>
    concept Functor = Invokable<F> && std::is_class_v<std::remove_reference_t<F>>;

    /** @} */ // end of InvocableTypes group
} // namespace original