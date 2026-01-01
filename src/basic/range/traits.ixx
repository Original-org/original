module;
#include <type_traits>
export module original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


export namespace original
{
    template<typename T, Size::Type Size>
    T* begin(T (&arr) [Size])
    {
        return arr;
    }

    template<typename T, Size::Type Size>
    T* end(T (&arr) [Size])
    {
        return arr + Size;
    }

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
    struct RangeTraits;

    template<Range R>
    struct RangeTraits<R>
    {
        using BeginIterType = decltype(std::declval<R&>().begin());
        using EndIterType = decltype(std::declval<R&>().end());
        using ReferenceType = IterTraits<BeginIterType>::ReferenceType;
        using ValueType = IterTraits<BeginIterType>::ValueType;
        using PointerType = IterTraits<BeginIterType>::PointerType;
        using DifferenceType = IterTraits<BeginIterType>::DifferenceType;

        static BeginIterType begin(R& r)
        {
            return r.begin();
        }

        static EndIterType end(R& r)
        {
            return r.end();
        }
    };

    template<Range R>
    struct RangeTraits<const R>
    {
        using BeginIterType = decltype(std::declval<const R&>().begin());
        using EndIterType = decltype(std::declval<const R&>().end());
        using ReferenceType = IterTraits<BeginIterType>::ReferenceType;
        using ValueType = IterTraits<BeginIterType>::ValueType;
        using PointerType = IterTraits<BeginIterType>::PointerType;
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

    template<typename T, Size::Type Size>
    struct RangeTraits<T[Size]>
    {
        using BeginIterType = T*;
        using EndIterType   = T*;
        using ReferenceType = IterTraits<BeginIterType>::ReferenceType;
        using ValueType = IterTraits<BeginIterType>::ValueType;
        using PointerType = IterTraits<BeginIterType>::PointerType;
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

    template<typename T, Size::Type Size>
    struct RangeTraits<const T[Size]>
    {
        using BeginIterType = const T*;
        using EndIterType   = const T*;
        using ReferenceType = IterTraits<BeginIterType>::ReferenceType;
        using ValueType = IterTraits<BeginIterType>::ValueType;
        using PointerType = IterTraits<BeginIterType>::PointerType;
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
}