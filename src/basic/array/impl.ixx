module;
#include <stdexcept>
#include <type_traits> // NOLINT
export module original.basic.array.impl;
import original.basic.types;
import original.basic.number;
import orginal.basic.iterator.impl;
import original.basic.iterator.traits;


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
        explicit constexpr ArrayImpl(Args&&... args) noexcept
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
        explicit constexpr Array(Args&&... args) noexcept
        : Base{static_cast<T>(std::forward<Args>(args))...} {}

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

        constexpr Array() = default;

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
    };

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
