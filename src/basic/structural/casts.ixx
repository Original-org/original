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
    template<Size::Type SL, Size::Type I, typename L, typename R>
    constexpr auto forwardElemLL(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr(I < SL)
        {
            return RemoveCVRefType<decltype(get(l, IndexConstant<I>{}))>
                 (get(l, IndexConstant<I>{}));
        }
        else
        {
            return RemoveCVRefType<decltype(get(r, IndexConstant<I - SL>{}))>
                (get(r, IndexConstant<I - SL>{}));
        }
    }

    template<Size::Type SL, Size::Type I, typename L, typename R>
    constexpr auto forwardElemLR(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr(I < SL)
        {
            return RemoveCVRefType<decltype(get(l, IndexConstant<I>{}))>
                 (get(l, IndexConstant<I>{}));
        }
        else
        {
            return RemoveCVRefType<decltype(std::move(get(r, IndexConstant<I - SL>{})))>
                (std::move(get(r, IndexConstant<I - SL>{})));
        }
    }

    template<Size::Type SL, Size::Type I, typename L, typename R>
    constexpr auto forwardElemRL(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr(I < SL)
        {
            return RemoveCVRefType<decltype(std::move(get(l, IndexConstant<I>{})))>
                 (std::move(get(l, IndexConstant<I>{})));
        }
        else
        {
            return RemoveCVRefType<decltype(get(r, IndexConstant<I - SL>{}))>
                (get(r, IndexConstant<I - SL>{}));
        }
    }

    template<Size::Type SL, Size::Type I, typename L, typename R>
    constexpr auto forwardElemRR(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr(I < SL)
        {
            return RemoveCVRefType<decltype(std::move(get(l, IndexConstant<I>{})))>
                 (std::move(get(l, IndexConstant<I>{})));
        }
        else
        {
            return RemoveCVRefType<decltype(std::move(get(r, IndexConstant<I - SL>{})))>
                (std::move(get(r, IndexConstant<I - SL>{})));
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
                    details::forwardElemLL<StructuralTraits<T>::SIZE, I>(lhs, rhs)...
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
                    details::forwardElemLR<StructuralTraits<T>::SIZE, I>(lhs, rhs)...
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
                    details::forwardElemRL<StructuralTraits<T>::SIZE, I>(lhs, rhs)...
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
                    details::forwardElemRR<StructuralTraits<T>::SIZE, I>(lhs, rhs)...
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
