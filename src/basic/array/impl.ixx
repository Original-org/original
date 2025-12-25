module;
#include <cstddef>
#include <stdexcept>
export module original.basic.array.impl;
import original.basic.types;
import original.basic.number.impl;
import orginal.basic.iterator.impl;


namespace original::details
{
    template<StdObject T, std::size_t N>
    class ArrayImpl
    {
    protected:
        using SizeType = std::size_t;
        using ValueType = T;

        static constexpr SizeType CAPACITY = N == 0 ? 1 : N;
        T data_[CAPACITY]{};

        constexpr ArrayImpl() = default;
    public:
        constexpr SizeType size() const noexcept // NOLINT
        {
            return N;
        }

        constexpr bool empty() const noexcept // NOLINT
        {
            return N == 0;
        }

        constexpr ValueType* data() noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        constexpr const ValueType* data() const noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& operator[](U index) noexcept
        {
            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr const ValueType& operator[](U index) const noexcept
        {
            return this->data_[numberLikeValue(index)];
        }
    };
}

export namespace original
{
    template<StdObject T, std::size_t N>
    class Array : public details::ArrayImpl<T, N>
    {
        using Base = details::ArrayImpl<T, N>;
    public:
        using IterType      = DefaultIterator<T, SpecifiedSource<Array>>;
        using ConstIterType = DefaultIterator<const T, SpecifiedSource<Array>>;
        using ValueType     = T;
        using SizeType      = std::size_t;

        constexpr Array() = default;

        constexpr IterType begin() noexcept
        {
            return IterType{this->data_};
        }

        constexpr IterType end() noexcept
        {
            return IterType{this->data_ + N};
        }

        constexpr ConstIterType begin() const noexcept
        {
            return ConstIterType{this->data_};
        }

        constexpr ConstIterType end() const noexcept
        {
            return ConstIterType{this->data_ + N};
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& at(U index)
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr const ValueType& at(U index) const
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }
    };

    template<StdObject T>
    class Array<T, 0> : public details::ArrayImpl<T, 0>
    {
        using Base = details::ArrayImpl<T, 0>;
    public:
        using IterType      = DefaultIterator<T, SpecifiedSource<Array>>;
        using ConstIterType = DefaultIterator<const T, SpecifiedSource<Array>>;
        using ValueType     = T;
        using SizeType      = std::size_t;

        constexpr Array() = default;

        constexpr IterType begin() noexcept
        {
            return IterType{};
        }

        constexpr IterType end() noexcept
        {
            return IterType{};
        }

        constexpr ConstIterType begin() const noexcept
        {
            return ConstIterType{};
        }

        constexpr ConstIterType end() const noexcept
        {
            return ConstIterType{};
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        [[noreturn]]
        constexpr ValueType& at(U)
        {
            throw std::out_of_range{"Array<T, 0> out of range."};
        }

        template<UnsignedIntegralLike U>
        requires StdSame<NumberLikeType<U>, NumberLikeType<SizeType>>
        [[noreturn]]
        constexpr const ValueType& at(U) const
        {
            throw std::out_of_range{"Array<T, 0> out of range."};
        }
    };
}
