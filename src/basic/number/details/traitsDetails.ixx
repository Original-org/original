module original.basic.number.traits:details;
import original.basic.types;
import original.basic.number.numberImpl;


namespace original::details
{
    /**
     * @brief Primary template: not a number wrapper.
     * @tparam T Type to test.
     * @note Intentionally left incomplete to enforce explicit specializations.
     */
    template<typename T>
    struct NumberTrait;

    /**
     * @brief Integer<T> specialization.
     * @tparam T Underlying integral type.
     * @note Provides Type alias to the underlying integral type.
     */
    template<StdIntegral T>
    struct NumberTrait<Integer<T>>
    {
        using Type = T;  ///< Underlying integral type.
    };

    /**
     * @brief Floating<T> specialization.
     * @tparam T Underlying floating-point type.
     * @note Provides Type alias to the underlying floating-point type.
     */
    template<StdFloating T>
    struct NumberTrait<Floating<T>>
    {
        using Type = T;  ///< Underlying floating-point type.
    };

    template<typename T, bool IsNumber>
    struct NumberLikeTypeImpl;

    template<typename T>
    struct NumberLikeTypeImpl<T, true>
    {
        using type = NumberTrait<T>::Type;  ///< Underlying type of the number wrapper.
    };

    template<typename T>
    struct NumberLikeTypeImpl<T, false>
    {
        using type = T;  ///< The arithmetic type itself.
    };
}