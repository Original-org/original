module;
#include <tuple>
export module original.basic.structural.builtin;
import original.basic.number;


export namespace std
{
    template<typename T, size_t N>
    struct tuple_size<T[N]> : integral_constant<size_t, N> {};

    template<typename T, size_t I, size_t N>
    struct tuple_element<I, T[N]>
    {
        using type = T;
    };

    template<typename T, size_t I, size_t N>
    struct tuple_element<I, const T[N]>
    {
        using type = std::add_const_t<
            tuple_element_t<I, T[N]>
        >::type;
    };

    template<size_t I, typename T, size_t N>
    constexpr decltype(auto) get(T (&arr)[N]) noexcept
    {
        return arr[I];
    }

    template<size_t I, typename T, size_t N>
    constexpr decltype(auto) get(const T (&arr)[N]) noexcept
    {
        return arr[I];
    }
}

export namespace original
{
    template<Size::Type I, typename T, Size::Type N>
    constexpr decltype(auto) get(T (&arr)[N]) noexcept
    {
        return arr[I];
    }

    template<Size::Type I, typename T, Size::Type N>
    constexpr decltype(auto) get(const T (&arr)[N]) noexcept
    {
        return arr[I];
    }

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
}