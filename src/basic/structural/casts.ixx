module;
#include <utility>
export module original.basic.structural.casts;
import original.basic.types;
import original.basic.number;
import original.basic.structural.utility;
import original.basic.structural.traits;
import original.basic.structural.algorithm;
import original.basic.structural.couple;
import original.basic.structural.tuple;


namespace original::details
{
    template<bool Copy, Size::Type I, Structural T>
    constexpr auto forwardElemImpl(T&& t) -> decltype(auto)
    {
        if constexpr(Copy)
        {
            return RemoveCVRefType<decltype(get(t, IndexConstant<I>{}))>
                 (get(t, IndexConstant<I>{}));
        }
        else
        {
            return RemoveCVRefType<decltype(std::move(get(t, IndexConstant<I>{})))>
                (std::move(get(t, IndexConstant<I>{})));
        }
    }

    template<bool LeftCopy, bool RightCopy, Size::Type SL, Size::Type I, Structural L, Structural R>
    constexpr auto forwardElem(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr(I < SL)
        {
            return forwardElemImpl<LeftCopy, I>(std::forward<L>(l));
        }
        else
        {
            return forwardElemImpl<RightCopy, I - SL>(std::forward<R>(r));
        }
    }
}

export namespace original
{
    template<Size::Type Start, Size::Type Cnt, Structural T>
    requires (Start + Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(const T& t)
    {
        return structural::forAll<Cnt>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    RemoveCVRefType<decltype(get(t, IndexConstant<Start + I>{}))>(
                        get(t, IndexConstant<Start + I>{})
                    )...
                };
            }
        );
    }

    template<Size::Type Cnt, Structural T>
    requires (Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(const T& t)
    {
        return toTuple<0, Cnt>(t);
    }

    template<Structural T>
    constexpr auto toTuple(const T& t)
    {
        return toTuple<0, StructuralTraits<T>::SIZE>(t);
    }

    template<Size::Type Start, Size::Type Cnt, Structural T>
    requires (Start + Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(T&& t)
    requires IsRValueReference<T&&>
    {
        return structural::forAll<Cnt>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    RemoveCVRefType<decltype(get(t, IndexConstant<Start + I>{}))>(
                        std::move(get(t, IndexConstant<Start + I>{}))
                    )...
                };
            }
        );
    }

    template<Size::Type Cnt, Structural T>
    requires (Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(T&& t)
    requires IsRValueReference<T&&>
    {
        return toTuple<0, Cnt>(std::forward<T>(t));
    }

    template<Structural T>
    constexpr auto toTuple(T&& t)
    requires IsRValueReference<T&&>
    {
        constexpr auto SIZE = StructuralTraits<T>::SIZE;
        return toTuple<0, SIZE>(std::forward<T>(t));
    }

    template<Size::Type Start, Structural T>
    requires (Start + 2 <= StructuralTraits<T>::SIZE)
    constexpr auto toCouple(const T& t)
    {
        return Couple{
            RemoveCVRefType<decltype(get(t, IndexConstant<Start>{}))>(get(t, IndexConstant<Start>{})),
            RemoveCVRefType<decltype(get(t, IndexConstant<Start + 1>{}))>(get(t, IndexConstant<Start + 1>{}))
        };
    }

    template<Structural T>
    requires (StructuralTraits<T>::SIZE >= 2)
    constexpr auto toCouple(const T& t)
    {
        return toCouple<0>(t);
    }

    template<Size::Type Start, Structural T>
    requires (Start + 2 <= StructuralTraits<T>::SIZE)
    constexpr auto toCouple(T&& t)
    requires IsRValueReference<T&&>
    {
        return Couple{
            RemoveCVRefType<decltype(get(t, IndexConstant<Start>{}))>(
                std::move(get(t, IndexConstant<Start>{}))
            ),
            RemoveCVRefType<decltype(get(t, IndexConstant<Start + 1>{}))>(
                std::move(get(t, IndexConstant<Start + 1>{}))
            )
        };
    }

    template<Structural T>
    requires (StructuralTraits<T>::SIZE >= 2)
    constexpr auto toCouple(T&& t)
    requires IsRValueReference<T&&>
    {
        return toCouple<0>(std::forward<T>(t));
    }

    template<Structural T, Structural U>
    constexpr auto concat(const T& lhs, const U& rhs)
    {
        constexpr Size::Type LS = StructuralTraits<T>::SIZE;
        constexpr Size::Type RS = StructuralTraits<U>::SIZE;
        return structural::forAll<LS + RS>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    details::forwardElem<true, true, LS, I>(lhs, rhs)...
                };
            }
        );
    }

    template<Structural T, Structural U>
    constexpr auto concat(const T& lhs, U&& rhs)
    requires IsRValueReference<U&&>
    {
        constexpr Size::Type LS = StructuralTraits<T>::SIZE;
        constexpr Size::Type RS = StructuralTraits<U>::SIZE;
        return structural::forAll<LS + RS>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    details::forwardElem<true, false, LS, I>(lhs, std::forward<U>(rhs))...
                };
            }
        );
    }

    template<Structural T, Structural U>
    constexpr auto concat(T&& lhs, const U& rhs)
    requires IsRValueReference<T&&>
    {
        constexpr Size::Type LS = StructuralTraits<T>::SIZE;
        constexpr Size::Type RS = StructuralTraits<U>::SIZE;
        return structural::forAll<LS + RS>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    details::forwardElem<false, true, LS, I>(std::forward<T>(lhs), rhs)...
                };
            }
        );
    }

    template<Structural T, Structural U>
    constexpr auto concat(T&& lhs, U&& rhs)
    requires (IsRValueReference<T&&> && IsRValueReference<U&&>)
    {
        constexpr Size::Type LS = StructuralTraits<T>::SIZE;
        constexpr Size::Type RS = StructuralTraits<U>::SIZE;
        return structural::forAll<LS + RS>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    details::forwardElem<false, false, LS, I>(std::forward<T>(lhs), std::forward<U>(rhs))...
                };
            }
        );
    }

    template<IsCouple T, IsCouple U>
    constexpr auto operator+(T&& lhs, U&& rhs)
    {
        return concat(std::forward<T>(lhs), std::forward<U>(rhs));
    }

    template<IsTuple T, IsTuple U>
    constexpr auto operator+(T&& lhs, U&& rhs)
    {
        return concat(std::forward<T>(lhs), std::forward<U>(rhs));
    }

    template<IsCouple T, IsTuple U>
    constexpr auto operator+(T&& lhs, U&& rhs)
    {
        return concat(std::forward<T>(lhs), std::forward<U>(rhs));
    }

    template<IsTuple T, IsCouple U>
    constexpr auto operator+(T&& lhs, U&& rhs)
    {
        return concat(std::forward<T>(lhs), std::forward<U>(rhs));
    }
}