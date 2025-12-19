module;
#include <cmath>
#include <limits>
export module original.basic.number.algorithms;
import original.basic.number.traits;
import original.basic.number.numberImpl;


export namespace original
{
    template<Number T>
    [[nodiscard]] constexpr T minimum() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::min()};
    }

    template<Number T>
    [[nodiscard]] constexpr T maximum() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::max()};
    }

    template<SignedInteger T>
    [[nodiscard]] constexpr T abs(const T& v) noexcept
    {
        return v < T{} ? -v : v;
    }

    template<FloatingPoint T>
    [[nodiscard]] constexpr T abs(const T& v) noexcept
    {
        return T{std::fabs(v.value())};
    }

    template<FloatingPoint T>
    [[nodiscard]] constexpr T infinity() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::infinity()};
    }

    template<FloatingPoint T>
    [[nodiscard]] constexpr T nan() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::quiet_NaN()};
    }

    template<FloatingPoint T>
    [[nodiscard]] constexpr bool isFinite(const T& v) noexcept
    {
        return std::isfinite(v.value());
    }

    template<FloatingPoint T>
    [[nodiscard]] constexpr bool isNaN(const T& v) noexcept
    {
        return std::isnan(v.value());
    }
}
