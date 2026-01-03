module;
#include <functional>
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

    template<IntegralLike Num, NumberLikeType<Num> Start, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    using IntegralSequenceType =
        details::MakeIntegralSequenceImpl<Num, Start, Start + Cnt>::Type;

    template<IntegralLike Num, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    using DefaultIntegralSequenceType =
        details::MakeIntegralSequenceImpl<Num, 0, Cnt>::Type;

    template<Size::Type Start, Size::Type Cnt>
    using IndexSequenceType =
        details::MakeIntegralSequenceImpl<Size, Start, Start + Cnt>::Type;

    template<Size::Type Cnt>
    using DefaultIndexSequenceType =
        details::MakeIntegralSequenceImpl<Size, 0, Cnt>::Type;

    template <IntegralLike Num, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    consteval auto makeIntegralSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Num, 0, Cnt>::Type;
        return Seq{};
    }

    template <IntegralLike Num, NumberLikeType<Num> Start, NumberLikeType<Num> Cnt>
    requires (numberLikeValue(Cnt) >= 0)
    consteval auto makeIntegralSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Num, Start, Start + Cnt>::Type;
        return Seq{};
    }

    template <Size::Type Cnt>
    consteval auto makeIndexSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Size, 0, Cnt>::Type;
        return Seq{};
    }

    template <Size::Type Start, Size::Type Cnt>
    consteval auto makeIndexSequence()
    {
        using Seq = details::MakeIntegralSequenceImpl<Size, Start, Start + Cnt>::Type;
        return Seq{};
    }

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
        IntegralLike Num,
        NumberLikeType<Num> Cnt,
        Invokable F
    >
    constexpr decltype(auto)
    forAll(F&& f)
    {
        return forAll(std::forward<F>(f), makeIntegralSequence<Num, Cnt>());
    }

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

    template<Size::Type I, Invokable F>
    constexpr decltype(auto)
    forAll(F&& f)
    {
        return forAll(std::forward<F>(f), makeIndexSequence<I>());
    }

    template<
        Invokable F,
        IntegralLike Num,
        NumberLikeType<Num>... I
    >
    requires Functor<F> && (InvokableWith<F, IntegralConstant<Num, I>> && ...)
    constexpr void
    forEach(F&& f, IntegralSequence<Num, I...>)
    {
        (f(IntegralConstant<Num, I>{}), ...);
    }

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

    template<Size::Type I, Invokable F>
    constexpr void
    forEach(F&& f)
    {
        forEach(std::forward<F>(f), makeIndexSequence<I>());
    }
}