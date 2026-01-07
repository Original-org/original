export module original.basic.algorithm.functor;
import original.basic.types;


export namespace original::algorithms
{
        /**
     * @brief Default equality comparator.
     *
     * Uses common type conversion and `operator==` to compare two values.
     * Suitable as the default predicate for equality-based algorithms.
     */
    struct EqualTo
    {
        /**
         * @brief Compares two values for equality after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value to compare.
         * @param rhs  Right-hand side value to compare.
         * @return true if the converted values are equal, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && EqualityComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs == type_rhs;
        }
    };

    /**
     * @brief Default three-way comparator.
     *
     * Converts operands to their common type and returns the result of
     * the three-way comparison (`<=>`). This is used by lexicographical
     * comparison helpers that rely on a three-way ordering.
     */
    struct ThreeWayCompare
    {
        /**
         * @brief Performs three-way comparison after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return std::strong_ordering (less, equal, or greater) reflecting the relationship.
         */
        template<typename T, typename U = T>
        requires HasCommonType<T, U> && ThreeWayComparable<CommonType<T, U>>
        constexpr auto operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs <=> type_rhs;
        }
    };

    /**
     * @brief Default less-than comparator.
     *
     * Compares two values after converting them to a common type using
     * `operator<`.
     */
    struct LessThan
    {
        /**
         * @brief Compares two values using less-than after common type conversion.
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return true if lhs is less than rhs after conversion, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && LessComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            using CommonType = CommonType<T, U>;
            const auto& type_lhs = static_cast<const CommonType&>(lhs);
            const auto& type_rhs = static_cast<const CommonType&>(rhs);
            return type_lhs < type_rhs;
        }
    };

    /**
     * @brief Default greater-than comparator.
     *
     * Implemented in terms of `LessThan` (i.e. `GreaterThan{}(a,b)` is
     * equivalent to `LessThan{}(b,a)`).
     */
    struct GreaterThan
    {
        /**
         * @brief Compares two values using greater-than (delegates to LessThan).
         *
         * @tparam T   Type of the left-hand side operand.
         * @tparam U   Type of the right-hand side operand (defaults to T).
         * @param lhs  Left-hand side value.
         * @param rhs  Right-hand side value.
         * @return true if lhs is greater than rhs, false otherwise.
         */
        template <typename T, typename U = T>
        requires HasCommonType<T, U> && LessComparable<CommonType<T, U>>
        constexpr bool operator()(const T& lhs, const U& rhs) const
        {
            return LessThan{}(rhs, lhs);
        }
    };
}