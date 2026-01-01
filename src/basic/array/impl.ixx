module;
#include <compare>
#include <stdexcept>
#include <type_traits> // NOLINT
export module original.basic.array.impl;
import original.basic.types;
import original.basic.number;
import orginal.basic.iterator.impl;
import original.basic.iterator.traits;
import original.basic.algorithm;


namespace original::details
{
    using namespace original::literals;

    template<IsObject T, Size::Type N>
    class ArrayImpl
    {
    protected:
        using SizeType = Size;
        using ValueType = T;

        static constexpr SizeType CAPACITY = N == 0 ? 1_size : SizeType{N};
        T data_[numberLikeValue(CAPACITY)]{};

        constexpr ArrayImpl() = default;

        template<typename... Args>
        requires (sizeof...(Args) <= N && (IsObject<Args> && ...))
        explicit constexpr ArrayImpl(Args&&... args)
        : data_{static_cast<T>(std::forward<Args>(args))...} {}
    public:
        constexpr SizeType size() const noexcept // NOLINT
        {
            return SizeType{N};
        }

        constexpr bool empty() const noexcept // NOLINT
        {
            return N == 0;
        }

        [[nodiscard]] constexpr ValueType* data() noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        [[nodiscard]] constexpr const ValueType* data() const noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& operator[](U index) noexcept
        {
            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr const ValueType& operator[](U index) const noexcept
        {
            return this->data_[numberLikeValue(index)];
        }
    };
}

export namespace original
{
    template<IsObject T, Size::Type N>
    class Array : public details::ArrayImpl<T, N>
    {
        using Base = details::ArrayImpl<T, N>;
    public:
        using IterType      = DefaultIterator<T, SpecifiedSource<Array>>;
        using ConstIterType = DefaultIterator<const T, SpecifiedSource<Array>>;
        using ValueType     = T;
        using SizeType      = Size;

        constexpr Array() = default;

        template<typename... Args>
        requires (sizeof...(Args) <= N && (IsObject<Args> && ...))
        explicit constexpr Array(Args&&... args)
        : Base{static_cast<T>(std::forward<Args>(args))...} {}

        constexpr Array(const Array&) = default;

        constexpr Array(Array&&) noexcept = default;

        constexpr Array& operator=(const Array&) = default;

        constexpr Array& operator=(Array&&) noexcept = default;

        constexpr IterType begin() noexcept
        {
            return IterType{this->data_};
        }

        constexpr IterType end() noexcept
        {
            return IterType{this->data_ + N};
        }

        [[nodiscard]] constexpr ConstIterType begin() const noexcept
        {
            return ConstIterType{this->data_};
        }

        [[nodiscard]] constexpr ConstIterType end() const noexcept
        {
            return ConstIterType{this->data_ + N};
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& at(U index)
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr const ValueType& at(U index) const
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }

        bool operator==(const Array& rhs) const
        requires EqualityComparable<ValueType>
        {
            return algorithms::equal(*this, rhs);
        }

        auto operator<=>(const Array& rhs) const
        requires ThreeWayComparable<ValueType>
        {
            return algorithms::lexicographicallyCompare(*this, rhs);
        }
    };

    template<IsObject T>
    class Array<T, 0> : public details::ArrayImpl<T, 0>
    {
        using Base = details::ArrayImpl<T, 0>;
    public:
        using IterType      = DefaultIterator<T, SpecifiedSource<Array>>;
        using ConstIterType = DefaultIterator<const T, SpecifiedSource<Array>>;
        using ValueType     = T;
        using SizeType      = Size;

        constexpr Array() noexcept = default;

        constexpr Array(const Array&) noexcept = default;

        constexpr Array(Array&&) noexcept = default;

        constexpr Array& operator=(const Array&) noexcept = default;

        constexpr Array& operator=(Array&&) noexcept = default;

        constexpr IterType begin() noexcept
        {
            return IterType{};
        }

        constexpr IterType end() noexcept
        {
            return IterType{};
        }

        [[nodiscard]] constexpr ConstIterType begin() const noexcept
        {
            return ConstIterType{};
        }

        [[nodiscard]] constexpr ConstIterType end() const noexcept
        {
            return ConstIterType{};
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        [[noreturn]]
        constexpr ValueType& at(U)
        {
            throw std::out_of_range{"Array<T, 0> out of range."};
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        [[noreturn]]
        constexpr const ValueType& at(U) const
        {
            throw std::out_of_range{"Array<T, 0> out of range."};
        }

        bool operator==(const Array&) const noexcept
        {
            return true;
        }

        auto operator<=>(const Array&) const noexcept
        {
            return std::strong_ordering::equal;
        }
    };

    template<IsObject T, Size::Type N1, Size::Type N2>
    requires (N1 != N2) && EqualityComparable<T>
    constexpr bool operator==(const Array<T, N1>&, const Array<T, N2>&)
    noexcept
    {
        return false;
    }

    template<IsObject T, Size::Type N1, Size::Type N2>
    requires (N1 != N2) && ThreeWayComparable<T>
    constexpr auto operator<=>(const Array<T, N1>&, const Array<T, N2>&)
    noexcept
    {
        return N1 <=> N2;
    }

    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr T& get(Array<T, N>& a) noexcept // NOLINT
    {
        return a[I];
    }

    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr const T& get(const Array<T, N>& a) noexcept // NOLINT
    {
        return a[I];
    }

    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr T&& get(Array<T, N>&& a) noexcept // NOLINT
    {
        return std::move(a[I]);
    }
}

export namespace std
{
    template<original::IsObject T, original::Size::Type N>
    struct tuple_size<original::Array<T, N>> // NOLINT
        : std::integral_constant<std::size_t, N> {};

    template<std::size_t I, original::IsObject T, original::Size::Type N>
    struct tuple_element<I, original::Array<T, N>> // NOLINT
    {
        static_assert(I < N, "tuple_element index out of range");
        using type = T;
    };

    template<std::size_t I, original::IsObject T, original::Size::Type N>
    requires (I < N)
    constexpr T& get(original::Array<T, N>& a) noexcept // NOLINT
    {
        return original::get<I>(a);
    }

    template<std::size_t I, original::IsObject T, original::Size::Type N>
    requires (I < N)
    constexpr const T& get(const original::Array<T, N>& a) noexcept // NOLINT
    {
        return original::get<I>(a);
    }

    template<std::size_t I, original::IsObject T, original::Size::Type N>
    requires (I < N)
    constexpr T&& get(original::Array<T, N>&& a) noexcept // NOLINT
    {
        return original::get<I>(std::move(a));
    }
}
