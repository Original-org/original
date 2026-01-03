module;
export module original.basic.tuple.utility;
import original.basic.number;
import original.basic.types;


export namespace original
{
    template<IntegralLike NUM, NumberLikeType<NUM>...>
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
        IntegralLike NUM,
        NumberLikeType<NUM> N,
        NumberLikeType<NUM>... I
    >
    struct MakeIntegralSequenceImpl
    : MakeIntegralSequenceImpl<
          NUM,
          N - NumberLikeType<NUM>{1},
          N - NumberLikeType<NUM>{1},
          I...> {};

    template <
        IntegralLike NUM,
        NumberLikeType<NUM>... I
    >
    struct MakeIntegralSequenceImpl<NUM, NumberLikeType<NUM>{0}, I...>
    {
        using Type = IntegralSequence<NUM, I...>;
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

    template<IntegralLike NUM, NumberLikeType<NUM>... I>
    struct IntegralSequenceTraits<IntegralSequence<NUM, I...>>
    {
        using NumberType = NUM;
        using ValueType  = NumberLikeType<NUM>;

        static constexpr Size::Type SIZE = sizeof...(I);
    };

    template<IntegralLike NUM, NumberLikeType<NUM> N>
    using MakeIntegralSequence =
        details::MakeIntegralSequenceImpl<NUM, N>::Type;

    template<Size::Type I>
    using MakeIndexSequence = details::MakeIntegralSequenceImpl<Size, I>::Type;

    template<
        Invokable F,
        IntegralLike NUM,
        NumberLikeType<NUM>... I
    >
    requires Functor<F> &&
             InvokableWith<F, IntegralConstant<NUM, I>...>
    constexpr decltype(auto)
    forAll(F&& f, IntegralSequence<NUM, I...>)
    {
        return f(IntegralConstant<NUM, I>{}...);
    }

    template<
        Invokable F,
        IntegralLike NUM,
        NumberLikeType<NUM>... I
    >
    requires Functor<F> &&
             (InvokableWith<F, IntegralConstant<NUM, I>> && ...)
    constexpr void
    forEach(F&& f, IntegralSequence<NUM, I...>)
    {
        (f(IntegralConstant<NUM, I>{}), ...);
    }
}