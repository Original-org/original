module;
export module original.basic.structural.utility;
import original.basic.number;
import original.basic.types;


export namespace original
{
    /**
     * @brief Represents a compile-time sequence of integral constants.
     *
     * @tparam Num The underlying integral-like type (e.g., Size, std::size_t)
     * @tparam V The pack of constant values of type NumberLikeType<Num>
     *
     * This type holds a parameter pack of compile-time constant values, similar
     * to `std::integer_sequence`. It is primarily used with metaprogramming utilities
     * in this module.
     *
     * @code
     * using Seq = IntegralSequence<Size, 0, 1, 2>;
     * static_assert(Seq::SIZE == 3);  // via IntegralSequenceTraits
     * @endcode
     */
    template<IntegralLike Num, NumberLikeType<Num>... V>
    struct IntegralSequence {};

    template<Size::Type... V>
    using IndexSequence = IntegralSequence<Size, V...>;
}

namespace original::details
{
    template<Size::Type I, typename... Args>
    struct ArgsAt;

    template<typename First, typename... Rest>
    struct ArgsAt<0, First, Rest...>
    {
        using Type = First;
    };

    template<Size::Type I, typename First, typename... Rest>
    struct ArgsAt<I, First, Rest...>
    {
        static_assert(I < sizeof...(Rest) + 1, "ArgsAt index out of range");
        using Type = ArgsAt<I - 1, Rest...>::Type;
    };

    template <
        IntegralLike Num,
        NumberLikeType<Num> Cur,
        NumberLikeType<Num> End,
        NumberLikeType<Num>... I
    >
    struct MakeIntegralSequenceImpl
    : MakeIntegralSequenceImpl<
          Num,
          Cur + NumberLikeType<Num>{1},
          End,
          I...,
          Cur>
    {
        static_assert(Cur <= End,
        "MakeIntegralSequenceImpl: Current value must be less than or equal to End");
    };

    template <
        IntegralLike Num,
        NumberLikeType<Num> End,
        NumberLikeType<Num>... I
    >
    struct MakeIntegralSequenceImpl<Num, End, End, I...>
    {
        using Type = IntegralSequence<Num, I...>;
    };
}

export namespace original
{
    /**
     * @addtogroup StructuralUtility
     * @{
     */

    /**
     * @brief Provides traits for inspecting type packs.
     *
     * @tparam Args The types in the parameter pack
     *
     * This class template exposes information about a parameter pack, such as its
     * size and the ability to retrieve the type at a specific index.
     *
     * @code
     * using Traits = ArgsTraits<int, double, char>;
     * static_assert(Traits::SIZE == 3);
     * using Second = Traits::ArgAt<1>;  // double
     * @endcode
     */
    template<typename...Args>
    struct ArgsTraits
    {
        static constexpr Size::Type SIZE = sizeof...(Args);

        template<Size::Type I>
        using ArgAt = details::ArgsAt<I, Args...>::Type;
    };

    /**
     * @brief Provides traits for inspecting an IntegralSequence.
     *
     * @tparam Seq The IntegralSequence type to inspect
     *
     * This specialization extracts the underlying number type, value type,
     * and size of the sequence.
     *
     * @code
     * using Seq = IntegralSequence<Size, 0, 1, 3>;
     * using Traits = IntegralSequenceTraits<Seq>;
     * static_assert(Traits::SIZE == 3);
     * static_assert(SameType<Traits::ValueType, Size::Type>);
     * @endcode
     */
    template<typename Seq>
    struct IntegralSequenceTraits;

    template<IntegralLike Num, NumberLikeType<Num>... I>
    struct IntegralSequenceTraits<IntegralSequence<Num, I...>>
    {
        using NumberType = Num;
        using ValueType  = NumberLikeType<Num>;

        static constexpr Size::Type SIZE = sizeof...(I);
    };

    /**
     * @brief Alias for a sequence of integral constants starting at a specified value.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Start The starting constant value
     * @tparam Cnt The number of elements in the sequence
     *
     * Produces IntegralSequence<Num, Start, Start+1, ..., Start+Cnt-1>.
     * Requires Cnt >= 0.
     *
     * @code
     * using Seq = IntegralSequenceType<Size, 5, 3>;  // IntegralSequence<Size, 5, 6, 7>
     * @endcode
     */
    template<IntegralLike Num, NumberLikeType<Num> Start, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    using IntegralSequenceType =
        details::MakeIntegralSequenceImpl<Num, Start, Start + Cnt>::Type;

