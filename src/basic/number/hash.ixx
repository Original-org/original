module;
#include <cstddef>
#include <functional>
export module original.basic.number.hash;
import original.basic.number.traits;
import original.basic.number.casts;


export namespace std // NOLINT
{
    /**
     * @brief Hash specialization for Number types.
     *
     * Provides std::hash support for all Number types defined in the original namespace.
     * This enables Number objects to be used as keys in unordered containers (std::unordered_map,
     * std::unordered_set, etc.) and with other hash-based algorithms.
     *
     * @tparam N Number type satisfying the original::Number concept.
     *
     * @note The hash is computed from the underlying arithmetic value using std::hash specialized
     *       for that type, ensuring consistency with hashing of built-in arithmetic types.
     */
    template<original::Number N>
    struct hash<N>
    {
        /**
         * @brief Computes hash value for a Number object.
         *
         * Converts the Number to its underlying arithmetic type and computes the hash
         * using the corresponding std::hash specialization for that type.
         *
         * @param n Number object to hash.
         * @return Hash value of type std::size_t.
         *
         * @note This operation is noexcept and can be evaluated at compile time when appropriate.
         *       The hash value is consistent with direct hashing of the underlying arithmetic value.
         *
         * @example
         * @code
         * original::I32 value{42};
         * std::hash<original::I32> hasher;
         * std::size_t h = hasher(value);  // Same as std::hash<std::int32_t>{}(42)
         * @endcode
         */
        constexpr size_t operator()(N const& n) const noexcept
        {
            std::hash<original::NumberLikeType<N>> hash;
            return hash(original::numberLikeValue(n));
        }
    };
}