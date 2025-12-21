module;
#include <cstddef>
export module original.basic.number.casts;
import :details;


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
     * @brief Convert standard size_t to U64.
     *
     * @param n Standard size_t value
     * @return U64 Converted unsigned 64-bit integer
     *
     * @note Useful for interfacing with standard library functions
     */
    [[nodiscard]]
    constexpr U64 fromStdSize(const std::size_t n) noexcept
    {
        return U64{n};
    }

    /**
     * @brief Convert U64 to standard size_t.
     *
     * @param v U64 value to convert
     * @return size_t Converted standard size type
     *
     * @note Useful for passing to standard library functions
     * @warning May truncate on platforms where size_t < 64 bits
     */
    [[nodiscard]]
    constexpr std::size_t toStdSize(const U64& v) noexcept
    {
        return v.value();
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
}