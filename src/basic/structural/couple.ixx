module;
#include <type_traits>
#include <tuple>
export module original.basic.structural.couple;
import original.basic.types;
import original.basic.number;
import original.basic.structural.utility;
import original.basic.structural.algorithm;


export namespace original
{
    template<typename T1, typename T2>
    struct Couple;
}

namespace original::details
{
    template<typename>
    struct IsCoupleTraits
        : std::false_type {};

    template<typename T1, typename T2>
    struct IsCoupleTraits<Couple<T1, T2>>
        : std::true_type {};
}

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
            (!SameType<RemoveCVRefType<U1>, Couple>) &&
            (!SameType<RemoveCVRefType<U2>, Couple>)
        constexpr Couple(U1&& first, U2&& second)
            : first(std::forward<U1>(first))
            , second(std::forward<U2>(second)) {}

        constexpr Couple(const Couple&) = default;

        constexpr Couple& operator=(const Couple&) = default;

        constexpr Couple(Couple&&) noexcept = default;

        constexpr Couple& operator=(Couple&&) noexcept = default;

        template<Size::Type I>
        requires (I < 2)
        constexpr decltype(auto) get() & noexcept
        {
            if constexpr(I == 0)
                return (this->first);
            else
                return (this->second);
        }

        template<Size::Type I>
        requires (I < 2)
        constexpr decltype(auto) get() const& noexcept
        {
            if constexpr(I == 0)
                return (this->first);
            else
                return (this->second);
        }

        template<Size::Type I>
        requires (I < 2)
        constexpr decltype(auto) get() && noexcept
        {
            if constexpr(I == 0)
                return std::move(this->first);
            else
                return std::move(this->second);
        }

        constexpr bool operator==(const Couple& rhs) const
        requires EqualityComparable<T1> && EqualityComparable<T2>
        {
            return structural::equal(*this, rhs);
        }

        constexpr auto operator<=>(const Couple& rhs) const
        requires ThreeWayComparable<T1> && ThreeWayComparable<T2>
        {
            return structural::lexicographicallyCompare(*this, rhs);
        }
    };

    template<typename T1, typename T2>
    Couple(T1&&, T2&&) -> Couple<std::decay_t<T1>, std::decay_t<T2>>;

    template<typename T1, typename T2, typename U1, typename U2>
    requires HasCommonType<T1, U1> && HasCommonType<T2, U2> &&
             EqualityComparable<CommonType<T1, U1>> && EqualityComparable<CommonType<T2, U2>>
    constexpr bool operator==(const Couple<T1, T2>& lhs, const Couple<U1, U2>& rhs)
    {
        return structural::equal(lhs, rhs);
    }

    template<typename T1, typename T2, typename U1, typename U2>
    requires HasCommonType<T1, U1> && HasCommonType<T2, U2> &&
             ThreeWayComparable<CommonType<T1, U1>> && ThreeWayComparable<CommonType<T2, U2>>
    constexpr auto operator<=>(const Couple<T1, T2>& lhs, const Couple<U1, U2>& rhs)
    {
        return structural::lexicographicallyCompare(lhs, rhs);
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(const Couple<T1, T2>& cp) noexcept
    {
        return cp.template get<I>();
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>& cp) noexcept
    {
        return cp.template get<I>();
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>&& cp) noexcept
    {
        return std::move(cp).template get<I>();
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(const Couple<T1, T2>& cp, IndexConstant<I>) noexcept
    {
        return cp.template get<I>();
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>& cp, IndexConstant<I>) noexcept
    {
        return cp.template get<I>();
    }

    template<Size::Type I, typename T1, typename T2>
    requires (I < 2)
    constexpr decltype(auto) get(Couple<T1, T2>&& cp, IndexConstant<I>) noexcept
    {
        return std::move(cp).template get<I>();
    }

    template<typename T>
    concept IsCouple = details::IsCoupleTraits<RemoveCVRefType<T>>::value;
}

export namespace std
{
    template<typename T1, typename T2>
    struct tuple_size<original::Couple<T1, T2>> // NOLINT
    : std::integral_constant<std::size_t, 2> {};

    template<size_t I, typename T1, typename T2>
    struct tuple_element<I, original::Couple<T1, T2>> // NOLINT
    {
        using type = original::ArgsTraits<T1, T2>::template ArgAt<I>;
    };

    template<std::size_t I, typename T1, typename T2>
    struct tuple_element<I, const original::Couple<T1, T2>> // NOLINT
    {
        using type = std::add_const_t<tuple_element_t<I, original::Couple<T1, T2>>>;
    };

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(const original::Couple<T1, T2>& cp) noexcept // NOLINT
    {
        return original::get<I>(cp);
    }

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(original::Couple<T1, T2>& cp) noexcept // NOLINT
    {
        return original::get<I>(cp);
    }

    template<std::size_t I, typename T1, typename T2>
    constexpr decltype(auto) get(original::Couple<T1, T2>&& cp) noexcept // NOLINT
    {
        return original::get<I>(std::move(cp));
    }
}
