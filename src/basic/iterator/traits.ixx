module;
#include <concepts>
export module original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


export namespace original
{
    /**
     * @brief Primary template for iterator traits.
     * @tparam T Iterator type.
     */
    template<typename T>
    struct IterTrait {};

    /**
     * @brief Basic iterator concept.
     * @tparam T Type to test.
     * @note Requires copy constructible, dereferenceable, and equality comparable.
     */
    template<typename T>
    concept Iterator =
        std::copy_constructible<T> &&
        Dereferenceable<T> &&
        requires(T it)
    {
        { *it } -> std::same_as<decltype(*std::declval<T>())>;
        { it == it } -> std::convertible_to<bool>;
    };

    /**
     * @brief Forward iterator concept.
     * @tparam T Type to test.
     * @note Requires Iterator plus increment operations and default constructible.
     */
    template<typename T>
    concept ForwardIterator = Iterator<T> && requires(T it)
    {
        { ++it } -> std::same_as<T&>;
        { it++ } -> std::same_as<T>;
        requires std::default_initializable<T>;
    };

    /**
     * @brief Bidirectional iterator concept.
     * @tparam T Type to test.
     * @note Requires ForwardIterator plus decrement operations.
     */
    template<typename T>
    concept BidirectionalIterator = ForwardIterator<T> && requires(T it)
    {
        { --it } -> std::same_as<T&>;
        { it-- } -> std::same_as<T>;
    };

    /**
     * @brief Random access iterator concept.
     * @tparam T Iterator type.
     * @tparam DifferenceType Signed integral-like type for distance operations.
     * @note Requires BidirectionalIterator plus arithmetic and subscript operations.
     */
    template<typename T, typename DifferenceType>
    concept RandomAccessIterator
        = BidirectionalIterator<T> &&
          SignedIntegralLike<DifferenceType> &&
          requires(T it, DifferenceType dis)
    {
        { it + dis } -> std::same_as<T>;
        { dis + it } -> std::same_as<T>;
        { it - dis } -> std::same_as<T>;
        { it - it } -> std::same_as<DifferenceType>;
        { it += dis } -> std::same_as<T&>;
        { it -= dis } -> std::same_as<T&>;
        { it[dis] } -> std::same_as<decltype(*it)>;
        { it <=> it } -> StdThreeWayCompareResult;
    };

    /**
     * @brief Concept for types that have a DifferenceType member.
     * @tparam T Type to test.
     */
    template<typename T>
    concept HasDifferenceType = requires
    {
        typename T::DifferenceType;
    };

    /**
     * @brief Iterator traits specialization.
     * @tparam Iter Iterator type satisfying Iterator concept.
     */
    template<Iterator Iter>
    struct IterTrait<Iter>
    {
        using IterType = Iter;                  ///< The iterator type itself.
        using DerivedType = Iter::DerivedType;  ///< Type derived from the iterator.
        using ValueType = Iter::ValueType;      ///< Type of the dereferenced value.
        using ReferenceType = Iter::ReferenceType; ///< Reference type returned by dereference.
        using PointerType = Iter::PointerType;  ///< Pointer type for the value.
        using DifferenceType
            = std::conditional_t<HasDifferenceType<Iter>, typename Iter::DifferenceType, Diff>; ///< Distance type, defaults to Diff.
    };
}