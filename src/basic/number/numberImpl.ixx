module;
#include <compare>
#include <cstddef>
#include <cstdint>
export module original.basic.number.numberImpl;
import original.basic.types;

export namespace original
{
    template<StdIntegral T>
    class Integer;

    template<StdFloating T>
    class Floating;

    /** @brief Signed 8-bit integer wrapper. */
    using I8 = Integer<std::int8_t>;
    /** @brief Signed 16-bit integer wrapper. */
    using I16 = Integer<std::int16_t>;
    /** @brief Signed 32-bit integer wrapper. */
    using I32 = Integer<std::int32_t>;
    /** @brief Signed 64-bit integer wrapper. */
    using I64 = Integer<std::int64_t>;
    /** @brief Pointer difference type wrapper. */
    using Diff = Integer<std::ptrdiff_t>;
    /** @brief Unsigned 8-bit integer wrapper. */
    using U8 = Integer<std::uint8_t>;
    /** @brief Unsigned 16-bit integer wrapper. */
    using U16 = Integer<std::uint16_t>;
    /** @brief Unsigned 32-bit integer wrapper. */
    using U32 = Integer<std::uint32_t>;
    /** @brief Unsigned 64-bit integer wrapper. */
    using U64 = Integer<std::uint64_t>;
    /** @brief Alias for U8 representing a byte. */
    using Byte = U8;
    /** @brief Single-precision floating-point wrapper. */
    using F32 = Floating<float>;
    /** @brief Double-precision floating-point wrapper. */
    using F64 = Floating<double>;
    /** @brief Extended-precision floating-point wrapper. */
    using F80 = Floating<long double>;
}

namespace original::details
{
        /**
         * @brief Base class providing common functionality for strongly-typed numbers.
         * This class is intended for internal use only and stores the underlying value.
         * @tparam T Underlying arithmetic type satisfying StdArithmetic.
         */
        template<StdArithmetic T>
        class Number
        {
        protected:
            T value_{};
    template<StdArithmetic To, StdArithmetic From, From V>
    consteval void rangeCheckLiterals()
    {
        static_assert(
            V <= static_cast<From>(std::numeric_limits<To>::max()),
            "Number literal out of range"
        );
    }

    template<StdIntegral To>
    consteval To parseIntegral(const char* str, const std::size_t n)
    {
        To v = 0;
        for (std::size_t i = 0; i < n; ++i)
        {
            if (str[i] < '0' || str[i] > '9')
                throw std::invalid_argument{"invalid integer literal"};

            v = v * 10 + (str[i] - '0');
        }
        return v;
    }

    template<StdArithmetic To, char... Cs>
    consteval Integer<To> literalIntegral()
    {
        constexpr char INPUT[] {Cs...};
        constexpr auto PARSED_RESULT = parseIntegral<unsigned long long>(INPUT, sizeof...(Cs));
        rangeCheckLiterals<To, unsigned long long, PARSED_RESULT>();
        return Integer<To>{static_cast<To>(PARSED_RESULT)};
    }

    template<StdArithmetic T>
    constexpr T checkedAdd(T a, T b)
    {
        T result;
        if constexpr (USING_MSVC()) // NOLINT
        {
            if (_addcarry_u64(0, a, b, &result))
                throw std::overflow_error{"Add operation overflows"};
        }
        else if constexpr (USING_GCC() || USING_CLANG()) // NOLINT
        {
            if (__builtin_add_overflow(a, b, &result))
                throw std::overflow_error{"Add operation overflows"};
        }
        return result;
    }

    template<StdArithmetic T>
    constexpr T checkedSub(T a, T b)
    {
        if constexpr (StdSignedIntegral<T>)
        {
            if ((b > 0 && a < std::numeric_limits<T>::min() + b) ||
                (b < 0 && a > std::numeric_limits<T>::max() + b))
            {
                throw std::overflow_error{"Sub operation overflows"};
            }
        }
        else
        {
            if (a < b)
                throw std::overflow_error{"Sub operation overflows"};
        }
        return a - b;
    }

