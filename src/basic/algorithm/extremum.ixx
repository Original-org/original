module;
#include <utility>
export module original.basic.algorithm.extremum;
import original.basic.algorithm.functor;
import original.basic.types;
import original.basic.iterator.traits;
import original.basic.range.traits;


export namespace original::algorithms
{
    template<typename T, Invokable Pred>
    requires BinaryPredicate<Pred, T, T>
    constexpr T extremum(const T& lhs, const T& rhs, Pred pred)
    {
        return pred(lhs, rhs) ? lhs : rhs;
    }

    template<
        Invokable Pred,
        typename T,
        typename... Ts
    >
    requires
        (sizeof...(Ts) > 0) &&
        BinaryPredicate<Pred, T, T> &&
        (SameType<T, Ts> && ...)
    constexpr T extremum(Pred pred, const T& first, const T& second, const Ts&... rest)
    {
        T result = extremum(first, second, pred);
        ((result = extremum(result, rest, pred)), ...);
        return result;
    }

    template<Iterator Iter, Invokable Pred>
    requires
    BinaryPredicate<
        Pred,
        typename IterTraits<Iter>::ReferenceType,
        typename IterTraits<Iter>::ReferenceType
    >
    constexpr Iter extremeElement(Iter first, Iter last, Pred pred)
    {
        auto result = first;
        for (auto it = first; it != last; ++it)
        {
            if (pred(*it, *result))
            {
                result = it;
            }
        }
        return result;
    }

    template<Range R, Invokable Pred>
    requires
    BinaryPredicate<
        Pred,
        typename RangeTraits<R>::ReferenceType,
        typename RangeTraits<R>::ReferenceType
    >
    constexpr auto extremeElement(R&& range, Pred pred)
    {
        return extremeElement(
            RangeTraits<R>::begin(std::forward<R>(range)),
            RangeTraits<R>::end(std::forward<R>(range)),
            pred
        );
    }

    template<typename T, Invokable Pred = LessThan>
    requires BinaryPredicate<Pred, T, T>
    constexpr T minimum(const T& lhs, const T& rhs, Pred pred = {})
    {
        return extremum(lhs, rhs, pred);
    }

    template<typename T, typename... Ts>
    requires
        (sizeof...(Ts) > 0) &&
        (SameType<T, Ts> && ...)
    constexpr T minimum(const T& first, const T& second, const Ts&... rest)
    {
        return extremum(LessThan{}, first, second, rest...);
    }

    template<
        Invokable Pred,
        typename T,
        typename... Ts
    >
    requires
        (sizeof...(Ts) > 0) &&
        BinaryPredicate<Pred, T, T> &&
        (SameType<T, Ts> && ...)
    constexpr T minimum(Pred pred, const T& first, const T& second, const Ts&... rest)
    {
        return extremum(pred, first, second, rest...);
    }

    template<Iterator Iter, Invokable Pred = LessThan>
    requires
    BinaryPredicate<
        Pred,
        typename IterTraits<Iter>::ReferenceType,
        typename IterTraits<Iter>::ReferenceType
    >
    constexpr Iter minElement(Iter first, Iter last, Pred pred = {})
    {
        return extremeElement(first, last, pred);
    }

    template<Range R, Invokable Pred = LessThan>
    requires
    BinaryPredicate<
        Pred,
        typename RangeTraits<R>::ReferenceType,
        typename RangeTraits<R>::ReferenceType
    >
    constexpr auto minElement(R&& range, Pred pred = {})
    {
        return extremeElement(std::forward<R>(range), pred);
    }

    template<typename T, Invokable Pred = GreaterThan>
    requires BinaryPredicate<Pred, T, T>
    constexpr T maximum(const T& lhs, const T& rhs, Pred pred = {})
    {
        return extremum(lhs, rhs, pred);
    }

    template<typename T, typename... Ts>
    requires
        (sizeof...(Ts) > 0) &&
        (SameType<T, Ts> && ...)
    constexpr T maximum(const T& first, const T& second, const Ts&... rest)
    {
        return extremum(GreaterThan{}, first, second, rest...);
    }

    template<
        Invokable Pred,
        typename T,
        typename... Ts
    >
    requires
        (sizeof...(Ts) > 0) &&
        BinaryPredicate<Pred, T, T> &&
        (SameType<T, Ts> && ...)
    constexpr T maximum(Pred pred, const T& first, const T& second, const Ts&... rest)
    {
        return extremum(pred, first, second, rest...);
    }

    template<Iterator Iter, Invokable Pred = GreaterThan>
    requires
    BinaryPredicate<
        Pred,
        typename IterTraits<Iter>::ReferenceType,
        typename IterTraits<Iter>::ReferenceType
    >
    constexpr Iter maxElement(Iter first, Iter last, Pred pred = {})
    {
        return extremeElement(first, last, pred);
    }

    template<Range R, Invokable Pred = GreaterThan>
    requires
    BinaryPredicate<
        Pred,
        typename RangeTraits<R>::ReferenceType,
        typename RangeTraits<R>::ReferenceType
    >
    constexpr auto maxElement(R&& range, Pred pred = {})
    {
        return extremeElement(std::forward<R>(range), pred);
    }
}