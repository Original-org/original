module;
#include <cmath>
#include <limits>
export module original.basic.number.algorithms;
import original.basic.number.traits;
import original.basic.number.numberImpl;


export namespace original
{
    /**
     * @brief Get the minimum finite value representable by the numeric type.
     *
     * @tparam T Number type
     * @return T Minimum representable value
     *
     * @note For floating-point types, returns the most negative finite value
     * @note For integral types, returns the minimum value (negative for signed)
     *
     * @example
     * @code
     * auto min_i32 = minimum<I32>();  // Returns -2147483648
     * auto min_f64 = minimum<F64>();  // Returns -1.79769e+308
     * auto min_u16 = minimum<U16>();  // Returns 0
     * @endcode
     */
    template<Number T>
    [[nodiscard]] constexpr T minimum() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::min()};
    }

    /**
     * @brief Get the maximum finite value representable by the numeric type.
     *
     * @tparam T Number type
     * @return T Maximum representable value
     *
     * @note For floating-point types, returns the largest finite value
     * @note For integral types, returns the maximum value
     *
     * @example
     * @code
     * auto max_i32 = maximum<I32>();  // Returns 2147483647
     * auto max_f64 = maximum<F64>();  // Returns 1.79769e+308
     * auto max_u16 = maximum<U16>();  // Returns 65535
     * @endcode
     */
    template<Number T>
    [[nodiscard]] constexpr T maximum() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::max()};
    }

    /**
     * @brief Compute absolute value of a signed integer.
     *
     * @tparam T Signed integer type
     * @param v Input value
     * @return T Absolute value |v|
     *
     * @pre T must be a signed integer type
     * @note For T{}, returns T{}
     * @warning For minimum(), behavior is two's complement overflow
     *
     * @example
     * @code
     * I32 x = -42;
     * I32 y = abs(x);  // y = 42
     *
     * I32 z = 0;
     * I32 w = abs(z);  // w = 0
     * @endcode
     */
    template<SignedInteger T>
    [[nodiscard]] constexpr T abs(const T& v) noexcept
    {
        return v < T{} ? -v : v;
    }

    /**
     * @brief Compute absolute value of a floating-point number.
     *
     * @tparam T Floating-point type
     * @param v Input value
     * @return T Absolute value |v|
     *
     * @pre T must be a floating-point type
     * @note Handles special values: NaN, infinity, zero
     *
     * @example
     * @code
     * F64 a = -3.14;
     * F64 b = abs(a);  // b = 3.14
     *
     * F64 c = infinity<F64>();
     * F64 d = abs(c);  // d = infinity
     *
     * F64 e = -0.0;
     * F64 f = abs(e);  // f = 0.0
     * @endcode
     */
    template<FloatingPoint T>
    [[nodiscard]] constexpr T abs(const T& v) noexcept
    {
        return T{std::fabs(v.value())};
    }

    /**
     * @brief Get positive infinity representation for floating-point type.
     *
     * @tparam T Floating-point type
     * @return T Positive infinity value
     *
     * @pre T must be a floating-point type
     * @note Only meaningful for floating-point types
     * @note Returns +∞, not -∞
     *
     * @example
     * @code
     * F32 inf32 = infinity<F32>();
     * F64 inf64 = infinity<F64>();
     *
     * bool test = isFinite(inf32);  // false
     * bool test2 = inf64 > maximum<F64>();  // true
     * @endcode
     */
    template<FloatingPoint T>
    [[nodiscard]] constexpr T infinity() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::infinity()};
    }

    /**
     * @brief Get quiet NaN (Not-a-Number) representation for floating-point type.
     *
     * @tparam T Floating-point type
     * @return T Quiet NaN value
     *
     * @pre T must be a floating-point type
     * @note Returns a quiet NaN, not a signaling NaN
     * @note NaN values are never equal to themselves
     *
     * @example
     * @code
     * F32 nan32 = nan<F32>();
     * F64 nan64 = nan<F64>();
     *
     * bool test = isNaN(nan32);     // true
     * bool test2 = nan64 == nan64;  // false (NaN != NaN)
     * @endcode
     */
    template<FloatingPoint T>
    [[nodiscard]] constexpr T nan() noexcept
    {
        using NumberType = NumberType<T>;
        return T{std::numeric_limits<NumberType>::quiet_NaN()};
    }

    /**
     * @brief Check if a floating-point value is finite.
     *
     * @tparam T Floating-point type
     * @param v Value to test
     * @return true if v is finite (not NaN and not infinity)
     * @return false if v is NaN or infinity
     *
     * @pre T must be a floating-point type
     * @note Finite values include zero, subnormal, and normal numbers
     *
     * @example
     * @code
     * F64 a = 3.14;
     * F64 b = infinity<F64>();
     * F64 c = nan<F64>();
     *
     * bool test1 = isFinite(a);  // true
     * bool test2 = isFinite(b);  // false
     * bool test3 = isFinite(c);  // false
     * @endcode
     */
    template<FloatingPoint T>
    [[nodiscard]] constexpr bool isFinite(const T& v) noexcept
    {
        return std::isfinite(v.value());
    }

    /**
     * @brief Check if a floating-point value is NaN (Not-a-Number).
     *
     * @tparam T Floating-point type
     * @param v Value to test
     * @return true if v is NaN (quiet or signaling)
     * @return false if v is a valid number or infinity
     *
     * @pre T must be a floating-point type
     * @note Detects both quiet and signaling NaN values
     *
     * @example
     * @code
     * F32 a = 2.5f;
     * F32 b = nan<F32>();
     * F32 c = infinity<F32>();
     *
     * bool test1 = isNaN(a);  // false
     * bool test2 = isNaN(b);  // true
     * bool test3 = isNaN(c);  // false
     * @endcode
     */
    template<FloatingPoint T>
    [[nodiscard]] constexpr bool isNaN(const T& v) noexcept
    {
        return std::isnan(v.value());
    }
}