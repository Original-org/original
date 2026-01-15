module;
#include <utility>
export module original.basic.structural.algorithm;
import original.basic.structural.utility;
import original.basic.structural.traits;
import original.basic.types;
import original.basic.number;
import original.basic.algorithm.functor;
import original.basic.algorithm.extremum;


namespace original::structural::details {
    template<
        Size::Type I,
        typename Category,
        Structural L,
        Structural R,
        Invokable Pred
    >
    constexpr Category lexicographicallyCompareImpl(const L& lhs, const R& rhs, Pred pred)
    {
        if constexpr (constexpr Size::Type NL = StructuralTraits<L>::SIZE;
                      I == NL)
        {
            return static_cast<Category>(std::strong_ordering::equal);
        }
        else
        {
            if (auto cmp = pred(get(lhs, IndexConstant<I>{}), get(rhs, IndexConstant<I>{})); cmp != 0)
                return static_cast<Category>(cmp);

            return lexicographicallyCompareImpl<I + 1, Category>(lhs, rhs, pred);
        }
    }
}

export namespace original
{
    template<Size::Type I, typename T, Size::Type N>
    constexpr decltype(auto) get(T (&arr)[N], IndexConstant<I>) noexcept
    {
        return arr[I];
    }

    template<Size::Type I, typename T, Size::Type N>
    constexpr decltype(auto) get(const T (&arr)[N], IndexConstant<I>) noexcept
    {
        return arr[I];
    }

    template<Size::Type I, typename T1, typename T2>
    constexpr decltype(auto) get(std::pair<T1, T2>& cp, IndexConstant<I>) noexcept
    {
        return std::get<I>(cp);
    }

    template<Size::Type I, typename T1, typename T2>
    constexpr decltype(auto) get(const std::pair<T1, T2>& cp, IndexConstant<I>) noexcept
    {
        return std::get<I>(cp);
    }

    template<Size::Type I, typename T1, typename T2>
    constexpr decltype(auto) get(std::pair<T1, T2>&& cp, IndexConstant<I>) noexcept
    {
        return std::get<I>(std::move(cp));
    }

    template<Size::Type I, typename... Args>
    constexpr decltype(auto) get(std::tuple<Args...>& tp, IndexConstant<I>) noexcept
    {
        return std::get<I>(tp);
    }

    template<Size::Type I, typename... Args>
    constexpr decltype(auto) get(const std::tuple<Args...>& tp, IndexConstant<I>) noexcept
    {
        return std::get<I>(tp);
    }

    template<Size::Type I, typename... Args>
    constexpr decltype(auto) get(std::tuple<Args...>&& tp, IndexConstant<I>) noexcept
    {
        return std::get<I>(std::move(tp));
    }
}

export namespace original::structural
{
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
            [&]<Size::Type... I>(IndexConstant<I>... c)
            {
                return f(get(std::forward<T>(t), c)...);
            }
        );
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
     * forEach(t, []<Size::Type I>(IndexConstant<I>, auto& x)
     * { std::cout << I << ": " << x << std::endl; });
     * // prints:
     * // 0: 42
     * // 1: 3.14
     * @endcode
     */
    template<Structural T, Invokable F>
    constexpr void
    forEach(T&& t, F&& f)
    {
        forEach<StructuralTraits<T>::SIZE>
        (
            [&]<Size::Type I>(IndexConstant<I> c)
            {
                f(c, get(std::forward<T>(t), c));
            }
        );
    }

    template<Structural L, Structural R>
    requires (StructuralTraits<L>::SIZE != StructuralTraits<R>::SIZE)
    constexpr bool equal(const L&, const R&) noexcept
    {
        return false;
    }

    template<Structural L, Structural R, Invokable Pred = algorithms::EqualTo>
    requires StructuralEqualityComparable<Pred, L, R>
    constexpr bool equal(const L& lhs, const R& rhs, Pred pred = {})
    {
        bool result = true;
        forEach<StructuralTraits<L>::SIZE>(
            [&]<Size::Type I>(IndexConstant<I> c)
            {
                if (!pred(get(lhs, c), get(rhs, c)))
                    result = false;
            }
        );
        return result;
    }

    template<Structural L, Structural R>
    requires (StructuralTraits<L>::SIZE != StructuralTraits<R>::SIZE)
    constexpr auto lexicographicallyCompare(const L&, const R&) noexcept
    {
        using Category = StructuralCompareCategory<L, R>;
        return static_cast<Category>(StructuralTraits<L>::SIZE <=> StructuralTraits<R>::SIZE);
    }

    template<Structural L, Structural R, Invokable Pred = algorithms::ThreeWayCompare>
    requires StructuralThreeWayComparable<Pred, L, R>
    constexpr auto lexicographicallyCompare(const L& lhs, const R& rhs, Pred pred = {})
    {
        using Category = StructuralCompareCategory<L, R>;
        return details::lexicographicallyCompareImpl<0, Category>(lhs, rhs, pred);
    }
}