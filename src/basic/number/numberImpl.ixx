module;
#include <compare>
#include <cstdint>
export module original.basic.number.numberImpl;
import original.basic.types;

namespace original
{
    namespace details
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

    /**
     * @class Integer
     * @brief Strongly-typed wrapper around integral types providing type safety and consistent interface.
     * Integer behaves like the underlying integral type but prevents implicit mixing with other integral types.
     * @tparam T Underlying integral type (must satisfy StdIntegral).
     */
    export template<StdIntegral T>
    class Integer final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        /**
         * @brief The underlying integral type.
         */
        using Type = T;

        constexpr Integer() noexcept = default;

        explicit constexpr Integer(T value) noexcept : Base(value) {}

        /**
         * @brief Converting constructor from a smaller integral type.
         * @tparam U Source integral type.
         * @param value The value to convert and wrap.
         */
        template<StdIntegral U>
        requires (sizeof(U) <= sizeof(T))
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
        constexpr std::strong_ordering operator<=>(T rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        /**
         * @brief Equality comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return true if values are equal.
         */
        constexpr bool operator==(T rhs) const noexcept
        {
            return this->value_ == rhs;
        }

        /**
         * @brief Compound addition assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator+=(Integer rhs) noexcept
        {
            this->value_ += rhs.value();
            return *this;
        }

        /**
         * @brief Compound addition assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator+=(T rhs) noexcept
        {
            return *this += Integer{rhs};
        }

        /**
         * @brief Compound subtraction assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator-=(Integer rhs) noexcept
        {
            this->value_ -= rhs.value();
            return *this;
        }

        /**
         * @brief Compound subtraction assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator-=(T rhs) noexcept
        {
            return *this -= Integer{rhs};
        }

        /**
         * @brief Compound multiplication assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator*=(Integer rhs) noexcept
        {
            this->value_ *= rhs.value();
            return *this;
        }

        /**
         * @brief Compound multiplication assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator*=(T rhs) noexcept
        {
            return *this *= Integer{rhs};
        }

        /**
         * @brief Compound division assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         * @note Division by zero leads to undefined behavior.
         */
        constexpr Integer& operator/=(Integer rhs)
        {
            this->value_ /= rhs.value();
            return *this;
        }

        /**
         * @brief Compound division assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         * @note Division by zero leads to undefined behavior.
         */
        constexpr Integer& operator/=(T rhs)
        {
            return *this /= Integer{rhs};
        }

        /**
         * @brief Compound modulo assignment with another Integer.
         * @param rhs Right-hand side operand.
         * @return Reference to this Integer.
         * @note Modulo by zero leads to undefined behavior.
         */
        constexpr Integer& operator%=(Integer rhs)
        {
            this->value_ %= rhs.value();
            return *this;
        }

        /**
         * @brief Compound modulo assignment with underlying type.
         * @param rhs Value of underlying type.
         * @return Reference to this Integer.
         * @note Modulo by zero leads to undefined behavior.
         */
        constexpr Integer& operator%=(T rhs)
        {
            return *this %= Integer{rhs};
        }

        /**
         * @brief Prefix increment.
         * @return Reference to this Integer after increment.
         */
        constexpr Integer& operator++() noexcept
        {
            ++this->value_;
            return *this;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to this Integer after decrement.
         */
        constexpr Integer& operator--() noexcept
        {
            --this->value_;
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @param postfix Dummy parameter to distinguish from prefix increment.
         * @return Copy of this Integer before increment.
         */
        constexpr Integer operator++(int postfix) noexcept
        {
            Integer tmp{*this};
            this->value_ += 1;
            return tmp;
        }

        /**
         * @brief Postfix decrement.
         * @param postfix Dummy parameter to distinguish from prefix decrement.
         * @return Copy of this Integer before decrement.
         */
        constexpr Integer operator--(int postfix) noexcept
        {
            Integer tmp{*this};
            this->value_ -= 1;
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
        constexpr Integer operator-() const noexcept
        requires StdSignedIntegral<T>
        {
            return Integer{static_cast<T>(-this->value_)};
        }

        /**
         * @brief Addition with another Integer.
         * @param rhs Right-hand side operand.
         * @return New Integer containing the sum.
         */
        constexpr Integer operator+(Integer rhs) const noexcept
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
        constexpr Integer operator+(T rhs) const noexcept
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
        constexpr Integer operator-(Integer rhs) const noexcept
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
        constexpr Integer operator-(T rhs) const noexcept
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
        constexpr Integer operator*(Integer rhs) const noexcept
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
        constexpr Integer operator*(T rhs) const noexcept
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
        constexpr Integer operator/(T rhs) const
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
        constexpr Integer operator%(T rhs) const
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
        constexpr Integer& operator<<=(std::size_t shift) noexcept
        {
            this->value_ <<= shift;
            return *this;
        }

        /**
         * @brief Right shift compound assignment.
         * @param shift Number of bits to shift right.
         * @return Reference to this Integer.
         */
        constexpr Integer& operator>>=(std::size_t shift) noexcept
        {
            this->value_ >>= shift;
            return *this;
        }

        /**
         * @brief Left shift operator.
         * @param shift Number of bits to shift left.
         * @return New Integer containing the shifted value.
         */
        constexpr Integer operator<<(std::size_t shift) const noexcept
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
        constexpr Integer operator>>(std::size_t shift) const noexcept
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
        constexpr Integer& operator&=(T rhs) noexcept
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
        constexpr Integer& operator|=(T rhs) noexcept
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
        constexpr Integer& operator^=(T rhs) noexcept
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
        constexpr Integer operator&(T rhs) const noexcept
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
        constexpr Integer operator|(T rhs) const noexcept
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
        constexpr Integer operator^(T rhs) const noexcept
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
    export template<StdFloating T>
    class Floating final : public details::Number<T> {
        using Base = details::Number<T>;
    public:
        /**
         * @brief The underlying floating-point type.
         */
        using Type = T;

        constexpr Floating() noexcept = default;

        /**
         * @brief Constructs a Floating from a value of the underlying type.
         * @param value The value to wrap.
         */
        explicit constexpr Floating(T value) noexcept : Base(value) {}

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
        constexpr std::partial_ordering operator<=>(T rhs) const noexcept
        {
            return this->value_ <=> rhs;
        }

        /**
         * @brief Equality comparison with underlying type.
         * @param rhs Value of underlying type.
         * @return true if values are equal.
         */
        constexpr bool operator==(T rhs) const noexcept
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
        constexpr Floating& operator+=(T rhs) noexcept
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
        constexpr Floating& operator-=(T rhs) noexcept
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
        constexpr Floating& operator*=(T rhs) noexcept
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
        constexpr Floating& operator/=(T rhs)
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
        constexpr Floating operator+(T rhs) const noexcept
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
        constexpr Floating operator-(T rhs) const noexcept
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
        constexpr Floating operator*(T rhs) const noexcept
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
        constexpr Floating operator/(T rhs) const noexcept
        {
            Floating tmp{*this};
            tmp /= rhs;
            return tmp;
        }
    };

    /** @brief Signed 8-bit integer wrapper. */
    export using I8 = Integer<std::int8_t>;
    /** @brief Signed 16-bit integer wrapper. */
    export using I16 = Integer<std::int16_t>;
    /** @brief Signed 32-bit integer wrapper. */
    export using I32 = Integer<std::int32_t>;
    /** @brief Signed 64-bit integer wrapper. */
    export using I64 = Integer<std::int64_t>;
    /** @brief Unsigned 8-bit integer wrapper. */
    export using U8 = Integer<std::uint8_t>;
    /** @brief Unsigned 16-bit integer wrapper. */
    export using U16 = Integer<std::uint16_t>;
    /** @brief Unsigned 32-bit integer wrapper. */
    export using U32 = Integer<std::uint32_t>;
    /** @brief Unsigned 64-bit integer wrapper. */
    export using U64 = Integer<std::uint64_t>;
    /** @brief Alias for U8 representing a byte. */
    export using Byte = U8;
    /** @brief Single-precision floating-point wrapper. */
    export using F32 = Floating<float>;
    /** @brief Double-precision floating-point wrapper. */
    export using F64 = Floating<double>;
    /** @brief Extended-precision floating-point wrapper. */
    export using F80 = Floating<long double>;

    /**
     * @brief Namespace containing user-defined literal operators
     * for convenient construction.
     */
    export namespace literals
    {
        /**
         * @brief User-defined literal for creating I32 values.
         * @param v Unsigned long long literal value.
         * @return I32 instance with the given value.
         */
        constexpr I32 operator""_i32(const unsigned long long v)
        {
            return I32{static_cast<std::int32_t>(v)};
        }

        /**
         * @brief User-defined literal for creating I64 values.
         * @param v Unsigned long long literal value.
         * @return I64 instance with the given value.
         */
        constexpr I64 operator""_i64(const unsigned long long v)
        {
            return I64{static_cast<std::int64_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U32 values.
         * @param v Unsigned long long literal value.
         * @return U32 instance with the given value.
         */
        constexpr U32 operator""_u32(const unsigned long long v)
        {
            return U32{static_cast<std::uint32_t>(v)};
        }

        /**
         * @brief User-defined literal for creating U64 values.
         * @param v Unsigned long long literal value.
         * @return U64 instance with the given value.
         */
        constexpr U64 operator""_u64(const unsigned long long v)
        {
            return U64{v};
        }

        /**
         * @brief User-defined literal for creating F32 values.
         * @param v Long double literal value.
         * @return F32 instance with the given value.
         */
        constexpr F32 operator""_f32(const long double v)
        {
            return F32{static_cast<float>(v)};
        }

        /**
         * @brief User-defined literal for creating F64 values.
         * @param v Long double literal value.
         * @return F64 instance with the given value.
         */
        constexpr F64 operator""_f64(const long double v)
        {
            return F64{static_cast<double>(v)};
        }

        /**
         * @brief User-defined literal for creating F80 values.
         *
         * @param v Long double literal value.
         * @return F80 instance with the given value.
         */
        constexpr F80 operator""_f80(const long double v)
        {
            return F80{v};
        }
    }

    /**
     * @brief Performs a narrowing cast between signed integral types.
     * @details Converts a larger signed Integer to a smaller signed Integer.
     *          The cast is explicit and may truncate; use with caution.
     * @tparam To   Target signed integral type (must be smaller than From).
     * @tparam From Source signed integral type.
     * @param i     Signed Integer to cast.
     * @return Integer<To> with the narrowed value.
     */
    export template<StdSignedIntegral To, StdSignedIntegral From>
    requires (sizeof(To) < sizeof(From))
    [[nodiscard]]
    constexpr Integer<To> narrowCast(const Integer<From>& i) noexcept
    {
        return Integer<To>{static_cast<To>(i.value())};
    }

    /**
     * @brief Performs a narrowing cast between unsigned integral types.
     * @details Converts a larger unsigned Integer to a smaller unsigned Integer.
     *          The cast is explicit and may truncate; use with caution.
     * @tparam To   Target unsigned integral type (must be smaller than From).
     * @tparam From Source unsigned integral type.
     * @param i     Unsigned Integer to cast.
     * @return Integer<To> with the narrowed value.
     */
    export template<StdUnsignedIntegral To, StdUnsignedIntegral From>
    requires (sizeof(To) < sizeof(From))
    [[nodiscard]]
    constexpr Integer<To> narrowCast(const Integer<From>& i) noexcept
    {
        return Integer<To>{static_cast<To>(i.value())};
    }

    export template<StdFloating To, StdFloating From>
    requires (sizeof(To) < sizeof(From))
    [[nodiscard]]
    constexpr Floating<To> narrowCast(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Casts between signed and unsigned integral types of the same size.
     * @details Performs a bitwise reinterpretation of the value without changing its representation.
     *          The source and destination types must have the same size.
     * @tparam To   Target unsigned integral type.
     * @tparam From Source signed integral type.
     * @param v     Signed Integer to reinterpret.
     * @return Integer<To> with the reinterpreted value.
     */
    export template<StdUnsignedIntegral To, StdSignedIntegral From>
    requires (sizeof(To) == sizeof(From))
    [[nodiscard]]
    constexpr Integer<To> signCast(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Casts between unsigned and signed integral types of the same size.
     * @details Performs a bitwise reinterpretation of the value without changing its representation.
     *          The source and destination types must have the same size.
     * @tparam To   Target signed integral type.
     * @tparam From Source unsigned integral type.
     * @param v     Unsigned Integer to reinterpret.
     * @return Integer<To> with the reinterpreted value.
     */
    export template<StdSignedIntegral To, StdUnsignedIntegral From>
    requires (sizeof(To) == sizeof(From))
    [[nodiscard]]
    constexpr Integer<To> signCast(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Performs a general numeric cast between integral types.
     * @details Converts an Integer of one type to another integral type.
     *          Supports both signed and unsigned conversions regardless of size.
     *          Use with caution as this may involve sign changes or truncation.
     * @tparam To   Target integral type.
     * @tparam From Source integral type.
     * @param v     Integer to cast.
     * @return Integer<To> with the converted value.
     */
    export template<StdIntegral To, StdIntegral From>
    [[nodiscard]]
    constexpr Integer<To> numericCast(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    export template<StdFloating To, StdFloating From>
    [[nodiscard]]
    constexpr Floating<To> numericCast(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    export template<StdFloating To, StdIntegral From>
    [[nodiscard]]
    constexpr Floating<To> numericCast(const Integer<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    export template<StdIntegral To, StdFloating From>
    [[nodiscard]]
    constexpr Integer<To> numericCast(const Floating<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Converts a std::size_t to U64 safely.
     * @param n The std::size_t value.
     * @return Corresponding U64 instance.
     */
    export [[nodiscard]]
    constexpr U64 fromStdSize(const std::size_t n) noexcept
    {
        return U64{static_cast<std::uint64_t>(n)};
    }

    /**
     * @brief Converts a U64 back to std::size_t.
     * @details The conversion assumes the value fits in std::size_t.
     * @param u The U64 instance.
     * @return The underlying value as std::size_t.
     */
    export constexpr std::size_t toStdSize(const U64& u)
    {
        return u.value();
    }
}