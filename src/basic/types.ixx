module;
#include <type_traits>
#include <compare>
#include <concepts>

export module original.basic.types;

/**
 * @brief Namespace containing type traits and concepts for standard arithmetic types.
 */
export namespace original
{
    /**
     * @brief Concept that constrains a type to be a standard object type.
     *
     * @note True for any type that is not a function, reference, or void
     */
    template<typename T>
    concept StdObject = std::is_object_v<T>;

    /**
     * @brief Concept that constrains a type to be an empty class type.
     *
     * @note True for classes with no non-static data members and no virtual functions.
     * Useful for optimizing empty base class storage in EBO (Empty Base Optimization).
     */
    template<typename T>
    concept StdEmpty = std::is_empty_v<T>;

    /**
     * @brief Concept that constrains a type to be dereferenceable.
     * A type satisfies this concept if `*t` is a valid expression.
     */
    template<typename T>
    concept Dereferenceable = requires(T t) { *t; };

    /**
     * @brief Concept for convertible types.
     * @tparam From Source type.
     * @tparam To Target type.
    */
    template<typename From, typename To>
    concept StdConvertible = std::is_convertible_v<From, To>;

    /**
     * @brief Concept for same types.
     * @tparam T First type.
     * @tparam U Second type.
     */
    template<typename T, typename U>
    concept StdSame = std::same_as<T, U>;

    /**
     * @brief Concept that constrains a type to be a raw pointer type.
     */
    template<typename T>
    concept StdPointer = std::is_pointer_v<T>;

    /**
     * @brief Concept that constrains a type to be a reference type.
     */
    template<typename T>
    concept StdReference = std::is_reference_v<T>;

    template<typename T>
    using RemoveCVRefType = std::remove_cvref_t<T>;

    /**
     * @brief Concept that constrains a type to be a standard arithmetic type.
     */
    template<typename T>
    concept StdArithmetic = std::is_arithmetic_v<T>;

    /**
     * @brief Concept that constrains a type to be a standard integral type.
     */
    template<typename T>
    concept StdIntegral = std::is_integral_v<T>;

    /**
     * @brief Concept that constrains a type to be a signed standard integral type.
     */
    template<typename T>
    concept StdSignedIntegral =
        StdIntegral<T> && std::is_signed_v<T>;

    /**
     * @brief Concept that constrains a type to be an unsigned standard integral type.
     */
    template<typename T>
    concept StdUnsignedIntegral =
        StdIntegral<T> && std::is_unsigned_v<T>;

    /**
     * @brief Concept that constrains two integral types to have the same signedness.
     */
    template<typename T, typename U>
    concept StdSameSignIntegral =
        (StdSignedIntegral<T> && StdSignedIntegral<U>) ||
        (StdUnsignedIntegral<T> && StdUnsignedIntegral<U>);

    /**
     * @brief Concept that constrains a type to be a standard floating-point type.
     */
    template<typename T>
    concept StdFloating = std::is_floating_point_v<T>;

    /**
     * @brief Concept that constrains a type to be a valid three-way comparison result.
     */
    template<typename T>
    concept StdThreeWayCompareResult =
        StdSame<T, std::strong_ordering> ||
        StdSame<T, std::weak_ordering>   ||
        StdSame<T, std::partial_ordering>;
}