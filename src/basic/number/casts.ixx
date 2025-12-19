module;
#include <cstdint>
export module original.basic.number.casts;
import original.basic.number.traits;
import original.basic.number.numberImpl;
import original.basic.types;


namespace original::details
{
    template<StdIntegral To, StdIntegral From>
    requires (sizeof(To) < sizeof(From) && StdSameSignIntegral<To, From>)
    constexpr Integer<To> narrowCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    template<StdFloating To, StdFloating From>
    requires (sizeof(To) < sizeof(From))
    constexpr Floating<To> narrowCastImpl(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    template<StdIntegral To, StdIntegral From>
    requires (sizeof(To) == sizeof(From) && !StdSameSignIntegral<To, From>)
    constexpr Integer<To> signCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    template<StdIntegral To, StdIntegral From>
    constexpr Integer<To> numericCastImpl(const Integer<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }

    template<StdFloating To, StdFloating From>
    constexpr Floating<To> numericCastImpl(const Floating<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    template<StdFloating To, StdIntegral From>
    constexpr Floating<To> numericCastImpl(const Integer<From>& v) noexcept
    {
        return Floating<To>{static_cast<To>(v.value())};
    }

    template<StdIntegral To, StdFloating From>
    constexpr Integer<To> numericCastImpl(const Floating<From>& v) noexcept
    {
        return Integer<To>{static_cast<To>(v.value())};
    }
}

export namespace original
{
    template<Number To, Number From>
    [[nodiscard]]
    constexpr To narrowCast(const From& v) noexcept
    {
        return details::narrowCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    template<Number To, Number From>
    [[nodiscard]]
    constexpr To signCast(const From& v) noexcept
    {
        return details::signCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    template<Number To, Number From>
    [[nodiscard]]
    constexpr To numericCast(const From& v) noexcept
    {
        return details::numericCastImpl<NumberType<To>, NumberType<From>>(v);
    }

    [[nodiscard]]
    constexpr U64 fromStdSize(const size_t n) noexcept
    {
        return U64{n};
    }

    constexpr size_t toStdSize(const U64& v) noexcept
    {
        return v.value();
    }
}
