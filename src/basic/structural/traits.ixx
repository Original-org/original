module;
#include <type_traits>
#include <tuple>
export module original.basic.structural.traits;
import original.basic.number;
import original.basic.types;
import original.basic.structural.utility;

/**
 * @addtogroup StructuralTraits
 * @{
 */

namespace original::details
{
    using std::get;

    // Internal recursive checks for structural compatibility (not exported)

    template<typename T, Size::Type I, Size::Type N>
    struct StructuralElementCheck
    {
        static constexpr bool value =
        requires
        {
            typename std::tuple_element<I, std::remove_cvref_t<T>>::type; // NOLINT
        }
        && std::tuple_size<std::remove_cvref_t<T>>::value >= 0 // NOLINT
        && StructuralElementCheck<T, I + 1, N>::value;
    };

    template<typename T, Size::Type N>
    struct StructuralElementCheck<T, N, N>
        : std::true_type {};

    template<typename T, Size::Type N>
    inline constexpr bool HasStructuralElementsValue =
        StructuralElementCheck<T, 0, N>::value;

    template<typename T, Size::Type I, Size::Type N>
    struct StructuralGetCheck
    {
        static constexpr bool value =
        requires(T& t) { get<I>(t); } &&
        requires(const T& t) { get<I>(t); } &&
        (std::is_move_constructible_v<T> ?
        requires(T&& t) { get<I>(std::move(t)); } : true) &&
        StructuralGetCheck<T, I + 1, N>::value;
    };

    template<typename T, Size::Type N>
    struct StructuralGetCheck<T, N, N>
        : std::true_type {};

    template<typename T, Size::Type N>
    inline constexpr bool HasStructuralGetValue =
        StructuralGetCheck<T, 0, N>::value;
}

export namespace original
{
    using std::get;

    /**
     * @brief Concept requiring a type to expose a compile-time tuple size.
     *
     * @tparam T The type to check
     *
     * Satisfied if `std::tuple_size<std::remove_cvref_t<T>>` is well-formed.
     *
     * @code
     * static_assert(HasStructuralSize<std::tuple<int, double>>);  // true
     * static_assert(HasStructuralSize<std::pair<int, int>>);      // true
     * static_assert(!HasStructuralSize<int>);                     // false
     * @endcode
     */
    template<typename T>
    concept HasStructuralSize =
    requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    /**
     * @brief Concept requiring a type to have valid tuple elements for all indices.
     *
     * @tparam T The type to check
     *
     * Requires HasStructuralSize and that `std::tuple_element<I, T>` is well-formed
     * for every I in [0, tuple_size).
     */
    template<typename T>
    concept HasStructuralElements =
    HasStructuralSize<T> &&
    details::HasStructuralElementsValue<
        T,
        std::tuple_size<std::remove_cvref_t<T>>::value // NOLINT
    >;

    /**
     * @brief Concept identifying types structurally compatible with std::tuple.
     *
     * @tparam T The type to check
     *
     * A type satisfies Structural if:
     * - It has a valid tuple_size
     * - All tuple_element<I, T> are well-formed
     * - get<I>(t) is valid for lvalue, const lvalue, and (if move-constructible) rvalue references
     *
     * This enables generic code to treat std::tuple, std::pair, std::array, and custom
     * structural types uniformly.
     *
     * @code
     * static_assert(Structural<std::tuple<int, double>>);     // true
     * static_assert(Structural<std::pair<int, char>>);        // true
     * static_assert(Structural<original::Array<int, 3>>);     // true
     * @endcode
     */
    template<typename T>
    concept Structural =
    HasStructuralElements<T> &&
    details::HasStructuralGetValue<
        T,
        std::tuple_size<std::remove_cvref_t<T>>::value // NOLINT
    >;

    /**
     * @brief Concept for pair-like structural types.
     *
     * @tparam T The type to check
     *
     * Satisfied if T satisfies Structural and has exactly two elements.
     *
     * @code
     * static_assert(CoupleLike<std::pair<int, double>>);       // true
     * static_assert(!CoupleLike<std::tuple<int>>);             // false
     * @endcode
     */
    template<typename T>
    concept CoupleLike =
    Structural<T> &&
    std::tuple_size<std::remove_cvref_t<T>>::value == 2; // NOLINT

    /**
     * @brief Concept for tuple-like structural types.
     *
     * @tparam T The type to check
     *
     * Satisfied if T satisfies Structural and has at least one element.
     *
     * @code
     * static_assert(TupleLike<std::tuple<int>>);               // true
     * static_assert(TupleLike<std::pair<int, int>>);           // true
     * static_assert(!TupleLike<int>);                          // false
     * @endcode
     */
    template<typename T>
    concept TupleLike =
    Structural<T> &&
    std::tuple_size<std::remove_cvref_t<T>>::value >= 1; // NOLINT

    /**
     * @brief Traits class providing uniform access to structural type properties.
     *
     * @tparam T A type satisfying Structural
     *
     * Exposes the underlying type, element types by index, and compile-time size.
     *
     * @code
     * using Traits = StructuralTraits<std::tuple<int, double, char>>;
     * static_assert(Traits::SIZE == 3);
     * using Second = Traits::ElementType<1>;  // double
     * @endcode
     */
    template<Structural T>
    struct StructuralTraits
    {
        using Type = std::remove_cvref_t<T>;
        template<Size::Type I>
        using ElementType = std::tuple_element<I, Type>::type; // NOLINT
        static constexpr Size::Type SIZE = std::tuple_size<Type>::value; // NOLINT
    };

    /**
     * @brief Sequentially invokes a callable for each element of a structural object.
     *
     * @tparam T Structural type
     * @tparam F Callable type
     * @param t Structural object (forwarded)
     * @param f Callable invoked with each element
     *
     * Expands to f(get<0>(t)), f(get<1>(t)), ...
     * Returns void. Useful for side effects.
     *
     * @code
     * std::tuple<int, double> t{42, 3.14};
     * forEach(t, [](auto& x) { std::cout << x << ' '; });  // prints 42 3.14
     * @endcode
     */
    template<Structural T, Invokable F>
    constexpr void
    forEach(T&& t, F&& f)
    {
        forEach<StructuralTraits<T>::SIZE>
        (
            [&]<Size::Type I>(IndexConstant<I>)
            {
                f(get<I>(std::forward<T>(t)));
            }
        );
    }

    /**
     * @brief Invokes a callable with all elements of a structural object as separate arguments.
     *
     * @tparam T Structural type
     * @tparam F Callable type
     * @param t Structural object (forwarded)
     * @param f Callable invoked with the expanded element pack
     *
     * Expands to f(get<0>(t), get<1>(t), ...)
     * Returns the result of invoking f.
     *
     * @code
     * auto sum = [](auto... xs) { return (xs + ...); };
     * std::tuple<int, double, char> t{1, 2.5, 3};
     * constexpr auto total = forAll(t, sum);  // 6.5
     * @endcode
     */
    template<Structural T, Invokable F>
    constexpr decltype(auto)
    forAll(T&& t, F&& f)
    {
        return forAll<StructuralTraits<T>::SIZE>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return f(get<I>(std::forward<T>(t))...);
            }
        );
    }
}

/** @} */ // end of StructuralTraits group