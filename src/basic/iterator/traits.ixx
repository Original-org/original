module;
#include <concepts>
#include <memory>
export module original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


namespace original::details
{
    /**
     * @brief Concept for types that have a DifferenceType member.
     * @tparam T Type to test.
     */
    template<typename T>
    concept HasDifferenceType = requires
    {
        typename T::DifferenceType;
    };

    template<typename T>
    concept HasCRTPDerived = requires
    {
        typename T::DerivedType;
    };

    template<typename T>
    struct IterCRTPTrait
    {
        using DerivedType = T;
    };

    template<HasCRTPDerived T>
    struct IterCRTPTrait<T>
    {
        using DerivedType = T::DerivedType;
    };

    template<typename>
    struct DifferenceTrait
    {
        using DifferenceType = Diff::Type;
    };

    template<HasDifferenceType T>
    struct DifferenceTrait<T>
    {
        using DifferenceType = T::DifferenceType;
    };

    template<typename T>
    using DifferenceType = DifferenceTrait<T>::DifferenceType;

    template<typename T>
    using IterCRTPDerivedType = IterCRTPTrait<T>::DerivedType;
}

export namespace original
{
    template<typename T>
    concept SourceTag = StdObject<T> &&
    requires
    {
        typename T::SourceType;
    };

    struct NoSource
    {
        using SourceType = void;
    };

    template<StdObject T>
    struct SpecifiedSource
    {
        using SourceType = T;
    };

    /**
     * @brief Primary template for iterator traits.
     * @tparam T Iterator type.
     */
    template<typename T>
    struct IterTraits {};

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
        { *it } -> StdSame<decltype(*std::declval<T>())>;
        { it == it } -> StdConvertible<bool>;
    };

    /**
     * @brief Forward iterator concept.
     * @tparam T Type to test.
     * @note Requires Iterator plus increment operations and default constructible.
     */
    template<typename T>
    concept ForwardIterator = Iterator<T> && requires(T it)
    {
        { ++it } -> StdSame<T&>;
        { it++ } -> StdSame<T>;
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
        { --it } -> StdSame<T&>;
        { it-- } -> StdSame<T>;
    };

    /**
     * @brief Random access iterator concept.
     * @tparam T Iterator type.
     * @note Requires BidirectionalIterator plus arithmetic and subscript operations.
     */
    template<typename T>
    concept RandomAccessIterator
        = BidirectionalIterator<T> &&
          requires(T it,
          details::DifferenceType<T> dis)
    {
        { it + dis } -> StdSame<T>;
        { dis + it } -> StdSame<T>;
        { it - dis } -> StdSame<T>;
        { it - it } -> StdSame<details::DifferenceType<T>>;
        { it += dis } -> StdSame<T&>;
        { it -= dis } -> StdSame<T&>;
        { it[dis] } -> StdSame<decltype(*it)>;
        { it <=> it } -> StdThreeWayCompareResult;
    };

    template<typename T>
    concept ContiguousIterator = RandomAccessIterator<T> &&
    requires(T it)
    {
        requires StdSame<NumberLikeType<details::DifferenceType<T>>, std::ptrdiff_t>;
        { std::to_address(it) } -> StdConvertible<const RemoveCVRefType<decltype(*it)>*>;
    };

    template<typename It>
    concept IteratorView =
    Iterator<It> &&
    requires(It it) {
        { it.iterator() } -> Iterator;
    };

    /**
     * @brief Iterator traits specialization.
     * @tparam Iter Iterator type satisfying Iterator concept.
     */
    template<Iterator Iter>
    struct IterTraits<Iter>
    {
        using IterType = Iter;                  ///< The iterator type itself.
        using DerivedType = details::IterCRTPDerivedType<Iter>;  ///< Type derived from the iterator.
        using ValueType = Iter::ValueType;      ///< Type of the dereferenced value.
        using ReferenceType = Iter::ReferenceType; ///< Reference type returned by dereference.
        using PointerType = Iter::PointerType;  ///< Pointer type for the value.
        using DifferenceType = details::DifferenceType<Iter>; ///< Distance type, defaults to Diff.
    };

    template<typename T>
    concept HasIterator = StdObject<T> && requires
    {
        typename T::IterType;
        typename T::ConstIterType;
    };
}