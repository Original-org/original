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
        return toTuple<0, Cnt>(std::move(t));
    }

    template<Structural T>
    constexpr auto toTuple(T&& t)
    requires IsRValueReference<T&&>
    {
        return toTuple<0, typename StructuralTraits<T>::SIZE>(std::move(t));
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
        return toCouple<0>(std::move(t));
    }
}
