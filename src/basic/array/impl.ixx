module;
#include <compare>
#include <stdexcept>
#include <memory>
#include <type_traits> // NOLINT
export module original.basic.array.impl;
import original.basic.types;
import original.basic.number;
import original.basic.iterator;
import original.basic.algorithm;

/**
 * @addtogroup Array
 * @{
 */

namespace original::details
{
    using namespace original::literals;

    /**
     * @brief Internal implementation base for Array.
     *
     * @tparam T Element type (must satisfy IsObject)
     * @tparam N Compile-time size
     *
     * Provides storage and basic unchecked access facilities.
     * Handles the special case of N == 0 by allocating a single element
     * to ensure a valid this pointer, while reporting size() as 0.
     */
    template<IsObject T, Size::Type N>
    class ArrayImpl
    {
    protected:
        using SizeType = Size;
        using ValueType = T;

        static constexpr Size::Type CAPACITY = N == 0 ? 1 : N;
        T data_[CAPACITY]{};

        constexpr ArrayImpl() = default;

        /**
         * @brief Constructs from a parameter pack of arguments.
         *
         * @tparam Args Argument types
         * @param args Arguments forwarded to element construction
         *
         * Requires sizeof...(Args) <= N and each argument to be convertible to T.
         */
        template<typename... Args>
        requires (sizeof...(Args) <= N && (IsObject<Args> && ...))
        explicit constexpr ArrayImpl(Args&&... args)
        : data_{static_cast<T>(std::forward<Args>(args))...} {}
    public:
        /**
         * @brief Returns the compile-time size N.
         */
        constexpr SizeType size() const noexcept // NOLINT
        {
            return SizeType{N};
        }

        /**
         * @brief Indicates whether the array is empty (N == 0).
         */
        constexpr bool empty() const noexcept // NOLINT
        {
            return N == 0;
        }

        /**
         * @brief Returns a pointer to the underlying storage.
         *
         * @return Pointer to the first element, or nullptr if N == 0.
         */
        [[nodiscard]] constexpr ValueType* data() noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        /**
         * @brief Const overload of data().
         */
        [[nodiscard]] constexpr const ValueType* data() const noexcept
        {
            return N != 0 ? this->data_ : nullptr;
        }

        /**
         * @brief Unchecked element access.
         *
         * @tparam U Unsigned integral-like index type
         * @param index Index to access
         * @return Reference to the element at the specified index
         */
        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& operator[](U index) noexcept
        {
            return this->data_[numberLikeValue(index)];
        }

        /**
         * @brief Const overload of operator[].
         */
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
    /**
     * @brief Fixed-size array with std::array-compatible interface.
     *
     * @tparam T Element type (must satisfy IsObject)
     * @tparam N Compile-time capacity
     *
     * Provides contiguous storage, iterators, bounds-checked access,
     * comparison operators, subarray extraction, and concatenation.
     * Specialised for N == 0 to behave as an empty container.
     */
    template<IsObject T, Size::Type N>
    class Array : public details::ArrayImpl<T, N>
    {
        using Base = details::ArrayImpl<T, N>;
    public:
        using IterType       = DefaultIterator<T, SpecifiedSource<Array>>;
        using ConstIterType  = DefaultIterator<const T, SpecifiedSource<Array>>;
        using RIterType      = ReversedIterator<IterType>;
        using ConstRIterType = ReversedIterator<ConstIterType>;
        using ValueType      = T;
        using SizeType       = Size;

        constexpr Array() = default;

        /**
         * @brief Constructs from a parameter pack of arguments.
         *
         * @tparam Args Argument types
         * @param args Arguments forwarded to element construction
         *
         * Requires sizeof...(Args) <= N.
         */
        template<typename... Args>
        requires (sizeof...(Args) <= N && (IsObject<Args> && ...))
        explicit constexpr Array(Args&&... args)
        : Base{static_cast<T>(std::forward<Args>(args))...} {}

        constexpr Array(const Array&) = default;

        constexpr Array(Array&&) noexcept = default;

        constexpr Array& operator=(const Array&) = default;

        constexpr Array& operator=(Array&&) noexcept = default;

        /**
         * @brief Returns an iterator to the beginning.
         */
        constexpr IterType begin() noexcept
        {
            return IterType{this->data_};
        }

        /**
         * @brief Returns an iterator to one past the end.
         */
        constexpr IterType end() noexcept
        {
            return IterType{this->data_ + N};
        }

