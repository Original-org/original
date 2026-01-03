module;
export module original.basic.number.casts;
import original.basic.number.traits;
import original.basic.number.impl;
import original.basic.types;


namespace original::details
{
    /**
     * @brief Implementation of narrowing cast between integrals with same sign.
     *
     * @tparam To Destination integral type (must be smaller than From)
     * @tparam From Source integral type
     * @param v Source value to cast
     * @return Integer<To> Narrowed value
     *
     * @pre sizeof(To) < sizeof(From)
     * @pre To and From arguments must have same signedness
     * @note This is a low-level implementation function
     */
    template<StdIntegral To, StdIntegral From>
    requires (sizeof(To) < sizeof(From) && StdSameSignIntegral<To, From>)
    constexpr Integer<To> narrowCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of narrowing cast between floating-point types.
     *
     * @tparam To Destination floating-point type (must be smaller than From)
     * @tparam From Source floating-point type
     * @param v Source value to cast
     * @return Floating<To> Narrowed value
     *
     * @pre sizeof(To) < sizeof(From)
     * @note This is a low-level implementation function
     */
    template<StdFloating To, StdFloating From>
    requires (sizeof(To) < sizeof(From))
    constexpr Floating<To> narrowCastImpl(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of sign cast between same-size integrals with different signs.
     *
     * @tparam To Destination integral type
     * @tparam From Source integral type
     * @param v Source value to cast
     * @return Integer<To> Value with changed sign representation
     *
     * @pre sizeof(To) == sizeof(From)
     * @pre To and From arguments must have different signedness
     * @note This is a low-level implementation function
     */
    template<StdIntegral To, StdIntegral From>
    requires (sizeof(To) == sizeof(From) && !StdSameSignIntegral<To, From>)
    constexpr Integer<To> signCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of numeric cast between integral types.
     *
     * @tparam To Destination integral type
     * @tparam From Source integral type
     * @param v Source value to cast
     * @return Integer<To> Converted value
     *
     * @note This is a low-level implementation function
     * @note No size or sign restrictions apply
     */
    template<StdIntegral To, StdIntegral From>
    constexpr Integer<To> numericCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of numeric cast between floating-point types.
     *
     * @tparam To Destination floating-point type
     * @tparam From Source floating-point type
     * @param v Source value to cast
     * @return Floating<To> Converted value
     *
     * @note This is a low-level implementation function
     * @note No size restrictions apply
     */
    template<StdFloating To, StdFloating From>
    constexpr Floating<To> numericCastImpl(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of numeric cast from integral to floating-point.
     *
     * @tparam To Destination floating-point type
     * @tparam From Source integral type
     * @param v Source value to cast
     * @return Floating<To> Converted floating-point value
     *
     * @note This is a low-level implementation function
     */
    template<StdFloating To, StdIntegral From>
    constexpr Floating<To> numericCastImpl(const Integer<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    /**
     * @brief Implementation of numeric cast from floating-point to integral.
     *
     * @tparam To Destination integral type
     * @tparam From Source floating-point type
     * @param v Source value to cast
     * @return Integer<To> Converted integral value
     *
     * @note This is a low-level implementation function
     * @warning Fractional part is truncated
     */
    template<StdIntegral To, StdFloating From>
    constexpr Integer<To> numericCastImpl(const Floating<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }
}

export namespace original
{
    /**
     * @brief Narrowing cast between numeric types with compile-time size check.
     *
     * @tparam To Destination number type (must be smaller than From)
     * @tparam From Source number type
     * @param v Source value to cast
     * @return To Narrowed value
     *
     * @pre sizeof(NumberType<To>) < sizeof(NumberType<From>)
     * @pre For integrals, both types must have same signedness
     *
     * @example
     * @code
     * I64 large = 1000_i64;
     * I32 small = narrowCast<I32>(large);  // OK: narrowing from 64 to 32 bits
     * @endcode
     */
    template<Number To, Number From>
    [[nodiscard]]
    constexpr To narrowCast(const From& v) noexcept
    {
        return details::narrowCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    /**
     * @brief Cast between same-size integral types with different signs.
     *
     * @tparam To Destination integral type
     * @tparam From Source integral type
     * @param v Source value to cast
     * @return To Value with changed sign representation
     *
     * @pre sizeof(NumberType<To>) == sizeof(NumberType<From>)
     * @pre Types must have different signedness
     *
     * @example
     * @code
     * I32 signed_val = -42_i32;
     * U32 unsigned_val = signCast<U32>(signed_val);  // Reinterpret bits
     * @endcode
     */
    template<Number To, Number From>
    [[nodiscard]]
    constexpr To signCast(const From& v) noexcept
    {
        return details::signCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    /**
     * @brief Generic numeric cast between any numeric types.
     *
     * @tparam To Destination number type
     * @tparam From Source number type
     * @param v Source value to cast
     * @return To Converted value
     *
     * @note Supports all numeric type conversions
     * @warning May truncate (float→int) or overflow (int→smaller int)
     *
     * @example
     * @code
     * F64 d = 3.14_f64;
     * I32 i = numericCast<I32>(d);  // i = 3
     * F32 f = numericCast<F32>(i);  // f = 3.0f
     * @endcode
     */
    template<Number To, Number From>
    [[nodiscard]]
    constexpr To numericCast(const From& v) noexcept
    {
        return details::numericCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    /**
     * @brief Extracts the underlying arithmetic value from a NumberLike type.
     * @details Safely converts both wrapped number types (Integer, Floating) and
     *          standard arithmetic types to their underlying arithmetic value.
     *          This function provides a uniform way to obtain the primitive value
     *          regardless of whether the input is wrapped or unwrapped.
     * @tparam T Type satisfying the NumberLike concept.
     * @param v The value to extract the underlying arithmetic from.
     * @return The underlying arithmetic value as NumberLikeType<T>.
     *
     * @note For Integer<T>/Floating<T>, returns T
     * @note For standard arithmetic types, returns the type itself
     * @note Always noexcept due to simple static_cast or conversion
     */
    template<NumberLike T>
    [[nodiscard]]
    constexpr NumberLikeType<T> numberLikeValue(const T& v) noexcept
    {
        return static_cast<NumberLikeType<T>>(v);
    }

    template<NumberLike T, NumberLikeType<T> V>
    [[nodiscard]]
    consteval NumberLikeType<T> numberLikeValue(NumberConstant<T, V>) noexcept
    {
        return V;
    }
}