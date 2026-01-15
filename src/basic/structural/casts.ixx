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
    template<Size::Type SL, Size::Type I, Structural L, Structural R>
    constexpr auto forwardElem(L&& l, R&& r) -> decltype(auto)
    {
        if constexpr (I < SL)
        {
            return RemoveCVRefType<decltype(get(std::forward<L>(l), IndexConstant<I>{}))>(
                get(std::forward<L>(l), IndexConstant<I>{})
            );
        }
        else
        {
            return RemoveCVRefType<decltype(get(std::forward<R>(r), IndexConstant<I - SL>{}))>(
                get(std::forward<R>(r), IndexConstant<I - SL>{})
            );
        }
    }
}

export namespace original
{
    template<Size::Type Start, Size::Type Cnt, Structural T>
    requires (Start + Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(T&& t)
    {
        return structural::forAll<Cnt>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    RemoveCVRefType<decltype(get(std::forward<T>(t), IndexConstant<Start + I>{}))>(
                        get(std::forward<T>(t), IndexConstant<Start + I>{})
                    )...
                };
            }
        );
    }

    template<Size::Type Cnt, Structural T>
    requires (Cnt <= StructuralTraits<T>::SIZE)
    constexpr auto toTuple(T&& t)
    {
        return toTuple<0, Cnt>(std::forward<T>(t));
    }

    template<Structural T>
    constexpr auto toTuple(T&& t)
    {
        constexpr auto SIZE = StructuralTraits<T>::SIZE;
        return toTuple<0, SIZE>(std::forward<T>(t));
    }

    template<Size::Type Start, Structural T>
    requires (Start + 2 <= StructuralTraits<T>::SIZE)
    constexpr auto toCouple(T&& t)
    {
        return Couple{
            RemoveCVRefType<decltype(get(std::forward<T>(t), IndexConstant<Start>{}))>(
                get(std::forward<T>(t), IndexConstant<Start>{})
            ),
            RemoveCVRefType<decltype(get(std::forward<T>(t), IndexConstant<Start + 1>{}))>(
                get(std::forward<T>(t), IndexConstant<Start + 1>{})
            )
        };
    }

    template<Structural T>
    requires (StructuralTraits<T>::SIZE >= 2)
    constexpr auto toCouple(T&& t)
    {
        return toCouple<0>(std::forward<T>(t));
    }

    template<Structural T, Structural U>
    constexpr auto concat(T&& lhs, U&& rhs)
    {
        constexpr Size::Type LS = StructuralTraits<T>::SIZE;
        constexpr Size::Type RS = StructuralTraits<U>::SIZE;
        return structural::forAll<LS + RS>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Tuple{
                    details::forwardElem<LS, I>(std::forward<T>(lhs), std::forward<U>(rhs))...
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