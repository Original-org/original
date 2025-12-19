module;
#include <type_traits>
export module original.basic.types;

/**
 * @brief Namespace containing type traits and concepts for standard arithmetic types.
 */
export namespace original
{
    /**
     * @brief Concept that constrains a type to be a standard arithmetic type.
     * This concept is true for any type T where std::is_arithmetic_v<T> is true.
     * It includes integral and floating-point types.
     * @tparam T The type to check.
     */
    template<typename T>
    concept StdArithmetic = std::is_arithmetic_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard arithmetic type.
     * Equivalent to std::is_arithmetic_v<T>.
     * @tparam T The type to check.
     */
    template<typename T>
    constexpr bool IS_STD_ARITHMETIC = std::is_arithmetic_v<T>;

    /**
     * @brief Concept that constrains a type to be a standard integral type.
     * This concept is true for any type T where std::is_integral_v<T> is true
     * (including bool and character types).
     * @tparam T The type to check.
     */
    template<typename T>
    concept StdIntegral = std::is_integral_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard integral type.
     * Equivalent to std::is_integral_v<T>.
     * @tparam T The type to check.
     */
    template<typename T>
    constexpr bool IS_STD_INTEGRAL = std::is_integral_v<T>;

    /**
     * @brief Concept that constrains a type to be a signed standard integral type.
     * Requires StdIntegral<T> and that T is signed.
     * @tparam T The type to check.
     */
    template<typename T>
    concept StdSignedIntegral = StdIntegral<T> && std::is_signed_v<T>;

    template<typename T>
    constexpr bool IS_STD_SIGNED_INTEGRAL = StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Concept that constrains a type to be an unsigned standard integral type.
     * Requires StdIntegral<T> and that T is unsigned.
     * @tparam T The type to check.
     */
    template<typename T>
    concept StdUnsignedIntegral = StdIntegral<T> && std::is_unsigned_v<T>;

    template<typename T>
    constexpr bool IS_STD_UNSIGNED_INTEGRAL = StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Concept that constrains a type to be a standard floating-point type.
     * This concept is true for any type T where std::is_floating_point_v<T> is true.
     * @tparam T The type to check.
     */
    template<typename T>
    concept StdFloating = std::is_floating_point_v<T>;

    /**
     * @brief Compile-time constant indicating whether T is a standard floating-point type.
     * Equivalent to std::is_floating_point_v<T>.
     * @tparam T The type to check.
     */
    template<typename T>
    constexpr bool IS_STD_FLOATING = std::is_floating_point_v<T>;
}