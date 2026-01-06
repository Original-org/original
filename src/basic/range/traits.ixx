module;
#include <type_traits>
export module original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.iterator.adaptor;
import original.basic.types;
import original.basic.number;

/**
 * @addtogroup RangeTraits
 * @{
 */

namespace original::details
{
    /**
     * @brief Internal traits to determine reverse iterator types.
     *
     * @tparam R The range type
     *
     * Primary template assumes no reverse iterator support.
     */
    template<typename R, typename = void>
    struct ReverseIteratorTraits
    {
        using RBeginIterType = void;
        using REndIterType   = void;
    };

    /**
     * @brief Helper to detect member rBegin.
     */
    template<typename R, typename = void>
    struct HasMemberRBegin : std::false_type {};

    template<typename R>
    struct HasMemberRBegin<R, std::void_t<decltype(std::declval<R&>().rBegin())>>
        : std::true_type {};

    /**
     * @brief Helper to detect non-member rBegin.
     */
    template<typename R, typename = void>
    struct HasNonMemberRBegin : std::false_type {};

    template<typename R>
    struct HasNonMemberRBegin<R, std::void_t<decltype(rBegin(std::declval<R&>()))>>
        : std::true_type {};

    /**
     * @brief Helper to detect member rEnd.
     */
    template<typename R, typename = void>
    struct HasMemberREnd : std::false_type {};

    template<typename R>
    struct HasMemberREnd<R, std::void_t<decltype(std::declval<R&>().rEnd())>>
        : std::true_type {};

    /**
     * @brief Helper to detect non-member rEnd.
     */
    template<typename R, typename = void>
    struct HasNonMemberREnd : std::false_type {};

    template<typename R>
    struct HasNonMemberREnd<R, std::void_t<decltype(rEnd(std::declval<R&>()))>>
        : std::true_type {};

    /**
     * @brief Specialization for ranges with member rBegin/rEnd.
     */
    template<typename R>
    struct ReverseIteratorTraits<R,
        std::enable_if_t<HasMemberRBegin<R>::value && HasMemberREnd<R>::value>>
    {
        using RBeginIterType = decltype(std::declval<R&>().rBegin());
        using REndIterType   = decltype(std::declval<R&>().rEnd());
    };

    /**
     * @brief Specialization for ranges with non-member rBegin/rEnd.
     */
    template<typename R>
    struct ReverseIteratorTraits<R,
        std::enable_if_t<HasNonMemberRBegin<R>::value && HasNonMemberREnd<R>::value &&
                        !HasMemberRBegin<R>::value>>
    {
        using RBeginIterType = decltype(rBegin(std::declval<R&>()));
        using REndIterType   = decltype(rEnd(std::declval<R&>()));
    };
}

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

    template<typename T, Size::Type Size>
    ReversedIterator<T*> rBegin(T (&arr) [Size])
    {
        return ReversedIterator<T*>{arr + Size - 1};
    }

    template<typename T, Size::Type Size>
    ReversedIterator<T*> rEnd(T (&arr) [Size])
    {
        return ReversedIterator<T*>{arr - 1};
    }

    template<typename T, Size::Type Size>
    ReversedIterator<const T*> rBegin(const T (&arr) [Size])
    {
        return ReversedIterator<const T*>{arr + Size - 1};
    }

    template<typename T, Size::Type Size>
    ReversedIterator<const T*> rEnd(const T (&arr) [Size])
    {
        return ReversedIterator<const T*>{arr - 1};
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
    requires(R& r)
    {
        { r.end() } -> SameType<decltype(std::declval<R&>().begin())>;
    } ||
    requires(R& r)
    {
        { end(r) } -> SameType<decltype(begin(std::declval<R&>()))>;
    };

    template<typename R>
    concept BidirectionalRange = IterRange<R> &&
    requires(R& r)
    {
        { r.rBegin() } -> Iterator;
        { r.rEnd() } -> SameType<decltype(std::declval<R&>().rBegin())>;
    } ||
    requires(R& r)
    {
        { rBegin(r) } -> Iterator;
        { rEnd(r) } -> SameType<decltype(rBegin(std::declval<R&>()))>;
    };

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
        using RBeginIterType = details::ReverseIteratorTraits<R>::RBeginIterType;
        using REndIterType   = details::ReverseIteratorTraits<R>::REndIterType;

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

        static RBeginIterType rBegin(R& r)
        requires BidirectionalRange<R>
        {
            return r.rBegin();
        }

        static REndIterType rEnd(R& r)
        requires BidirectionalRange<R>
        {
            return r.rEnd();
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
        using RBeginIterType = details::ReverseIteratorTraits<const R>::RBeginIterType;
        using REndIterType   = details::ReverseIteratorTraits<const R>::REndIterType;

        static BeginIterType begin(const R& r)
        {
            return r.begin();
        }

        static EndIterType end(const R& r)
        {
            return r.end();
        }

        static RBeginIterType rBegin(const R& r)
        requires BidirectionalRange<const R>
        {
            return r.rBegin();
        }

        static REndIterType rEnd(const R& r)
        requires BidirectionalRange<const R>
        {
            return r.rEnd();
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
        using RBeginIterType = ReversedIterator<BeginIterType>;
        using REndIterType   = ReversedIterator<EndIterType>;

        static BeginIterType begin(T (&arr) [Size])
        {
            return arr;
        }

        static EndIterType end(T (&arr) [Size])
        {
            return arr + Size;
        }

        static RBeginIterType rBegin(T (&arr) [Size])
        {
            return RBeginIterType{arr + Size - 1};
        }

        static REndIterType rEnd(T (&arr) [Size])
        {
            return RBeginIterType{arr - 1};
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
        using RBeginIterType = ReversedIterator<BeginIterType>;
        using REndIterType   = ReversedIterator<EndIterType>;


        static BeginIterType begin(const T (&arr) [Size])
        {
            return arr;
        }

        static EndIterType end(const T (&arr) [Size])
        {
            return arr + Size;
        }

        static RBeginIterType rBegin(const T (&arr) [Size])
        {
            return RBeginIterType{arr + Size - 1};
        }

        static REndIterType rEnd(const T (&arr) [Size])
        {
            return RBeginIterType{arr - 1};
        }
    };

    template<typename T, Size::Type Size>
    struct RangeTraits<T (&)[Size]> : RangeTraits<T[Size]> {};

    template<typename T, Size::Type Size>
    struct RangeTraits<const T (&)[Size]> : RangeTraits<const T[Size]> {};
}

/** @} */ // end of RangeTraits group