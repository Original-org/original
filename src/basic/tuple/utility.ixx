module;
#include <functional>
export module original.basic.tuple.utility;
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
     * @addtogroup TupleUtility
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
     * using Second = Traits::ArgsAt<1>;  // double
     * @endcode
     */
    template<typename...Args>
    struct ArgsTraits
    {
        static constexpr Size::Type SIZE = sizeof...(Args);

        template<Size::Type I>
        using ArgsAt = details::ArgsAt<I, Args...>::Type;
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

    /**
     * @brief Invokes a functor with all integral constants from a sequence as separate arguments.
     *
     * @tparam F The functor type
     * @tparam Num The underlying integral-like type
     * @tparam I The sequence values
     * @param f The functor instance
     * @param seq The integral sequence
     *
     * Expands to f(IntegralConstant<Num, I>{}...).
     * Requires F to be a functor (class type with operator()) and invocable with the pack.
     *
     * @return The result of invoking f with the expanded constants.
     *
     * @code
     * auto sum = [](auto... constants) { return (constants.value + ...); };
     * constexpr auto seq = makeIntegralSequence<Size, 3>();
     * constexpr auto total = forAll(sum, seq);  // 0 + 1 + 2 = 3
     * @endcode
     */
    template<
        Invokable F,
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    requires Functor<F> &&
             InvokableWith<F, IntegralConstant<Num, I>...>
    constexpr decltype(auto)
    forAll(F&& f, IntegralSequence<Num, I...> seq)
    {
        return f(IntegralConstant<Num, I>{}...);
    }

    /**
     * @brief Invokes a functor with a default integral sequence (starting at 0).
     *
     * @tparam Num The underlying integral-like type
     * @tparam Cnt The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs IntegralSequence<Num, 0, 1, ..., Cnt-1> implicitly and forwards to
     * the primary forAll overload.
     *
     * @return The result of invoking f with the expanded constants.
     *
     * @code
     * auto product = [](auto... cs) { return (cs.value * ... * 1); };
     * constexpr auto res = forAll<Size, 4>(product);  // 0 * 1 * 2 * 3 = 0
     * @endcode
     */
    template<
        IntegralLike Num,
        NumberLikeType<Num> Cnt,
        Invokable F
    >
    constexpr decltype(auto)
    forAll(F&& f)
    {
        return forAll(std::forward<F>(f), makeIntegralSequence<Num, Cnt>());
    }

    /**
     * @brief Invokes a functor with a custom-range integral sequence.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Start The starting value
     * @tparam Cnt The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs IntegralSequence<Num, Start, Start+1, ..., Start+Cnt-1> implicitly
     * and forwards to the primary forAll overload.
     *
     * @return The result of invoking f with the expanded constants.
     *
     * @code
     * auto sumFrom5 = [](auto... cs) { return (cs.value + ...); };
     * constexpr auto res = forAll<Size, 5, 3>(sumFrom5);  // 5 + 6 + 7 = 18
     * @endcode
     */
    template<
        IntegralLike Num,
        NumberLikeType<Num> Start,
        NumberLikeType<Num> Cnt,
        Invokable F
    >
    constexpr decltype(auto)
    forAll(F&& f)
    {
        return forAll(std::forward<F>(f), makeIntegralSequence<Num, Start, Cnt>());
    }

    /**
     * @brief Invokes a functor with a default index sequence (Size-based, starting at 0).
     *
     * @tparam I The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs DefaultIndexSequenceType<I> implicitly and forwards to the primary
     * forAll overload.
     *
     * @return The result of invoking f with the expanded constants.
     *
     * @code
     * auto count = [](auto... idx) { return sizeof...(idx); };
     * static_assert(forAll<5>(count) == 5);
     * @endcode
     */
    template<Size::Type I, Invokable F>
    constexpr decltype(auto)
    forAll(F&& f)
    {
        return forAll(std::forward<F>(f), makeIndexSequence<I>());
    }

    /**
     * @brief Sequentially invokes a functor for each integral constant in the sequence.
     *
     * @tparam F The functor type
     * @tparam Num The underlying integral-like type
     * @tparam I The sequence values
     * @param f The functor instance
     * @param seq The integral sequence
     *
     * Expands to (f(IntegralConstant<Num, I>{}), ...).
     * Returns void. Useful for side effects (e.g., static assertions, initialization).
     *
     * @code
     * int count = 0;
     * auto inc = [&]<typename C>(C) { ++count; };
     * forEach(inc, makeIntegralSequence<Size, 4>());
     * // count == 4 after execution
     * @endcode
     */
    template<
        Invokable F,
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    requires Functor<F> && (InvokableWith<F, IntegralConstant<Num, I>> && ...)
    constexpr void
    forEach(F&& f, IntegralSequence<Num, I...> seq)
    {
        (f(IntegralConstant<Num, I>{}), ...);
    }

    /**
     * @brief Sequentially invokes a functor with a default integral sequence (starting at 0).
     *
     * @tparam Num The underlying integral-like type
     * @tparam Cnt The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs IntegralSequence<Num, 0, 1, ..., Cnt-1> implicitly and forwards to
     * the primary forEach overload.
     *
     * @code
     * std::vector<int> values;
     * auto push = [&]<typename C>(C) { values.push_back(C::value); };
     * forEach<Size, 3>(push);
     * // values == {0, 1, 2}
     * @endcode
     */
    template<
        IntegralLike Num,
        NumberLikeType<Num> Cnt,
        Invokable F
    >
    constexpr void
    forEach(F&& f)
    {
        forEach(std::forward<F>(f), makeIntegralSequence<Num, Cnt>());
    }

    /**
     * @brief Sequentially invokes a functor with a custom-range integral sequence.
     *
     * @tparam Num The underlying integral-like type
     * @tparam Start The starting value
     * @tparam Cnt The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs IntegralSequence<Num, Start, Start+1, ..., Start+Cnt-1> implicitly
     * and forwards to the primary forEach overload.
     *
     * @code
     * int sum = 0;
     * auto add = [&]<typename C>(c) { sum += numberLikeValue(c); };
     * forEach<Size, 10, 4>(add);
     * // sum == 10 + 11 + 12 + 13 = 46
     * @endcode
     */
    template<
        IntegralLike Num,
        NumberLikeType<Num> Start,
        NumberLikeType<Num> Cnt,
        Invokable F
    >
    constexpr void
    forEach(F&& f)
    {
        forEach(std::forward<F>(f), makeIntegralSequence<Num, Start, Cnt>());
    }

    /**
     * @brief Sequentially invokes a functor with a default index sequence (Size-based, starting at 0).
     *
     * @tparam I The number of elements in the sequence
     * @tparam F The functor type
     * @param f The functor instance
     *
     * Constructs DefaultIndexSequenceType<I> implicitly and forwards to the primary
     * forEach overload.
     *
     * @code
     * constexpr int array[5]{};
     * auto check = []<typename C>(c) { static_assert(numberLikeValue(c) < 5); };
     * forEach<5>(check);  // compiles only if all indices < 5
     * @endcode
     */
    template<Size::Type I, Invokable F>
    constexpr void
    forEach(F&& f)
    {
        forEach(std::forward<F>(f), makeIndexSequence<I>());
    }

    /** @} */ // end of TupleUtility group
} // namespace original