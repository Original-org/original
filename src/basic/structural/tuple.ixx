module;
#include <utility>
#include <type_traits>
export module original.basic.structural.tuple;
import original.basic.structural.utility;
import original.basic.structural.traits;
import original.basic.structural.algorithm;
import original.basic.types;
import original.basic.number;


export namespace original
{
    template<typename... Ts>
    class Tuple;
}

namespace original::details
{
    template<Size::Type, typename T>
    struct TupleLeaf
    {
        T value_{};
    };

    template<typename Seq, typename... Ts>
    struct TupleImpl;

    template<Size::Type... Is, typename... Ts>
    struct TupleImpl<IndexSequence<Is...>, Ts...>
        : TupleLeaf<Is, Ts>...
    {
        protected:
        constexpr TupleImpl() = default;

        template<typename... Us>
        requires (sizeof...(Us) == sizeof...(Ts)) &&
                 (Constructible<Ts, Us&&> && ...)
        explicit constexpr TupleImpl(Us&&... us)
            : TupleLeaf<Is, Ts>{ std::forward<Us>(us) }...
        {}
    };

    template<typename>
    struct IsTupleTraits
        : std::false_type {};

    template<typename... Ts>
    struct IsTupleTraits<Tuple<Ts...>>
        : std::true_type {};
}

export namespace original
{
    template<typename... Ts>
    class Tuple
    : details::TupleImpl<DefaultIndexSequenceType<ArgsTraits<Ts...>::SIZE>, Ts...>
    {
        template<Size::Type I>
        using Base =
            details::TupleLeaf<I, typename ArgsTraits<Ts...>::template ArgAt<I>>;

        template<Size::Type I>
        constexpr decltype(auto) base() const noexcept
        {
            return static_cast<const Base<I>&>(*this);
        }

        template<Size::Type I>
        constexpr decltype(auto) base() noexcept
        {
            return static_cast<Base<I>&>(*this);
        }
    public:
        static constexpr Size::Type SIZE = ArgsTraits<Ts...>::SIZE;

        constexpr Tuple() = default;

        template<typename... Us>
        requires
            (sizeof...(Us) == SIZE) &&
            (Constructible<Ts, Us&&> && ...) &&
            (!SameType<RemoveCVRefType<Us>, Tuple> && ...)
        explicit constexpr Tuple(Us&&... us)
            : details::TupleImpl<
                  DefaultIndexSequenceType<SIZE>, Ts...>(
                  std::forward<Us>(us)...) {}

        constexpr Tuple(const Tuple&) = default;

        constexpr Tuple& operator=(const Tuple&) = default;

        constexpr Tuple(Tuple&&) noexcept = default;

        constexpr Tuple& operator=(Tuple&&) noexcept = default;

        template<Size::Type I>
        requires (I < SIZE)
        constexpr decltype(auto) get() & noexcept
        {
            return (this->base<I>().value_);
        }

        template<Size::Type I>
        requires (I < SIZE)
        constexpr decltype(auto) get() const& noexcept
        {
            return (this->base<I>().value_);
        }

        template<Size::Type I>
        requires (I < SIZE)
        constexpr decltype(auto) get() && noexcept
        {
            return std::move(this->base<I>().value_);
        }

        constexpr bool operator==(const Tuple& rhs) const
        requires (EqualityComparable<Ts> && ...)
        {
            return structural::equal(*this, rhs);
        }

        constexpr auto operator<=>(const Tuple& rhs) const
        requires (ThreeWayComparable<Ts> && ...)
        {
            return structural::lexicographicallyCompare(*this, rhs);
        }
    };

    template<typename... Us>
    Tuple(Us&&...) -> Tuple<std::decay_t<Us>...>;

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(const Tuple<Ts...>& tp) noexcept
    {
        return tp.template get<I>();
    }

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(Tuple<Ts...>& tp) noexcept
    {
        return tp.template get<I>();
    }

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(Tuple<Ts...>&& tp) noexcept
    {
        return std::move(tp).template get<I>();
    }

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(const Tuple<Ts...>& tp, IndexConstant<I>) noexcept
    {
        return tp.template get<I>();
    }

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(Tuple<Ts...>& tp, IndexConstant<I>) noexcept
    {
        return tp.template get<I>();
    }

    template<Size::Type I, typename... Ts>
    constexpr decltype(auto) get(Tuple<Ts...>&& tp, IndexConstant<I>) noexcept
    {
        return std::move(tp).template get<I>();
    }

    template<typename... Ts, typename... Us>
    requires
    (sizeof...(Ts) == sizeof...(Us)) &&
    (HasCommonType<Ts, Us> && ...) &&
    (EqualityComparable<CommonType<Ts, Us>> && ...)
    constexpr bool operator==(const Tuple<Ts...>& lhs, const Tuple<Us...>& rhs)
    {
        return structural::equal(lhs, rhs);
    }

    template<typename... Ts, typename... Us>
    requires
    (sizeof...(Ts) == sizeof...(Us)) &&
    (HasCommonType<Ts, Us> && ...) &&
    (ThreeWayComparable<CommonType<Ts, Us>> && ...)
    constexpr auto operator<=>(const Tuple<Ts...>& lhs, const Tuple<Us...>& rhs)
    {
        return structural::lexicographicallyCompare(lhs, rhs);
    }

    template<typename T>
    concept IsTuple = details::IsTupleTraits<RemoveCVRefType<T>>::value;
}

export namespace std
{
    template<typename... Ts>
    struct tuple_size<original::Tuple<Ts...>> // NOLINT
        : std::integral_constant<size_t, original::ArgsTraits<Ts...>::SIZE> {};

    template<size_t I, typename... Ts>
    struct tuple_element<I, original::Tuple<Ts...>> // NOLINT
    {
        using type = original::ArgsTraits<Ts...>::template ArgAt<I>;
    };

    template<size_t I, typename... Ts>
    struct tuple_element<I, const original::Tuple<Ts...>> // NOLINT
    {
        using type = std::add_const_t<
            tuple_element_t<I, original::Tuple<Ts...>>
        >;
    };

    template<size_t I, typename... Ts>
    constexpr decltype(auto) get(const original::Tuple<Ts...>& tp) noexcept // NOLINT
    {
        return original::get<I>(tp);
    }

    template<size_t I, typename... Ts>
    constexpr decltype(auto) get(original::Tuple<Ts...>& tp) noexcept // NOLINT
    {
        return original::get<I>(tp);
    }

    template<size_t I, typename... Ts>
    constexpr decltype(auto) get(original::Tuple<Ts...>&& tp) noexcept // NOLINT
    {
        return original::get<I>(std::move(tp));
    }
}
