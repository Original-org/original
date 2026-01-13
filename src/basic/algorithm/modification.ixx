module;
#include <utility>
export module original.basic.algorithm.modification;
import original.basic.types;
import original.basic.iterator;
import original.basic.number;
import original.basic.range;


namespace original::details
{
    using namespace original::literals;
    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveImpl(Iter1 src_first, Iter1 src_last, Iter2 dst_first, Size count)
    {
        while (src_first != src_last && count > 0_size)
        {
            *dst_first = std::move(*src_first);
            ++src_first;
            ++dst_first;
            --count;
        }
        return dst_first;
    }

    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveImpl(Iter1 src_first, Iter1 src_last, Iter2 dst_first, Iter2 dst_last, Size count)
    {
        while (src_first != src_last && dst_first != dst_last && count > 0_size)
        {
            *dst_first = std::move(*src_first);
            ++src_first;
            ++dst_first;
            --count;
        }
        return dst_first;
    }

    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveBackwardsImpl(Iter1 src_first, Iter1 src_last, Iter2 dst_last, Size count)
    {
        while (src_first != src_last && count > 0_size)
        {
            --src_last;
            --dst_last;
            *dst_last = std::move(*src_last);
            --count;
        }
        return dst_last;
    }

    using namespace original::literals;
    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveBackwardsImpl(Iter1 src_first, Iter1 src_last, Iter2 dst_first, Iter2 dst_last, Size count)
    {
        while (src_first != src_last && dst_first != dst_last && count > 0_size)
        {
            --src_last;
            --dst_last;
            *dst_last = std::move(*src_last);
            --count;
        }
        return dst_last;
    }
}

export namespace original::algorithms
{
    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 move(Iter1 src_first, Iter1 src_last,
                         Iter2 dst_first, const Size count = maximum<Size>())
    {
        return details::moveImpl(src_first, src_last, dst_first, count);
    }

    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                        RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 move(Iter1 src_first, Iter1 src_last,
                         Iter2 dst_first, Iter2 dst_last, const Size count = maximum<Size>())
    {
        return details::moveImpl(src_first, src_last, dst_first, dst_last, count);
    }

    template <Range R1, Range R2>
    requires AssignableFrom<typename IterTraits<R2>::ReferenceType,
                            RemoveReference<typename RangeTraits<R1>::ReferenceType>&&>
    constexpr auto move(R1& src_range, R2& dst_range, const Size count = maximum<Size>())
    {
        return move(RangeTraits<R1>::begin(src_range),
                             RangeTraits<R1>::end(src_range),
                             RangeTraits<R2>::begin(dst_range),
                             RangeTraits<R2>::end(),
                             count);
    }

    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveBackwards(Iter1 src_first, Iter1 src_last,
                                  Iter2 dst_last, const Size count = maximum<Size>())
    {
        return details::moveBackwardsImpl(src_first, src_last, dst_last, count);
    }

    template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
    requires AssignableFrom<typename IterTraits<Iter2>::ReferenceType,
                            RemoveReference<typename IterTraits<Iter1>::ReferenceType>&&>
    constexpr Iter2 moveBackwards(Iter1 src_first, Iter1 src_last,
                                  Iter2 dst_first, Iter2 dst_last, const Size count = maximum<Size>())
    {
        return details::moveBackwardsImpl(src_first, src_last, dst_first, dst_last, count);
    }

    template <Range R1, Range R2>
    requires AssignableFrom<typename IterTraits<R2>::ReferenceType,
                            RemoveReference<typename RangeTraits<R1>::ReferenceType>&&>
    constexpr auto moveBackwards(R1& src_range, R2& dst_range, const Size count = maximum<Size>())
    {
        return moveBackwards(RangeTraits<R1>::begin(src_range),
                             RangeTraits<R1>::end(src_range),
                             RangeTraits<R2>::begin(dst_range),
                             RangeTraits<R2>::end(),
                             count);
    }
}