        constexpr RIterType rBegin() noexcept
        {
            return RIterType{IterType{this->data_ + N - 1}};
        }

        constexpr RIterType rEnd() noexcept
        {
            return RIterType{IterType{this->data_ - 1}};
        }

        /**
         * @brief Const overloads of begin() and end().
         */
        [[nodiscard]] constexpr ConstIterType begin() const noexcept
        {
            return ConstIterType{this->data_};
        }

        [[nodiscard]] constexpr ConstIterType end() const noexcept
        {
            return ConstIterType{this->data_ + N};
        }

        constexpr ConstRIterType rBegin() const noexcept
        {
            return ConstRIterType{ConstIterType{this->data_ + N - 1}};
        }

        constexpr ConstRIterType rEnd() const noexcept
        {
            return ConstRIterType{ConstIterType{this->data_ - 1}};
        }

        /**
         * @brief Bounds-checked element access.
         *
         * @tparam U Unsigned integral-like index type
         * @param index Index to access
         * @return Reference to the element
         * @throw std::out_of_range if index >= N
         */
        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr ValueType& at(U index)
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }

        /**
         * @brief Const overload of at().
         */
        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        constexpr const ValueType& at(U index) const
        {
            if(index >= N)
                throw std::out_of_range{"Array<T, N> out of range."};

            return this->data_[numberLikeValue(index)];
        }

        /**
         * @brief Equality comparison.
         *
         * @param rhs Array to compare with
         * @return true if all elements are equal
         *
         * Requires ValueType to satisfy EqualityComparable.
         */
        bool operator==(const Array& rhs) const
        requires EqualityComparable<ValueType>
        {
            return algorithms::equal(*this, rhs);
        }

        /**
         * @brief Three-way comparison.
         *
         * @param rhs Array to compare with
         * @return Lexicographical comparison result
         *
         * Requires ValueType to satisfy ThreeWayComparable.
         */
        auto operator<=>(const Array& rhs) const
        requires ThreeWayComparable<ValueType>
        {
            return algorithms::lexicographicallyCompare(*this, rhs);
        }

        /**
         * @brief Extracts a subarray (lvalue reference overload).
         *
         * @tparam Start Starting index
         * @tparam Cnt Number of elements to extract
         * @return Array<T, Cnt> containing elements [Start, Start+Cnt)
         *
         * Requires Start + Cnt <= N.
         */
        template<Size::Type Start, Size::Type Cnt>
        requires (Start + Cnt <= N)
        constexpr Array<T, Cnt>
        subArray() const &
        {
            return forAll<Cnt>
            (
                [&]<Size::Type... I>(IndexConstant<I>...)
                {
                    return Array<T, Cnt>{
                        static_cast<T>((*this)[Start + I])...
                    };
                }
            );
        }

        /**
         * @brief Extracts a subarray (rvalue reference overload).
         *
         * Moves elements when possible.
         */
        template<Size::Type Start, Size::Type Cnt>
        requires (Start + Cnt <= N)
        constexpr Array<T, Cnt>
        subArray() &&
        {
            return forAll<Cnt>
            (
                [&]<Size::Type... I>(IndexConstant<I>...)
                {
                    return Array<T, Cnt>{
                        static_cast<T>(
                            std::move((*this)[Start + I])
                        )...
                    };
                }
            );
        }
    };

    /**
     * @brief Specialisation of Array for zero-sized arrays.
     *
     * Behaves as an empty container with begin() == end().
     */
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

        /**
         * @brief Bounds-checked access for empty array (always throws).
         */
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

        /**
         * @brief Equality for empty arrays (always true).
         */
        bool operator==(const Array&) const noexcept
        {
            return true;
        }

        /**
         * @brief Three-way comparison for empty arrays (always equal).
         */
        auto operator<=>(const Array&) const noexcept
        {
            return std::strong_ordering::equal;
        }