    template<StdArithmetic T>
    constexpr T checkedMul(T a, T b)
    {
        if constexpr (StdUnsignedIntegral<T>)
        {
            if (b != 0 && a > std::numeric_limits<T>::max() / b)
                throw std::overflow_error{"Mul overflow"};
        }
        else
        {
            if ((b > 0 && a > std::numeric_limits<T>::max() / b) ||
                (b < 0 && a < std::numeric_limits<T>::min() / b))
            {
                throw std::overflow_error{"Mul operation overflow"};
            }
        }

        return a * b;
    }

    template<StdArithmetic T>
    constexpr T checkedDiv(T a, T b)
    {
        if (b == 0)
            throw std::logic_error{"division by zero"};

        if constexpr (StdSignedIntegral<T>)
        {
            if (a == std::numeric_limits<T>::min() && b == T(-1))
                throw std::overflow_error{"Div operation overflow"};
        }

        return a / b;
    }

    template<StdArithmetic T>
    constexpr T checkedMod(T a, T b)
    {
        if (b == 0)
            throw std::logic_error{"modulo by zero"};

        if constexpr (StdSignedIntegral<T>)
        {
            if (b == -1 && b == std::numeric_limits<T>::min())
                throw std::overflow_error{"Mod operation overflow"};
        }

        return a % b;
    }

    template<StdSignedIntegral T>
    constexpr T checkedNeg(T a)
    {
        if (a == std::numeric_limits<T>::min())
            throw std::overflow_error{"Neg operation overflow"};

        return -a;
    }

    template<StdArithmetic T>
    constexpr T checkedShiftLeft(T a, std::size_t shift)
    {
        if (constexpr std::size_t bits = std::numeric_limits<T>::digits;
            shift > bits)
            throw std::overflow_error{"Shift operation overflows"};

        if constexpr (StdSignedIntegral<T>)
        {
            if (a < 0)
                throw std::overflow_error{"Shift operation overflows"};
        }

        if (a > std::numeric_limits<std::size_t>::max() >> shift)
            throw std::overflow_error{"Shift operation overflows"};

        return a << shift;
    }

    template<StdArithmetic T>
    constexpr T checkedShiftRight(T a, std::size_t shift)
    {
        if (constexpr std::size_t bits = std::numeric_limits<T>::digits;
            shift >= bits)
            throw std::overflow_error("right shift count out of range");

        return a >> shift;
    }


            constexpr Number() noexcept = default;

            /**
             * @brief Constructs from an underlying value.
             */
            explicit constexpr Number(T value) noexcept : value_(value) {}

            constexpr Number(const Number&) noexcept = default;
            constexpr Number& operator=(const Number&) noexcept = default;
            constexpr Number(Number&&) noexcept = default;
            constexpr Number& operator=(Number&&) noexcept = default;

        public:
            /**
             * @brief Returns the underlying value.
             */
            [[nodiscard]] constexpr T value() const noexcept { return this->value_; }
        };
}

export namespace original
{
    /**
     * @class Integer
     * @brief Strongly-typed wrapper around integral types providing type safety and consistent interface.
     * Integer behaves like the underlying integral type but prevents implicit mixing with other integral types.
     * @tparam T Underlying integral type (must satisfy StdIntegral).
     */
    template<StdIntegral T>
    class Integer final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        /**
         * @brief The underlying integral type.
         */
        using Type = T;

        constexpr Integer() noexcept = default;

        /**
         * @brief Converting constructor from a smaller integral type.
         * @tparam U Source integral type.
         * @param value The value to convert and wrap.
         */
        template<StdIntegral U>
        requires (sizeof(U) <= sizeof(T) && StdSameSignIntegral<T, U>)
        explicit constexpr Integer(U value) noexcept : Base(static_cast<T>(value)) {}

        /**
         * @brief Explicit conversion to the underlying type.
         */
        explicit constexpr operator T() const noexcept
        {
            return this->value_;
        }

        /**
         * @brief Explicit conversion to bool (non-zero check).
         */
        explicit constexpr operator bool() const noexcept
        {
            return this->value_ != 0;
        }

        /**
         * @brief Three-way comparison with another Integer.
         * @param rhs Right-hand side operand.
         * @return Strong ordering result.
         */
        constexpr std::strong_ordering operator<=>(const Integer& rhs) const noexcept
        {
            return this->value_ <=> rhs.value();
        }

