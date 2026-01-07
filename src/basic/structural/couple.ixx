module;
#include <type_traits>
#include <tuple>
export module original.basic.structural.couple;
import original.basic.types;


export namespace original
{
    template<typename T1, typename T2>
    struct Couple
    {
        T1 first{};
        T2 second{};

        constexpr Couple() = default;

        template<typename U1, typename U2>
        requires
            Constructible<T1, U1&&> &&
            Constructible<T2, U2&&> &&
            (!SameType<RemoveCVRefType<U1>, Couple> ||
             !SameType<RemoveCVRefType<U2>, Couple>)
        constexpr Couple(U1&& first, U2&& second)
            : first(std::forward<U1>(first))
            , second(std::forward<U2>(second)) {}

        constexpr Couple(const Couple&) = default;

        constexpr Couple& operator=(const Couple&) = default;

        constexpr Couple(Couple&&) noexcept = default;

        constexpr Couple& operator=(Couple&&) noexcept = default;

        constexpr bool operator==(const Couple& rhs) const
        requires EqualityComparable<T1> && EqualityComparable<T2>
        {
            return this->first == rhs.first && this->second == rhs.second;
        }

        constexpr auto operator<=>(const Couple& rhs) const
        requires ThreeWayComparable<T1> && ThreeWayComparable<T2>
        {
            if (auto c = this->first <=> rhs.first; c != 0)
                return c;
            return this->second <=> rhs.second;
        }
    };

    template<typename T1, typename T2>
    Couple(T1&&, T2&&) -> Couple<T1, T2>;

    template<typename T1, typename T2, typename U1, typename U2>
    requires HasCommonType<T1, U1> && HasCommonType<T2, U2> &&
             EqualityComparable<CommonType<T1, U1>> && EqualityComparable<CommonType<T2, U2>>
    constexpr bool operator==(const Couple<T1, T2>& lhs, const Couple<U1, U2>& rhs)
    {
        using CommonType1 = CommonType<T1, U1>;
        using CommonType2 = CommonType<T2, U2>;
        return static_cast<CommonType1>(lhs.first) == static_cast<CommonType1>(rhs.first) &&
               static_cast<CommonType2>(lhs.second) == static_cast<CommonType2>(rhs.second);
    }

    template<typename T1, typename T2, typename U1, typename U2>
    requires HasCommonType<T1, U1> && HasCommonType<T2, U2> &&
             ThreeWayComparable<CommonType<T1, U1>> && ThreeWayComparable<CommonType<T2, U2>>
    constexpr auto operator<=>(const Couple<T1, T2>& lhs, const Couple<U1, U2>& rhs)
    {
        using CommonType1 = CommonType<T1, U1>;
        using CommonType2 = CommonType<T2, U2>;
        const auto first_compare_result =
            static_cast<CommonType1>(lhs.first) <=> static_cast<CommonType1>(rhs.first);
        if (first_compare_result != 0)
            return first_compare_result;
        return static_cast<CommonType2>(lhs.second) <=> static_cast<CommonType2>(rhs.second);
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(const Couple<T1, T2>& cp)
    {
        if constexpr (I == 0)
            return cp.first;
        else
            return cp.second;
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>& cp)
    {
        if constexpr (I == 0)
            return cp.first;
        else
            return cp.second;
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>&& cp)
    {
        if constexpr (I == 0)
            return std::move(cp.first);
        else
            return std::move(cp.second);
    }
}

export namespace std
{
    template<typename T1, typename T2>
    struct tuple_size<original::Couple<T1, T2>> // NOLINT
    : std::integral_constant<std::size_t, 2> {};

    template<typename T1, typename T2>
    struct tuple_element<0, original::Couple<T1, T2>> // NOLINT
    {
        using type = T1;
    };

    template<typename T1, typename T2>
    struct tuple_element<1, original::Couple<T1, T2>> // NOLINT
    {
        using type = T2;
    };

    template<std::size_t I, typename T1, typename T2>
    struct tuple_element<I, const original::Couple<T1, T2>> // NOLINT
    {
        using type = std::add_const_t<tuple_element_t<I, original::Couple<T1, T2>>>;
    };

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(const original::Couple<T1, T2>& cp) // NOLINT
    {
        return original::get<I>(cp);
    }

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(original::Couple<T1, T2>& cp) // NOLINT
    {
        return original::get<I>(cp);
    }

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(original::Couple<T1, T2>&& cp) // NOLINT
    {
        return original::get<I>(std::move(cp));
    }
}
