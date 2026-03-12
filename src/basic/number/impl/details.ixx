module;
#include <stdexcept>
#include <limits>
#include <cstddef> // NOLINT
export module original.basic.number.impl:details;
import :declaration;


namespace original::details
{
    /**
     * @brief Compile-time range check for numeric literal conversions.
     * @tparam To Destination arithmetic type.
     * @tparam From Source arithmetic type.
     * @tparam V Value to check.
     * @note Triggers static assertion if value exceeds destination type's range.
     */
    template<StdArithmetic To, StdArithmetic From, From V>
    consteval void rangeCheckLiterals()
    {
        static_assert(
            V <= static_cast<From>(std::numeric_limits<To>::max()),
            "Number literal out of range"
        );
    }

    /**
     * @brief Parse decimal string to integral value at compile time.
     * @tparam To Destination integral type.
     * @param str Character array to parse.
     * @param n Number of characters to parse.
     * @return Parsed integral value.
     * @throw std::invalid_argument If string contains non-digit characters.
     */
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

    /**
     * @brief Create Integer instance from character sequence literal.
     * @tparam To Destination arithmetic type.
     * @tparam Cs Character sequence representing the integer.
     * @return Integer instance with parsed and range-checked value.
     * @note Performs compile-time parsing and range checking.
     */
    template<StdArithmetic To, char... Cs>
    consteval Integer<To> literalIntegral()
    {
        constexpr char INPUT[] {Cs...};
        constexpr auto PARSED_RESULT = parseIntegral<unsigned long long>(INPUT, sizeof...(Cs));
        rangeCheckLiterals<To, unsigned long long, PARSED_RESULT>();
        return Integer<To>{static_cast<To>(PARSED_RESULT)};
    }

    /**
     * @brief Checked addition with overflow detection.
     * @tparam T Arithmetic type.
     * @param a Left operand.
     * @param b Right operand.
     * @return Sum of a and b.
     * @throw std::overflow_error If addition would overflow.
     */
    template<StdArithmetic T>
    constexpr T checkedAdd(T a, T b)
    {
        if constexpr (StdSignedIntegral<T>)
        {
            if ((b > 0 && a > std::numeric_limits<T>::max() - b) ||
                (b < 0 && a < std::numeric_limits<T>::min() - b))
            {
                throw std::overflow_error{"Add operation overflows"};
            }
        }
        else
        {
            if (a > std::numeric_limits<T>::max() - b)
                throw std::overflow_error{"Add operation overflows"};
        }
        return a + b;
    }

    /**
     * @brief Checked subtraction with overflow detection.
     * @tparam T Arithmetic type.
     * @param a Left operand.
     * @param b Right operand.
     * @return Difference of a and b.
     * @throw std::overflow_error If subtraction would overflow.
     */
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

    /**
     * @brief Checked multiplication with overflow detection.
     * @tparam T Arithmetic type.
     * @param a Left operand.
     * @param b Right operand.
     * @return Product of a and b.
     * @throw std::overflow_error If multiplication would overflow.
     */
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

    /**
     * @brief Checked division with zero and overflow detection.
     * @tparam T Arithmetic type.
     * @param a Left operand.
     * @param b Right operand.
     * @return Quotient of a divided by b.
     * @throw std::logic_error If dividing by zero.
     * @throw std::overflow_error If division would overflow (signed min / -1).
     */
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

    /**
     * @brief Checked modulo with zero and overflow detection.
     * @tparam T Arithmetic type.
     * @param a Left operand.
     * @param b Right operand.
     * @return Remainder of a modulo b.
     * @throw std::logic_error If modulo by zero.
     * @throw std::overflow_error If modulo would overflow.
     */
    template<StdArithmetic T>
    constexpr T checkedMod(T a, T b)
    {
        if (b == 0)
            throw std::logic_error{"modulo by zero"};

        if constexpr (StdSignedIntegral<T>)
        {
            if (b == -1 && b == std::numeric_limits<T>::min())
            if (a == std::numeric_limits<T>::min() && b == static_cast<T>(-1))
                throw std::overflow_error{"Mod operation overflow"};
        }

        return a % b;
    }

    /**
     * @brief Checked negation with overflow detection.
     * @tparam T Signed integral type.
     * @param a Operand to negate.
     * @return Negated value.
     * @throw std::overflow_error If negation would overflow (signed min).
     */
    template<StdSignedIntegral T>
    constexpr T checkedNeg(T a)
    {
        if (a == std::numeric_limits<T>::min())
            throw std::overflow_error{"Neg operation overflow"};

        return -a;
    }

    /**
     * @brief Checked left shift with overflow detection.
     * @tparam T Arithmetic type.
     * @param a Value to shift.
     * @param shift Number of bits to shift.
     * @return Left-shifted value.
     * @throw std::overflow_error If shift would overflow.
     */
    template<StdArithmetic T>
    constexpr T checkedShiftLeft(T a, std::size_t shift)
    {
        if (constexpr std::size_t bits = std::numeric_limits<T>::digits;
            shift >= bits)
            throw std::overflow_error{"Shift operation overflows"};

        if constexpr (StdSignedIntegral<T>)
        {
            if (a < 0)
                throw std::overflow_error{"Shift operation overflows"};
        }

        if (a > (std::numeric_limits<T>::max() >> shift))
            throw std::overflow_error{"Shift operation overflows"};

        return a << shift;
    }

    /**
     * @brief Checked right shift with overflow detection.
     * @tparam T Arithmetic type.
     * @param a Value to shift.
     * @param shift Number of bits to shift.
     * @return Right-shifted value.
     * @throw std::overflow_error If shift count exceeds bit width.
     */
    template<StdArithmetic T>
    constexpr T checkedShiftRight(T a, std::size_t shift)
    {
        if (constexpr std::size_t bits = std::numeric_limits<T>::digits;
            shift >= bits)
            throw std::overflow_error("right shift count out of range");

        return a >> shift;
    }

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

        /** @brief Default constructor. */
        constexpr Number() noexcept = default;

        /**
         * @brief Constructs from an underlying value.
         */
        explicit constexpr Number(T value) noexcept : value_(value) {}

        /** @brief Copy constructor. */
        constexpr Number(const Number&) noexcept = default;
        /** @brief Copy assignment operator. */
        constexpr Number& operator=(const Number&) noexcept = default;
        /** @brief Move constructor. */
        constexpr Number(Number&&) noexcept = default;
        /** @brief Move assignment operator. */
        constexpr Number& operator=(Number&&) noexcept = default;

    public:
        /**
         * @brief Returns the underlying value.
         */
        [[nodiscard]] constexpr T value() const noexcept { return this->value_; }
    };
}