        /**
         * @brief Equality comparison with another Integer.
         * @param rhs Right-hand side operand.
         * @return true if values are equal.
         */
        constexpr bool operator==(const Integer& rhs) const noexcept
        {
            return this->value_ == rhs.value();
        }

        /**
         * @brief Three-way comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return Strong ordering result.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr std::strong_ordering operator<=>(U rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        /**
         * @brief Equality comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return true if values are equal.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr bool operator==(U rhs) const noexcept
        {
            return this->value_ == rhs;
        }

        /**
         * @brief Compound addition assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator+=(Integer rhs)
        {
            this->value_ = details::checkedAdd<T>(this->value_, rhs.value());
            return *this;
        }

        /**
         * @brief Compound addition assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator+=(U rhs)
        {
            this->value_ = details::checkedAdd<T>(this->value_, rhs);
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator-=(Integer rhs)
        {
            this->value_ = details::checkedSub<T>(this->value_, rhs.value());
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator-=(U rhs)
        {
            this->value_ = details::checkedSub<T>(this->value_, rhs);
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator*=(Integer rhs)
        {
            this->value_ = details::checkedMul<T>(this->value_, rhs.value());
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator*=(U rhs)
        {
            this->value_ = details::checkedMul<T>(this->value_, rhs);
            return *this;
        }

        /**
         * @brief Compound division assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         * @note Division by zero leads to undefined behavior.
         */
        constexpr Integer& operator/=(Integer rhs)
        {
            this->value_ = details::checkedDiv<T>(this->value_, rhs.value());
            return *this;
        }

        /**
         * @brief Compound division assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         * @note Division by zero leads to undefined behavior.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator/=(U rhs)
        {
            this->value_ = details::checkedDiv<T>(this->value_, rhs);
            return *this;
        }

        /**
         * @brief Compound modulo assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         * @note Modulo by zero leads to undefined behavior.
         */
        constexpr Integer& operator%=(Integer rhs)
        {
            this->value_ = details::checkedMod<T>(this->value_, rhs.value());
            return *this;
        }