    /**
     * @brief Alias for a sequence of integral constants starting at zero.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Cnt The number of elements in the sequence
     *
     * Produces IntegralSequence<Num, 0, 1, ..., Cnt-1>.
     * Requires Cnt >= 0.
     *
     * @code
     * using Seq = DefaultIntegralSequenceType<Size, 4>;  // IntegralSequence<Size, 0, 1, 2, 3>
     * @endcode
     */
    template<IntegralLike Num, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    using DefaultIntegralSequenceType =
        details::MakeIntegralSequenceImpl<Num, 0, Cnt>::Type;

    /**
     * @brief Alias for an index sequence using Size as the underlying type.
     *
     * @tparam Start The starting index
     * @tparam Cnt The number of elements
     *
     * Equivalent to IntegralSequenceType<Size, Start, Cnt>.
     *
     * @code
     * using Seq = IndexSequenceType<10, 2>;  // IntegralSequence<Size, 10, 11>
     * @endcode
     */
    template<Size::Type Start, Size::Type Cnt>
    using IndexSequenceType =
        details::MakeIntegralSequenceImpl<Size, Start, Start + Cnt>::Type;

    /**
     * @brief Alias for a default index sequence starting at zero.
     *
     * @tparam Cnt The number of elements
     *
     * Equivalent to DefaultIntegralSequenceType<Size, Cnt>.
     *
     * @code
     * using Seq = DefaultIndexSequenceType<5>;  // IntegralSequence<Size, 0, 1, 2, 3, 4>
     * @endcode
     */
    template<Size::Type Cnt>
    using DefaultIndexSequenceType =
        details::MakeIntegralSequenceImpl<Size, 0, Cnt>::Type;

    /**
     * @brief Creates a default integral sequence at compile time.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Cnt The number of elements
     *
     * Returns an instance of IntegralSequence<Num, 0, 1, ..., Cnt-1>.
     * Requires Cnt >= 0.
     *
     * @code
     * constexpr auto seq = makeIntegralSequence<Size, 3>();
     * // seq is IntegralSequence<Size, 0, 1, 2>{}
     * @endcode
     */
    template <IntegralLike Num, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    consteval auto makeIntegralSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Num, 0, Cnt>::Type;
        return Seq{};
    }

    /**
     * @brief Creates an integral sequence with custom start and count.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Start The starting value
     * @tparam Cnt The number of elements
     *
     * Returns an instance of IntegralSequence<Num, Start, Start+1, ..., Start+Cnt-1>.
     * Requires Cnt >= 0.
     *
     * @code
     * constexpr auto seq = makeIntegralSequence<Size, 10, 2>();
     * // seq is IntegralSequence<Size, 10, 11>{}
     * @endcode
     */
    template <IntegralLike Num, NumberLikeType<Num> Start, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    consteval auto makeIntegralSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Num, Start, Start + Cnt>::Type;
        return Seq{};
    }

    /**
     * @brief Creates a default index sequence at compile time.
     *
     * @tparam Cnt The number of elements
     *
     * Returns an instance of IntegralSequence<Size, 0, 1, ..., Cnt-1>.
     *
     * @code
     * constexpr auto seq = makeIndexSequence<4>();
     * // seq is IntegralSequence<Size, 0, 1, 2, 3>{}
     * @endcode
     */
    template <Size::Type Cnt>
    consteval auto makeIndexSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Size, 0, Cnt>::Type;
        return Seq{};
    }

    /**
     * @brief Creates an index sequence with custom start and count.
     *
     * @tparam Start The starting index
     * @tparam Cnt The number of elements
     *
     * Returns an instance of IntegralSequence<Size, Start, Start+1, ..., Start+Cnt-1>.
     *
     * @code
     * constexpr auto seq = makeIndexSequence<5, 3>();
     * // seq is IntegralSequence<Size, 5, 6, 7>{}
     * @endcode
     */
    template <Size::Type Start, Size::Type Cnt>
    consteval auto makeIndexSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Size, Start, Start + Cnt>::Type;
        return Seq{};
    }
    /** @} */ // end of StructuralUtility group
} // namespace original