module;
#include <type_traits>
export module original.basic.number.traits;
import original.basic.types;
import original.basic.number.numberImpl;

/**
 * @brief Namespace containing type traits for the strongly-typed number wrappers.
 */
export namespace original
{
    /**
     * @struct NumberTrait
     * @brief Trait to detect whether a type is one of the strongly-typed number wrappers.
     * Specializations exist for Integer<T> and Floating<T>.
     * @tparam T Type to test.
     */
    template<typename T>
    struct NumberTrait : std::false_type {};

    /** @brief Specialization for Integer types. */
    template<StdIntegral T>
    struct NumberTrait<Integer<T>> : std::true_type
    {
        using Type = T;
    };

    /** @brief Specialization for Floating types. */
    template<StdFloating T>
    struct NumberTrait<Floating<T>> : std::true_type
    {
        using Type = T;
    };

    /**
     * @brief Compile-time constant indicating whether T is a strongly-typed number wrapper.
     * @tparam T The type to check.
     */
    template<typename T>
    constexpr bool IS_NUMBER = NumberTrait<T>::value;

    /**
     * @brief Compile-time constant indicating whether T is either a strongly-typed number
     * or a standard arithmetic type.
     * @tparam T The type to check.
     */
    template<typename T>
    constexpr bool IS_NUMBER_LIKE = IS_NUMBER<T> || IS_STD_ARITHMETIC<T>;

    /**
     * @brief Concept requiring T to be a strongly-typed number wrapper.
     * @tparam T The type to constrain.
     */
    template<typename T>
    concept Number = IS_NUMBER<T>;

    /**
     * @brief Concept requiring T to be either a strongly-typed number wrapper
     * or a standard arithmetic type.
     * @tparam T The type to constrain.
     */
    template<typename T>
    concept NumberLike = IS_NUMBER_LIKE<T>;

    /**
     * @brief Extracts the underlying type from a strongly-typed number wrapper.
     * @tparam T A type satisfying the Number concept.
     */
    template<Number T>
    using NumberType = NumberTrait<T>::Type;

    /**
     * @struct NumberLikeTrait
     * @brief Trait to obtain the underlying arithmetic type from Number
     * or standard arithmetic types.
     * @tparam T Type to query.
     */
    template<typename T>
    struct NumberLikeTrait {};

    /** @brief Specialization for strongly-typed number wrappers. */
    template<Number T>
    struct NumberLikeTrait<T>
    {
        using Type = NumberType<T>;
    };

    /** @brief Specialization for plain standard arithmetic types. */
    template<StdArithmetic T>
    struct NumberLikeTrait<T>
    {
        using Type = T;
    };

    /**
     * @brief Alias template to retrieve the underlying arithmetic type
     * for NumberLike types.
     * @tparam T A type satisfying the NumberLike concept.
     */
    template<NumberLike T>
    using NumberLikeType = NumberLikeTrait<T>::Type;

    template<typename T>
    constexpr bool IS_UNSIGNED_INTEGER = IS_NUMBER<T> && IS_STD_UNSIGNED_INTEGRAL<NumberType<T>>;

    template<typename T>
    constexpr bool IS_SIGNED_INTEGER = IS_NUMBER<T> && IS_STD_SIGNED_INTEGRAL<NumberType<T>>;

    template<typename T>
    constexpr bool IS_FLOATING_POINT = IS_NUMBER<T> && IS_STD_FLOATING<NumberType<T>>;

    template<typename T>
    concept UnsignedInteger = IS_UNSIGNED_INTEGER<T>;

    template<typename T>
    concept SignedInteger = IS_SIGNED_INTEGER<T>;

    template<typename T>
    concept FloatingPoint = IS_FLOATING_POINT<T>;

    template<typename T, typename U>
    constexpr bool HAS_SAME_SIGN_INTEGRAL
        = IS_NUMBER<T> &&
          IS_NUMBER<U> &&
          STD_HAS_SAME_SIGN_INTEGRAL<NumberType<T>, NumberType<U>>;

    template<typename T, typename U>
    concept SameSignIntegral = HAS_SAME_SIGN_INTEGRAL<T, U>;
}