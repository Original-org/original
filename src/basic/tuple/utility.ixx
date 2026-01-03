module;
export module original.basic.tuple.utility;
import original.basic.number;
import original.basic.types;


export namespace original
{
    template<IntegralLike Num, NumberLikeType<Num>...>
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
        NumberLikeType<Num> N,
        NumberLikeType<Num>... I
    >
    struct MakeIntegralSequenceImpl
    : MakeIntegralSequenceImpl<
          Num,
          N - NumberLikeType<Num>{1},
          N - NumberLikeType<Num>{1},
          I...> {};

    template <
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    struct MakeIntegralSequenceImpl<Num, NumberLikeType<Num>{0}, I...>
    {
        using Type = IntegralSequence<Num, I...>;
    };
}

export namespace original
{
    template<typename...Args>
    struct ArgsTraits
    {
        static constexpr Size::Type SIZE = sizeof...(Args);

        template<Size::Type I>
        using ArgsAt = details::ArgsAt<I, Args...>::Type;
    };

    template<typename Seq>
    struct IntegralSequenceTraits;

    template<IntegralLike Num, NumberLikeType<Num>... I>
    struct IntegralSequenceTraits<IntegralSequence<Num, I...>>
    {
        using NumberType = Num;
        using ValueType  = NumberLikeType<Num>;

        static constexpr Size::Type SIZE = sizeof...(I);
    };

    template<IntegralLike Num, NumberLikeType<Num> N>
    using MakeIntegralSequence =
        details::MakeIntegralSequenceImpl<Num, N>::Type;

    template<Size::Type I>
    using MakeIndexSequence = details::MakeIntegralSequenceImpl<Size, I>::Type;

    template<
        Invokable F,
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    requires Functor<F> &&
             InvokableWith<F, IntegralConstant<Num, I>...>
    constexpr decltype(auto)
    forAll(F&& f, IntegralSequence<Num, I...>)
    {
        return f(IntegralConstant<Num, I>{}...);
    }

    template<
        Invokable F,
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    requires Functor<F> &&
             (InvokableWith<F, IntegralConstant<Num, I>> && ...)
    constexpr void
    forEach(F&& f, IntegralSequence<Num, I...>)
    {
        (f(IntegralConstant<Num, I>{}), ...);
    }
}