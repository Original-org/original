module;
#include <type_traits>
#include <array>
export module original.basic.array.traits;
import original.basic.array.impl;
import original.basic.types;
import original.basic.number.impl;


namespace original::details
{
    template<typename>
    struct ArrayTraits : std::false_type {};

    template<IsObject T, Size::Type N>
    struct ArrayTraits<Array<T, N>> : std::true_type {};
}

export namespace original
{
    template<typename A>
    concept IsArray = details::ArrayTraits<A>::value;

    template<typename>
    struct ArrayLikeTraits : std::false_type {};

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<T[N]> : std::true_type
    {
        using Type = T[N];

        static constexpr Size::Type SIZE = N;

        static constexpr T* data(T (&arr) [N])
        {
            return arr;
        }
    };

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<std::array<T, N>> : std::true_type
    {
        using Type = std::array<T, N>;

        static constexpr Size::Type SIZE = N;

        static constexpr T* data(std::array<T, N>& arr)
        {
            return arr.data();
        }
    };

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<Array<T, N>> : std::true_type
    {
        using Type = Array<T, N>;

        static constexpr Size::Type SIZE = N;

        static constexpr T* data(Array<T, N>& arr)
        {
            return arr.data();
        }
    };

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<const T[N]> : std::true_type
    {
        using Type = const T[N];

        static constexpr Size::Type SIZE = N;

        static constexpr const T* data(const T (&arr) [N])
        {
            return arr;
        }
    };

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<const std::array<T, N>> : std::true_type
    {
        using Type = const std::array<T, N>;

        static constexpr Size::Type SIZE = N;

        static constexpr const T* data(const std::array<T, N>& arr)
        {
            return arr.data();
        }
    };

    template<IsObject T, Size::Type N>
    struct ArrayLikeTraits<const Array<T, N>> : std::true_type
    {
        using Type = const Array<T, N>;

        static constexpr Size::Type SIZE = N;

        static constexpr const T* data(const Array<T, N>& arr)
        {
            return arr.data();
        }
    };

    template<typename A>
    concept IsArrayLike = ArrayLikeTraits<A>::value;
}