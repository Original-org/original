module;
#include <type_traits>
#include <array>
export module original.basic.array.traits;
import original.basic.array.impl;
import original.basic.types;


namespace original::details
{
    template<typename>
    struct ArrayTraits : std::false_type {};

    template<StdObject T, std::size_t N>
    struct ArrayTraits<Array<T, N>> : std::true_type {};

    template<typename>
    struct ArrayLikeTraits : std::false_type {};

    template<StdObject T, std::size_t N>
    struct ArrayLikeTraits<T[N]> : std::true_type
    {
        using Type = T[N];
    };

    template<StdObject T, std::size_t N>
    struct ArrayLikeTraits<std::array<T, N>> : std::true_type
    {
        using Type = std::array<T, N>;
    };

    template<StdObject T, std::size_t N>
    struct ArrayLikeTraits<Array<T, N>> : std::true_type
    {
        using Type = Array<T, N>;
    };
}

export namespace original
{
    template<typename A>
    concept IsArray = details::ArrayTraits<A>::value;

    template<typename A>
    concept IsArrayLike = details::ArrayLikeTraits<A>::value;

    template<typename A>
    using ArrayLikeType = details::ArrayLikeTraits<A>::Type;
}