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
    concept SourceTag = IsObject<T> &&
    requires
    {
        typename T::SourceType;
    };

    struct NoSource
    {
        using SourceType = void;
    };

    template<IsObject T>
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
        { *it } -> SameType<decltype(*std::declval<T>())>;
        { it == it } -> Convertible<bool>;
    };

    /**
     * @brief Forward iterator concept.
     * @tparam T Type to test.
     * @note Requires Iterator plus increment operations and default constructible.
     */
    template<typename T>
    concept ForwardIterator = Iterator<T> && requires(T it)
    {
        { ++it } -> SameType<T&>;
        { it++ } -> SameType<T>;
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
        { --it } -> SameType<T&>;
        { it-- } -> SameType<T>;
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
        { it + dis } -> SameType<T>;
        { dis + it } -> SameType<T>;
        { it - dis } -> SameType<T>;
        { it - it } -> SameType<details::DifferenceType<T>>;
        { it += dis } -> SameType<T&>;
        { it -= dis } -> SameType<T&>;
        { it[dis] } -> SameType<decltype(*it)>;
        { it <=> it } -> StdThreeWayCompareResult;
    };

    template<typename T>
    concept ContiguousIterator = RandomAccessIterator<T> &&
    requires(T it)
    {
        requires SameType<NumberLikeType<details::DifferenceType<T>>, std::ptrdiff_t>;
        { std::to_address(it) } -> Convertible<const RemoveCVRefType<decltype(*it)>*>;
    };

    template<typename It>
    concept IteratorView =
    Iterator<It> &&
    requires(It it) {
        { it.iterator() } -> Iterator;
    };

    template <typename T>
    struct IterTraits<T*>
    {
        using IterType       = T*;
        using DerivedType    = details::IterCRTPDerivedType<IterType>;
        using PointerType    = T*;
        using ValueType      = std::remove_cv_t<T>;
        using ReferenceType  = std::add_lvalue_reference_t<T>;
        using DifferenceType = details::DifferenceType<T*>;
    };

    template <typename T>
    struct IterTraits<const T*>
    {
        using IterType       = const T*;
        using DerivedType    = details::IterCRTPDerivedType<IterType>;
        using PointerType    = const T*;
        using ValueType      = std::remove_cv_t<T>;
        using ReferenceType  = std::add_lvalue_reference_t<const T>;
        using DifferenceType = details::DifferenceType<const T*>;
    };

    template <typename T>
    struct IterTraits<volatile T*>
    {
        using IterType       = volatile T*;
        using DerivedType    = details::IterCRTPDerivedType<IterType>;
        using PointerType    = volatile T*;
        using ValueType      = std::remove_cv_t<T>;
        using ReferenceType  = std::add_lvalue_reference_t<volatile T>;
        using DifferenceType = details::DifferenceType<volatile T*>;
    };

    template <typename T>
    struct IterTraits<const volatile T*>
    {
        using IterType       = const volatile T*;
        using DerivedType    = details::IterCRTPDerivedType<IterType>;
        using PointerType    = const volatile T*;
        using ValueType      = std::remove_cv_t<T>;
        using ReferenceType  = std::add_lvalue_reference_t<const volatile T>;
        using DifferenceType = details::DifferenceType<const volatile T*>;
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
    concept HasIterator = IsObject<T> && requires
    {
        typename T::IterType;
        typename T::ConstIterType;
    };
}