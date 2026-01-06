module;
#include <type_traits>
export module original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;

/**
 * @addtogroup RangeTraits
 * @{
 */

export namespace original
{
    /**
     * @brief Returns a pointer to the beginning of a C-style array.
     *
     * @tparam T Element type
     * @tparam Size Array size
     * @param arr Reference to the array
     * @return Pointer to the first element
     *
     * Overload for built-in arrays to enable range-based operations.
     */
    template<typename T, Size::Type Size>
    T* begin(T (&arr) [Size])
    {
        return arr;
    }

    /**
     * @brief Returns a pointer to one past the end of a C-style array.
     *
     * @tparam T Element type
     * @tparam Size Array size
     * @param arr Reference to the array
     * @return Pointer past the last element
     */
    template<typename T, Size::Type Size>
    T* end(T (&arr) [Size])
    {
        return arr + Size;
    }

    template<typename T, Size::Type Size>
    const T* begin(const T (&arr) [Size])
    {
        return arr;
    }

    template<typename T, Size::Type Size>
    const T* end(const T (&arr) [Size])
    {
        return arr + Size;
    }

    /**
     * @brief Concept identifying types that model a range.
     *
     * @tparam R The type to check
     *
     * A type satisfies this concept if it provides either member functions
     * `begin()` and `end()`, or if non-member `begin(r)` and `end(r)` are
     * available, and the begin expression yields an iterator.
     *
     * @code
     * static_assert(Range<std::vector<int>>);      // true (member begin/end)
     * static_assert(Range<int[5]>);                // true (non-member overloads)
     * static_assert(!Range<int>);                  // false
     * @endcode
     */
    template<typename R>
    concept Range =
    requires(R& r) {
        { r.begin() } -> Iterator;
        { r.end() };
    } ||
    requires(R& r) {
        { begin(r) } -> Iterator;
        { end(r) };
    };

    /**
     * @brief Concept for ranges where begin and end iterators have the same type.
     *
     * @tparam R The range type
     *
     * This refines Range by requiring that the iterator type returned by
     * `begin()` matches the sentinel type returned by `end()`. Typical for
     * containers where end() returns an iterator of the same type as begin().
     *
     * @code
     * static_assert(IterRange<std::vector<int>>);  // true
     * static_assert(IterRange<std::list<int>>);    // true
     * // Some sentinel-based ranges may not satisfy this
     * @endcode
     */
    template<typename R>
    concept IterRange = Range<R> &&
    (requires(R& r)
    {
        { r.end() } -> SameType<decltype(std::declval<R&>().begin())>;
    } ||
    requires(R& r)
    {
        { end(r) } -> SameType<decltype(begin(std::declval<R&>()))>;
    });

    template<typename R>
    concept ForwardRange = IterRange<R> &&
    (requires(R& r)
    {
        { r.begin() } -> ForwardIterator;
    } ||
    requires(R& r)
    {
        { begin(r) } -> ForwardIterator;
    });

    template<typename R>
    concept BidirectionalRange = ForwardRange<R> &&
    (requires(R& r)
    {
        { r.begin() } -> BidirectionalIterator;
    } ||
    requires(R& r)
    {
        { begin(r) } -> BidirectionalIterator;
    });

    template<typename R>
    concept RandomAccessRange = BidirectionalRange<R> &&
    (requires(R& r)
    {
        { r.begin() } -> RandomAccessIterator;
    } ||
    requires(R& r)
    {
        { begin(r) } -> RandomAccessIterator;
    });

    template<typename R>
    concept ContiguousRange = RandomAccessRange<R> &&
    (requires(R& r)
    {
        { r.begin() } -> ContiguousIterator;
    } ||
    requires(R& r)
    {
        { begin(r) } -> ContiguousIterator;
    });

    /**
     * @brief Traits class providing uniform access to range properties.
     *
     * @tparam R The range type
     *
     * This class template extracts iterator types, value/reference types,
     * and provides static `begin` and `end` functions that work uniformly
     * across different range kinds (member or non-member begin/end).
     *
     * Specializations exist for const-qualified ranges and built-in arrays.
     */
    template<typename R>
    struct RangeTraits;

    /**
     * @brief Primary template specialization for mutable ranges with member begin/end.
     *
     * @tparam R The range type (must satisfy Range)
     */
    template<Range R>
    struct RangeTraits<R>
    {
        using BeginIterType  = decltype(std::declval<R&>().begin());
        using EndIterType    = decltype(std::declval<R&>().end());
        using ReferenceType  = IterTraits<BeginIterType>::ReferenceType;
        using ValueType      = IterTraits<BeginIterType>::ValueType;
        using PointerType    = IterTraits<BeginIterType>::PointerType;
        using DifferenceType = IterTraits<BeginIterType>::DifferenceType;

        /**
         * @brief Returns the begin iterator using the range's member function.
         */
        static BeginIterType begin(R& r)
        {
            return r.begin();
        }

        /**
         * @brief Returns the end sentinel using the range's member function.
         */
        static EndIterType end(R& r)
        {
            return r.end();
        }
    };

    /**
     * @brief Specialization for const-qualified ranges.
     *
     * Adjusts iterator types to const iterators.
     */
    template<Range R>
    struct RangeTraits<const R>
    {
        using BeginIterType  = decltype(std::declval<const R&>().begin());
        using EndIterType    = decltype(std::declval<const R&>().end());
        using ReferenceType  = IterTraits<BeginIterType>::ReferenceType;
        using ValueType      = IterTraits<BeginIterType>::ValueType;
        using PointerType    = IterTraits<BeginIterType>::PointerType;
        using DifferenceType = IterTraits<BeginIterType>::DifferenceType;

        static BeginIterType begin(const R& r)
        {
            return r.begin();
        }

        static EndIterType end(const R& r)
        {
            return r.end();
        }
    };

    /**
     * @brief Specialization for built-in mutable arrays.
     *
     * Uses raw pointers as iterators.
     */
    template<typename T, Size::Type Size>
    struct RangeTraits<T[Size]>
    {
        using BeginIterType  = T*;
        using EndIterType    = T*;
        using ReferenceType  = IterTraits<BeginIterType>::ReferenceType;
        using ValueType      = IterTraits<BeginIterType>::ValueType;
        using PointerType    = IterTraits<BeginIterType>::PointerType;
        using DifferenceType = IterTraits<BeginIterType>::DifferenceType;

        static BeginIterType begin(T (&arr) [Size])
        {
            return arr;
        }

        static EndIterType end(T (&arr) [Size])
        {
            return arr + Size;
        }
    };

    /**
     * @brief Specialization for built-in const arrays.
     *
     * Uses const pointers as iterators.
     */
    template<typename T, Size::Type Size>
    struct RangeTraits<const T[Size]>
    {
        using BeginIterType  = const T*;
        using EndIterType    = const T*;
        using ReferenceType  = IterTraits<BeginIterType>::ReferenceType;
        using ValueType      = IterTraits<BeginIterType>::ValueType;
        using PointerType    = IterTraits<BeginIterType>::PointerType;
        using DifferenceType = IterTraits<BeginIterType>::DifferenceType;


        static BeginIterType begin(const T (&arr) [Size])
        {
            return arr;
        }

        static EndIterType end(const T (&arr) [Size])
        {
            return arr + Size;
        }
    };

    template<typename T, Size::Type Size>
    struct RangeTraits<T (&)[Size]> : RangeTraits<T[Size]> {};

    template<typename T, Size::Type Size>
    struct RangeTraits<const T (&)[Size]> : RangeTraits<const T[Size]> {};
}

/** @} */ // end of RangeTraits group