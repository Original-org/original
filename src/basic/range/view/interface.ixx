module;
#include <type_traits>
#include <functional>
export module original.basic.range.view:interface;
import :rangeView;


export namespace original::range
{
    /**
     * @brief Creates a pipeline adaptor that takes the first N elements.
     *
     * @param n Number of elements to take
     * @return RangePipeline yielding a TakeRange view
     *
     */
    auto take(const Size n) noexcept
    {
        return details::RangePipeline
        {
            [n]<ForwardRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::TakeRange<RangeType>{all, n};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that skips the first N elements.
     */
    auto skip(const Size n) noexcept
    {
        return details::RangePipeline
        {
            [n]<ForwardRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::SkipRange<RangeType>{all, n};
            }
        };
    }


    auto reverse() noexcept
    {
        return details::RangePipeline
        {
            []<BidirectionalRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::ReversedRange<RangeType>{all};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that enumerates elements with indices.
     *
     * @param start Starting index (default 0)
     */
    auto enumerate(const Size start = Size{}) noexcept
    {
        return details::RangePipeline
        {
            [start]<ForwardRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::EnumRange<RangeType>{all, start};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that transforms elements.
     *
     * @tparam F Callable type
     * @param func Transformation function
     */
    template<Invokable F>
    auto transform(F&& func) noexcept
    {
        return details::RangePipeline
        {
            [func = std::forward<F>(func)]<ForwardRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);

                using Iter = decltype(all.begin());
                using Ref  = IterTraits<Iter>::ReferenceType;

                static_assert(
                    InvokableWith<std::decay_t<F>&, Ref>,
                    "transform(F): F must be invocable with range element"
                );

                static_assert(
                    Functor<std::decay_t<F>&>,
                    "transform(F): F must be a functor"
                );

                return details::TransformRange<RangeType, F>{all, func};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that filters elements by predicate.
     *
     * @tparam F Predicate type
     * @param func Predicate (should return convertible to bool)
     */
    template<Invokable F>
    auto filter(F&& func) noexcept
    {
        return details::RangePipeline
        {
            [func = std::forward<F>(func)]<ForwardRange R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);

                using Iter = decltype(all.begin());
                using Ref  = IterTraits<Iter>::ReferenceType;

                static_assert(
                    InvokableWith<std::decay_t<F>&, Ref>,
                    "filter(F): F must be invocable with range element"
                );

                static_assert(
                    Predicate<std::decay_t<F>&, Ref>,
                    "filter(F): F must be a predicate"
                );

                return details::FilterRange<RangeType, F>{all, func};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that excludes elements satisfying a predicate.
     *
     * @tparam F Predicate type
     * @param func Predicate to exclude
     */
    template<Invokable F>
    auto exclude(F&& func) noexcept
    {
        return filter
        (
            [func = std::forward<F>(func)]<typename E>(E&& x)
            requires Predicate<std::decay_t<F>&, E&&>
            {
                return !std::invoke(func, std::forward<E>(x));
            }
        );
    }

    /**
     * @brief Creates a pipeline adaptor that concatenates another range.
     *
     * @tparam Next Second range type
     * @param next Range to append
     */
    template<ForwardRange Next>
    auto concat(Next&& next) noexcept
    {
        return details::RangePipeline
        {
            [n = details::all(std::forward<Next>(next))]<ForwardRange Prev>(Prev&& prev)
            {
                auto p = details::all(std::forward<Prev>(prev));

                using IterPrev = decltype(p.begin());
                using IterNext = decltype(n.begin());

                using RefPrev = IterTraits<IterPrev>::ReferenceType;
                using RefNext = IterTraits<IterNext>::ReferenceType;
                using CommonRef = CommonRefType<RefPrev, RefNext>;

                static_assert(
                    Convertible<RefNext, CommonRef> && Convertible<RefPrev, CommonRef>,
                    "Reference type of next range must be convertible to prev range's"
                );

                return details::ConcatRange<decltype(p), decltype(n)>{p, n};
            }
        };
    }

    /**
     * @brief Creates a pipeline adaptor that zips with another range.
     *
     * @tparam RHS Right-hand range type
     * @param rhs Range to zip with
     */
    template<ForwardRange RHS>
    auto zip(RHS&& rhs) noexcept
    {
        return details::RangePipeline
        {
            [r = details::all(std::forward<RHS>(rhs))]<ForwardRange LHS>(LHS&& lhs)
            {
                auto l = details::all(std::forward<LHS>(lhs));
                return details::ZipRange<decltype(l), decltype(r)>{l, r};
            }
        };
    }
}