        /**
         * @brief Subarray extraction for empty array (only valid when Cnt == 0).
         */
        template<Size::Type Start, Size::Type Cnt>
        requires (Start + Cnt == 0)
        constexpr Array subArray() const // NOLINT
        {
            return {};
        }
    };

    /**
     * @brief Equality comparison between arrays of different sizes.
     *
     * Returns false if sizes differ.
     */
    template<IsObject T, Size::Type N1, Size::Type N2>
    requires (N1 != N2) && EqualityComparable<T>
    constexpr bool operator==(const Array<T, N1>&, const Array<T, N2>&)
    noexcept
    {
        return false;
    }

    /**
     * @brief Three-way comparison between arrays of different sizes.
     *
     * Compares sizes when N1 != N2.
     */
    template<IsObject T, Size::Type N1, Size::Type N2>
    requires (N1 != N2) && ThreeWayComparable<T>
    constexpr auto operator<=>(const Array<T, N1>&, const Array<T, N2>&)
    noexcept
    {
        return N1 <=> N2;
    }

    /**
     * @brief Structured binding support: get element by compile-time index.
     *
     * @tparam I Compile-time index
     * @tparam T Element type
     * @tparam N Array size
     * @param a Array to access
     * @return Reference to the I-th element
     *
     * Requires I < N.
     */
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

    /**
     * @brief Overloads accepting IndexConstant<I> for metaprogramming.
     */
    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr T& get(Array<T, N>& a, IndexConstant<I>) noexcept
    {
        return a[I];
    }

    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr const T& get(const Array<T, N>& a, IndexConstant<I>) noexcept
    {
        return a[I];
    }

    template<std::size_t I, IsObject T, Size::Type N>
    requires (I < N)
    constexpr T&& get(Array<T, N>&& a, IndexConstant<I>) noexcept
    {
        return std::move(a[I]);
    }

    /**
     * @brief Concatenates two arrays.
     *
     * @tparam T Element type
     * @tparam N1 First array size
     * @tparam N2 Second array size
     * @param a First array (lvalue)
     * @param b Second array (lvalue)
     * @return Array<T, N1 + N2> containing elements of a followed by b
     */
    template<IsObject T, Size::Type N1, Size::Type N2>
    constexpr Array<T, N1 + N2>
    operator+(const Array<T, N1>& a, const Array<T, N2>& b)
    {
        return forAll<N1 + N2>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Array<T, N1 + N2>{
                    (static_cast<T>(I < N1 ? a[I] : b[I - N1]))...
                };
            }
        );
    }

    /**
     * @brief Concatenation overloads supporting move semantics.
     */
    template<IsObject T, Size::Type N1, Size::Type N2>
    constexpr Array<T, N1 + N2>
    operator+(Array<T, N1>&& a, const Array<T, N2>& b)
    {
        return forAll<N1 + N2>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Array<T, N1 + N2>{
                    (static_cast<T>(I < N1 ? std::move(a[I]) : b[I - N1]))...
                };
            }
        );
    }

    template<IsObject T, Size::Type N1, Size::Type N2>
    constexpr Array<T, N1 + N2>
    operator+(const Array<T, N1>& a, Array<T, N2>&& b)
    {
        return forAll<N1 + N2>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Array<T, N1 + N2>{
                    (static_cast<T>(I < N1 ? a[I] : std::move(b[I - N1])))...
                };
            }
        );
    }

    template<IsObject T, Size::Type N1, Size::Type N2>
    constexpr Array<T, N1 + N2>
    operator+(Array<T, N1>&& a, Array<T, N2>&& b)
    {
        return forAll<N1 + N2>(
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return Array<T, N1 + N2>{
                    (static_cast<T>(I < N1 ? std::move(a[I]) : std::move(b[I - N1])))...
                };
            }
        );
    }
}

/**
 * @brief Integration with std::tuple_size and std::tuple_element.
 */
export namespace std
{
    template<typename T, std::size_t N>
    struct tuple_size<original::Array<T, N>> // NOLINT
        : std::integral_constant<std::size_t, N> {};

    template<std::size_t I, typename T, std::size_t N>
    struct tuple_element<I, original::Array<T, N>> // NOLINT
    {
        static_assert(I < N, "tuple_element index out of range");
        using type = T;
    };

    /**
     * @brief std::get overloads for original::Array.
     */
    template<std::size_t I, typename T, std::size_t N>
    constexpr T& get(original::Array<T, N>& a) noexcept // NOLINT
    {
        return original::get<I>(a);
    }

    template<std::size_t I, typename T, std::size_t N>
    constexpr const T& get(const original::Array<T, N>& a) noexcept // NOLINT
    {
        return original::get<I>(a);
    }

    template<std::size_t I, typename T, std::size_t N>
    constexpr T&& get(original::Array<T, N>&& a) noexcept // NOLINT
    {
        return original::get<I>(std::move(a));
    }
}

/** @} */ // end of Array group