        /**
         * @brief Compound modulo assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         * @note Modulo by zero leads to undefined behavior.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator%=(U rhs)
        {
            this->value_ = details::checkedMod<T>(this->value_, rhs);
            return *this;
        }

        /**
         * @brief Prefix increment.
         * @return Reference to this Integer after increment.
         */
        constexpr Integer& operator++()
        {
            this->value_ = details::checkedAdd<T>(this->value_, 1);
            return *this;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to this Integer after decrement.
         */
        constexpr Integer& operator--()
        {
            this->value_ = details::checkedSub<T>(this->value_, 1);
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @param postfix Dummy parameter to distinguish from prefix increment.
         * @return Copy of this Integer before increment.
         */
        constexpr Integer operator++(int postfix)
        {
            Integer tmp{*this};
            this->value_ = details::checkedAdd<T>(this->value_, 1);
            return tmp;
        }

        /**
         * @brief Postfix decrement.
         * @param postfix Dummy parameter to distinguish from prefix decrement.
         * @return Copy of this Integer before decrement.
         */
        constexpr Integer operator--(int postfix)
        {
            Integer tmp{*this};
            this->value_ = details::checkedSub<T>(this->value_, 1);
            return tmp;
        }

        /**
         * @brief Unary plus operator.
         * @return Copy of this Integer.
         */
        constexpr Integer operator+() const noexcept
        {
            return *this;
        }

        /**
         * @brief Unary minus operator.
         * @return Integer with negated value.
         * @note Only available for signed integral types.
         */
        constexpr Integer operator-() const
        requires StdSignedIntegral<T>
        {
            return Integer{details::checkedNeg<T>(this->value_)};
        }

        /**
         * @brief Addition with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the sum.
         */
        constexpr Integer operator+(Integer rhs) const
        {
            Integer tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Addition with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the sum.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator+(U rhs) const
        {
            Integer tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the difference.
         */
        constexpr Integer operator-(Integer rhs) const
        {
            Integer tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the difference.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator-(U rhs) const
        {
            Integer tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the product.
         */
        constexpr Integer operator*(Integer rhs) const
        {
            Integer tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the product.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator*(U rhs) const
        {
            Integer tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Division with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the quotient.
         * @note Division by zero leads to undefined behavior.
         */
        constexpr Integer operator/(Integer rhs) const
        {
            Integer tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        /**
         * @brief Division with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the quotient.
         * @note Division by zero leads to undefined behavior.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator/(U rhs) const
        {
            Integer tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        /**
         * @brief Modulo with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the remainder.
         * @note Modulo by zero leads to undefined behavior.
         */
        constexpr Integer operator%(Integer rhs) const
        {
            Integer tmp{*this};
            tmp %= rhs;
            return tmp;
        }

        /**
         * @brief Modulo with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the remainder.
         * @note Modulo by zero leads to undefined behavior.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator%(U rhs) const
        {
            Integer tmp{*this};
            tmp %= rhs;
            return tmp;
        }

        /**
         * @brief Left shift compound assignment.
         * @param shift Number of bits to shift left.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator<<=(std::size_t shift)
        {
            this->value_ = details::checkedShiftLeft(this->value_, shift);
            return *this;
        }

        /**
         * @brief Right shift compound assignment.
         * @param shift Number of bits to shift right.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator>>=(std::size_t shift)
        {
            this->value_ = details::checkedShiftRight(this->value_, shift);
            return *this;
        }

        /**
         * @brief Left shift operator.
         * @param shift Number of bits to shift left.
         * @return New Integer containing the shifted value.
         */
        constexpr Integer operator<<(std::size_t shift) const
        {
            Integer tmp{*this};
            tmp <<= shift;
            return tmp;
        }

        /**
         * @brief Right shift operator.
         * @param shift Number of bits to shift right.
         * @return New Integer containing the shifted value.
         */
        constexpr Integer operator>>(std::size_t shift) const
        {
            Integer tmp{*this};
            tmp >>= shift;
            return tmp;
        }

        /**
         * @brief Bitwise AND compound assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator&=(Integer rhs) noexcept
        {
            this->value_ &= rhs.value();
            return *this;
        }

        /**
         * @brief Bitwise AND compound assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator&=(U rhs) noexcept
        {
            this->value_ &= rhs;
            return *this;
        }

        /**
         * @brief Bitwise OR compound assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator|=(Integer rhs) noexcept
        {
            this->value_ |= rhs.value();
            return *this;
        }

        /**
         * @brief Bitwise OR compound assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator|=(U rhs) noexcept
        {
            this->value_ |= rhs;
            return *this;
        }

        /**
         * @brief Bitwise XOR compound assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator^=(Integer rhs) noexcept
        {
            this->value_ ^= rhs.value();
            return *this;
        }

        /**
         * @brief Bitwise XOR compound assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer& operator^=(U rhs) noexcept
        {
            this->value_ ^= rhs;
            return *this;
        }

        /**
         * @brief Bitwise AND with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the result.
         */
        constexpr Integer operator&(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp &= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise AND with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the result.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator&(U rhs) const noexcept
        {
            Integer tmp{*this};
            tmp &= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise OR with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the result.
         */
        constexpr Integer operator|(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp |= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise OR with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the result.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator|(U rhs) const noexcept
        {
            Integer tmp{*this};
            tmp |= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise XOR with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the result.
         */
        constexpr Integer operator^(Integer rhs) const noexcept
        {
            Integer tmp{*this};
            tmp ^= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise XOR with underlying type.
         * @param rhs Value of underlying type.
         * @return New Integer containing the result.
         */
        template<StdIntegral U>
        requires std::same_as<U, T>
        constexpr Integer operator^(U rhs) const noexcept
        {
            Integer tmp{*this};
            tmp ^= rhs;
            return tmp;
        }

        /**
         * @brief Bitwise NOT operator.
         * @return New Integer containing the bitwise complement.
         */
        constexpr Integer operator~() const noexcept
        {
            return Integer{static_cast<T>(~this->value_)};
        }
    };

    /**
     * @class Floating
     * @brief Strongly-typed wrapper around floating-point types providing type safety and consistent interface.
     * Floating behaves like the underlying floating-point type but prevents implicit mixing with other types.
     * @tparam T Underlying floating-point type (must satisfy StdFloating).
     */
    template<StdFloating T>
    class Floating final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        /**
         * @brief The underlying floating-point type.
         */
        using Type = T;

        constexpr Floating() noexcept = default;

        /**
         * @brief Converting constructor from a smaller floating-point type.
         * @tparam U Source floating-point type.
         * @param value The value to convert and wrap.
         */
        template<StdFloating U>
        requires (sizeof(U) <= sizeof(T))
        explicit constexpr Floating(U value) noexcept : Base(static_cast<T>(value)) {}

        /**
         * @brief Explicit conversion to the underlying type.
         */
        explicit constexpr operator T() const noexcept
        {
            return this->value_;
        }

        /**
         * @brief Explicit conversion to bool (non-zero check).
         */
        explicit constexpr operator bool() const noexcept
        {
            return this->value_ != 0;
        }

        /**
         * @brief Three-way comparison with another Floating.
         * Uses std::partial_ordering due to possible NaN values.
         * @param rhs Right-hand side operand.
         * @return Partial ordering result.
         */
        constexpr std::partial_ordering operator<=>(const Floating& rhs) const noexcept
        {
            return this->value_ <=> rhs.value();
        }

        /**
         * @brief Equality comparison with another Floating.
         * @param rhs Right-hand side operand.
         * @return true if values are equal.
         */
        constexpr bool operator==(const Floating& rhs) const noexcept
        {
            return this->value_ == rhs.value();
        }

        /**
         * @brief Three-way comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return Partial ordering result.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr std::partial_ordering operator<=>(U rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        /**
         * @brief Equality comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return true if values are equal.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr bool operator==(U rhs) const noexcept
        {
            return this->value_ == rhs;
        }

        /**
         * @brief Compound addition assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator+=(Floating rhs) noexcept
        {
            this->value_ += rhs.value();
            return *this;
        }

        /**
         * @brief Compound addition assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator+=(U rhs) noexcept
        {
            this->value_ += rhs;
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator-=(Floating rhs) noexcept
        {
            this->value_ -= rhs.value();
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator-=(U rhs) noexcept
        {
            this->value_ -= rhs;
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator*=(Floating rhs) noexcept
        {
            this->value_ *= rhs.value();
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator*=(U rhs) noexcept
        {
            this->value_ *= rhs;
            return *this;
        }

        /**
         * @brief Compound division assignment with another Floating.
         * @param rhs Right-hand side operand.
         * @return Reference to this Floating.
         */
        constexpr Floating& operator/=(Floating rhs)
        {
            this->value_ /= rhs.value();
            return *this;
        }

        /**
         * @brief Compound division assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Floating.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating& operator/=(U rhs)
        {
            this->value_ /= rhs;
            return *this;
        }

        /**
         * @brief Prefix increment.
         * @return Reference to this Floating after increment.
         */
        constexpr Floating& operator++() noexcept
        {
            ++this->value_;
            return *this;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to this Floating after decrement.
         */
        constexpr Floating& operator--() noexcept
        {
            --this->value_;
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @param postfix Dummy parameter to distinguish from prefix increment.
         * @return Copy of this Floating before increment.
         */
        constexpr Floating operator++(int postfix) noexcept
        {
            Floating tmp{*this};
            ++this->value_;
            return tmp;
        }

        /**
         * @brief Postfix decrement.
         * @param postfix Dummy parameter to distinguish from prefix decrement.
         * @return Copy of this Floating before decrement.
         */
        constexpr Floating operator--(int postfix) noexcept
        {
            Floating tmp{*this};
            --this->value_;
            return tmp;
        }

        /**
         * @brief Unary plus operator.
         * @return Copy of this Floating.
         */
        constexpr Floating operator+() const noexcept
        {
            return *this;
        }

        /**
         * @brief Unary minus operator.
         * @return Floating with negated value.
         */
        constexpr Floating operator-() const noexcept
        {
            return Floating{-this->value_};
        }

        /**
         * @brief Addition with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the sum.
         */
        constexpr Floating operator+(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Addition with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the sum.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator+(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the difference.
         */
        constexpr Floating operator-(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Subtraction with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the difference.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator-(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp -= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the product.
         */
        constexpr Floating operator*(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Multiplication with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the product.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator*(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief Division with another Floating.
         * @param rhs Right-hand side operand.
         * @return New Floating containing the quotient.
         */
        constexpr Floating operator/(Floating rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }

        /**
         * @brief Division with underlying type.
         * @param rhs Value of underlying type.
         * @return New Floating containing the quotient.
         */
        template<StdFloating U>
        requires std::same_as<U, T>
        constexpr Floating operator/(U rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }
    };

    /**
     * @brief Namespace containing user-defined literal operators
     * for convenient construction of strongly-typed numbers.
     */
    namespace literals
    {
        /**
         * @brief User-defined literal for creating I8 values.
         * @param v Unsigned long long literal value.
         * @return I8 instance with the given value.
         * @note The value will be truncated to 8 bits.
         */
        constexpr I8 operator""_i8(const unsigned long long v) noexcept
        {
            return I8{static_cast<std::int8_t>(v)};
        }

        /**
         * @brief User-defined literal for creating I16 values.
         * @param v Unsigned long long literal value.
         * @return I16 instance with the given value.
         * @note The value will be truncated to 16 bits.
         */
        constexpr I16 operator""_i16(const unsigned long long v) noexcept
        {
            return I16{static_cast<std::int16_t>(v)};
        }

        /**
         * @brief User-defined literal for creating I32 values.
         * @param v Unsigned long long literal value.
         * @return I32 instance with the given value.
         */
        constexpr I32 operator""_i32(const unsigned long long v) noexcept
        {
            return I32{static_cast<std::int32_t>(v)};
        }

        /**
         * @brief User-defined literal for creating I64 values.
         * @param v Unsigned long long literal value.
         * @return I64 instance with the given value.
         */
        constexpr I64 operator""_i64(const unsigned long long v) noexcept
        {
            return I64{static_cast<std::int64_t>(v)};
        }

        /**
         * @brief User-defined literal for creating Diff values.
         * @param v Unsigned long long literal value.
         * @return Diff instance with the given value.
         * @note Diff is a platform-dependent signed integer type for pointer differences.
         */
        constexpr Diff operator""_diff(const unsigned long long v) noexcept
        {
            return Diff{static_cast<std::ptrdiff_t>(v)};
        }

        /**
         * @brief User-defined literal for creating Byte values.
         * @param v Unsigned long long literal value.
         * @return Byte instance with the given value.
         * @note Byte is an alias for U8, representing a byte value.
         * @note The value will be truncated to 8 bits.
         */
        constexpr Byte operator""_byte(const unsigned long long v) noexcept
        {
            return Byte{static_cast<std::uint8_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U8 values.
         * @param v Unsigned long long literal value.
         * @return U8 instance with the given value.
         * @note The value will be truncated to 8 bits.
         */
        constexpr U8 operator""_u8(const unsigned long long v) noexcept
        {
            return U8{static_cast<std::uint8_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U16 values.
         * @param v Unsigned long long literal value.
         * @return U16 instance with the given value.
         * @note The value will be truncated to 16 bits.
         */
        constexpr U16 operator""_u16(const unsigned long long v) noexcept
        {
            return U16{static_cast<std::uint16_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U32 values.
         * @param v Unsigned long long literal value.
         * @return U32 instance with the given value.
         */
        constexpr U32 operator""_u32(const unsigned long long v) noexcept
        {
            return U32{static_cast<std::uint32_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U64 values.
         * @param v Unsigned long long literal value.
         * @return U64 instance with the given value.
         */
        constexpr U64 operator""_u64(const unsigned long long v) noexcept
        {
            return U64{v};
        }

        /**
         * @brief User-defined literal for creating F32 values.
         * @param v Long double literal value.
         * @return F32 instance with the given value.
         */
        constexpr F32 operator""_f32(const long double v) noexcept
        {
            return F32{static_cast<float>(v)};
        }

        /**
         * @brief User-defined literal for creating F64 values.
         * @param v Long double literal value.
         * @return F64 instance with the given value.
         */
        constexpr F64 operator""_f64(const long double v) noexcept
        {
            return F64{static_cast<double>(v)};
        }

        /**
         * @brief User-defined literal for creating F80 values.
         * @param v Long double literal value.
         * @return F80 instance with the given value.
         */
        constexpr F80 operator""_f80(const long double v) noexcept
        {
            return F80{v};
        }
    }
}