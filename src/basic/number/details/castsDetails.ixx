module;
module original.basic.number.casts:details;
import original.basic.number.traits;
import original.basic.number.numberImpl;
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