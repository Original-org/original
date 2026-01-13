module;
#include <cstdint>
#include <type_traits>
#include <compare>
#include <cstddef> // NOLINT
export module original.basic.number.impl:integer;
import :details;


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

        /** @brief Default constructor. */
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
        template<StdUnsignedIntegral U>
        constexpr Integer& operator<<=(const U shift)
        {
            this->value_ = details::checkedShiftLeft(this->value_, static_cast<std::size_t>(shift));
            return *this;
        }

        /**
         * @brief Right shift compound assignment.
         * @param shift Number of bits to shift right.
         * @return Reference to this Integer.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer& operator>>=(const U shift)
        {
            this->value_ = details::checkedShiftRight(this->value_, static_cast<std::size_t>(shift));
            return *this;
        }

        /**
         * @brief Left shift compound assignment with Integer shift operand.
         * @param shift Number of bits to shift left.
         * @return Reference to this Integer.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer& operator<<=(const Integer<U> shift)
        {
            return *this <<= shift.value();
        }

        /**
         * @brief Right shift compound assignment with Integer shift operand.
         * @param shift Number of bits to shift right.
         * @return Reference to this Integer.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer& operator>>=(const Integer<U> shift)
        {
            return *this >>= shift.value();
        }

        /**
         * @brief Left shift operator.
         * @param shift Number of bits to shift left.
         * @return New Integer containing the shifted value.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer operator<<(const U shift) const
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
        template<StdUnsignedIntegral U>
        constexpr Integer operator>>(const U shift) const
        {
            Integer tmp{*this};
            tmp >>= shift;
            return tmp;
        }

        /**
         * @brief Left shift operator with Integer shift operand.
         * @param shift Number of bits to shift left.
         * @return New Integer containing the shifted value.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer operator<<(const Integer<U> shift) const
        {
            return *this << shift.value();
        }

        /**
         * @brief Right shift operator with Integer shift operand.
         * @param shift Number of bits to shift right.
         * @return New Integer containing the shifted value.
         */
        template<StdUnsignedIntegral U>
        constexpr Integer operator>>(const Integer<U> shift) const
        {
            return *this >> shift.value();
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
     * @brief Addition operator for mixed Integer types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Integer of appropriate size containing the sum.
     * @note Both types must have same signedness.
     */
    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr auto operator+(Integer<U> lhs, Integer<V> rhs)
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = details::checkedAdd<To>(static_cast<To>(lhs.value()), static_cast<To>(rhs.value()));
        return Integer<To>(result);
    }

    /**
     * @brief Subtraction operator for mixed Integer types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Integer of appropriate size containing the difference.
     * @note Both types must have same signedness.
     */
    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr auto operator-(Integer<U> lhs, Integer<V> rhs)
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = details::checkedSub<To>(static_cast<To>(lhs.value()), static_cast<To>(rhs.value()));
        return Integer<To>(result);
    }

    /**
     * @brief Multiplication operator for mixed Integer types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Integer of appropriate size containing the product.
     * @note Both types must have same signedness.
     */
    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr auto operator*(Integer<U> lhs, Integer<V> rhs)
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = details::checkedMul<To>(static_cast<To>(lhs.value()), static_cast<To>(rhs.value()));
        return Integer<To>(result);
    }

    /**
     * @brief Division operator for mixed Integer types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Integer of appropriate size containing the quotient.
     * @note Both types must have same signedness.
     */
    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr auto operator/(Integer<U> lhs, Integer<V> rhs)
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = details::checkedDiv<To>(static_cast<To>(lhs.value()), static_cast<To>(rhs.value()));
        return Integer<To>(result);
    }

    /**
     * @brief Modulo operator for mixed Integer types.
     * @tparam U Left operand type.
     * @tparam V Right operand type.
     * @param lhs Left operand.
     * @param rhs Right operand.
     * @return Integer of appropriate size containing the remainder.
     * @note Both types must have same signedness.
     */
    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr auto operator%(Integer<U> lhs, Integer<V> rhs)
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        To result = details::checkedMod<To>(static_cast<To>(lhs.value()), static_cast<To>(rhs.value()));
        return Integer<To>(result);
    }

    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr bool operator==(Integer<U> lhs, Integer<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        return static_cast<To>(lhs.value()) == static_cast<To>(rhs.value());
    }

    template<StdIntegral U, StdIntegral V>
    requires StdSameSignIntegral<U, V> && (!SameType<U, V>)
    constexpr std::strong_ordering operator<=>(Integer<U> lhs, Integer<V> rhs) noexcept
    {
        using To = std::conditional_t<sizeof(U) <= sizeof(V), V, U>;
        return static_cast<To>(lhs.value()) <=> static_cast<To>(rhs.value());
    }

    namespace literals
    {
        /**
         * @brief User-defined literal for creating I8 (8-bit signed integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return I8 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval I8 operator""_i8()
        {
            return details::literalIntegral<std::int8_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating I16 (16-bit signed integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return I16 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval I16 operator""_i16()
        {
            return details::literalIntegral<std::int16_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating I32 (32-bit signed integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return I32 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval I32 operator""_i32()
        {
            return details::literalIntegral<std::int32_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating I64 (64-bit signed integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return I64 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval I64 operator""_i64()
        {
            return details::literalIntegral<std::int64_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating Diff (pointer difference type) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return Diff instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval Diff operator""_diff()
        {
            return details::literalIntegral<std::ptrdiff_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating Byte (8-bit unsigned integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return Byte instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval Byte operator""_byte()
        {
            return details::literalIntegral<std::uint8_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating Size (size type) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return Size instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval Size operator""_size()
        {
            return details::literalIntegral<std::size_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating U8 (8-bit unsigned integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return U8 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval U8 operator""_u8()
        {
            return details::literalIntegral<std::uint8_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating U16 (16-bit unsigned integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return U16 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval U16 operator""_u16()
        {
            return details::literalIntegral<std::uint16_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating U32 (32-bit unsigned integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return U32 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval U32 operator""_u32()
        {
            return details::literalIntegral<std::uint32_t, Cs...>();
        }

        /**
         * @brief User-defined literal for creating U64 (64-bit unsigned integer) values.
         * @tparam Cs Character sequence representing the integer literal.
         * @return U64 instance with the parsed value.
         * @note This is a consteval function that performs compile-time validation.
         *       Invalid literals or values out of range result in compilation errors.
         */
        template<char... Cs>
        consteval U64 operator""_u64()
        {
            return details::literalIntegral<std::uint64_t, Cs...>();
        }
    }
}