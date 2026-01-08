module;
#include <type_traits>
#include <tuple>
export module original.basic.structural.traits;
import original.basic.number;
import original.basic.types;
import original.basic.structural.utility;
import original.basic.algorithm.extremum;

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
        ((requires(T& t, IndexConstant<I> c) { get(t, c); } &&
        requires(const T& t, IndexConstant<I> c) { get(t, c); } &&
        (MoveConstructible<T> ?
        requires(T&& t, IndexConstant<I> c) { get(std::move(t), c); } : true)) ||
        (requires(T& t) { get<I>(t); } &&
        requires(const T& t) { get<I>(t); } &&
        (MoveConstructible<T> ?
        requires(T&& t) { get<I>(std::move(t)); } : true))) &&
        StructuralGetCheck<T, I + 1, N>::value;
    };

    template<typename T, Size::Type N>
    struct StructuralGetCheck<T, N, N>
        : std::true_type {};

    template<typename T, Size::Type N>
    inline constexpr bool HasStructuralGetValue =
        StructuralGetCheck<T, 0, N>::value;

    template<typename L, typename R, Size::Type... I>
    consteval auto
    structuralCompareCategoryImpl(IndexSequence<I...>)
    {
        return std::type_identity<
            CommonComparisonCategory<
                decltype(get<I>(std::declval<L>()) <=> get<I>(std::declval<R>()))
            ...>
        >{};
    }

    template<typename Pred, typename L, typename R, Size::Type I, Size::Type N>
    struct StructuralEqualityComparableCheck
    {
        static constexpr bool value =
        requires(const L& lhs, const R& rhs, Pred pred) {
                { pred(get<I>(lhs), get<I>(rhs)) } -> Convertible<bool>;
        } && StructuralEqualityComparableCheck<Pred, L, R, I + 1, N>::value;
    };

    template<typename Pred, typename L, typename R, Size::Type N>
    struct StructuralEqualityComparableCheck<Pred, L, R, N, N>
    : std::true_type {};

    template<typename Pred, typename L, typename R, Size::Type N>
    inline constexpr bool StructuralEqualityComparableCheckValue =
        StructuralEqualityComparableCheck<Pred, L, R, 0, N>::value;

    template<typename Pred, typename L, typename R, Size::Type I, Size::Type N>
    struct StructuralThreeWayComparableCheck
    {
        static constexpr bool value =
        requires(const L& lhs, const R& rhs, Pred pred) {
                    { pred(get<I>(lhs), get<I>(rhs)) } -> StdThreeWayCompareResult;
        } && StructuralThreeWayComparableCheck<Pred, L, R, I + 1, N>::value;
    };

    template<typename Pred, typename L, typename R, Size::Type N>
    struct StructuralThreeWayComparableCheck<Pred, L, R, N, N>
    : std::true_type {};

    template<typename Pred, typename L, typename R, Size::Type N>
    inline constexpr bool StructuralThreeWayComparableCheckValue =
        StructuralThreeWayComparableCheck<Pred, L, R, 0, N>::value;
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

    template<Structural L, Structural R>
    using StructuralCompareCategory =
        decltype(
            details::structuralCompareCategoryImpl<L, R>(
                makeIndexSequence<
                    algorithms::minimum(
                        StructuralTraits<L>::SIZE,
                        StructuralTraits<R>::SIZE
                    )
                >()
            )
        )::type;

    template<typename Pred, typename L, typename R = L>
    concept StructuralEqualityComparable =
    Structural<L> &&
    Structural<R> &&
    StructuralTraits<L>::SIZE == StructuralTraits<R>::SIZE &&
    (StructuralTraits<L>::SIZE == 0 ||
    details::StructuralEqualityComparableCheckValue<
        Pred, L, R, StructuralTraits<L>::SIZE
    >);

    template<typename Pred, typename L, typename R = L>
    concept StructuralThreeWayComparable =
    Structural<L> &&
    Structural<R> &&
    requires {
        typename StructuralCompareCategory<L, R>;
    } &&
    StructuralTraits<L>::SIZE == StructuralTraits<R>::SIZE &&
    (StructuralTraits<L>::SIZE == 0 ||
    details::StructuralThreeWayComparableCheckValue<
        Pred, L, R, StructuralTraits<L>::SIZE
    >);
}

/** @} */ // end of StructuralTraits group