module;
#include <utility>
export module original.basic.range.impl;
import original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


namespace original::details
{
    template<ForwardIterator Iter>
    class TakeIterator
    {
        Iter cur_;
        Size remains_;

    public:
        using IterType = IterTraits<Iter>::IterType;
        using ValueType = IterTraits<Iter>::ValueType;
        using ReferenceType = IterTraits<Iter>::ReferenceType;
        using PointerType = IterTraits<Iter>::PointerType;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        TakeIterator() noexcept = default;

        TakeIterator(IterType cur, const Size remains) noexcept
            : cur_(cur), remains_(remains) {}

        ReferenceType operator*() const
        {
            return *this->cur_;
        }

        TakeIterator& operator++()
        {
            ++this->cur_;
            --this->remains_;
            return *this;
        }

        TakeIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const TakeIterator& rhs) const noexcept
        {
            if (!this->remains_ && !rhs.remains_)
                return true;

            return this->cur_ == rhs.cur_;
        }
    };

    template<ForwardIterator Iter>
    class EnumIterator
    {
        Iter cur_;
        Size index_;

    public:
        using IterType = IterTraits<Iter>::IterType;
        using ValueType = IterTraits<Iter>::ValueType;
        using ReferenceType = std::pair<const Size, typename IterTraits<Iter>::ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        EnumIterator() noexcept = default;

        explicit EnumIterator(IterType cur, const Size start) noexcept
            : cur_(cur), index_(start) {}

        ReferenceType operator*() const
        {
            return ReferenceType{this->index_, *this->cur_};
        }

        EnumIterator& operator++()
        {
            ++this->cur_;
            ++this->index_;
            return *this;
        }

        EnumIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const EnumIterator& rhs) const noexcept
        {
            return this->cur_ == rhs.cur_;
        }
    };

    template<Iterator Iter>
    class IterRangeBase
    {
    protected:
        using BeginIterType = Iter;
        using EndIterType = Iter;

        BeginIterType begin_;
        EndIterType end_;

        IterRangeBase(BeginIterType begin, EndIterType end) noexcept
            : begin_(begin), end_(end) {}

    public:
        [[nodiscard]] BeginIterType begin() const noexcept
        {
            return this->begin_;
        }

        [[nodiscard]] EndIterType end() const noexcept
        {
            return this->end_;
        }
    };

    template<ForwardIterator Iter>
    class TakeRange : public IterRangeBase<TakeIterator<Iter>>
    {
        using Base = IterRangeBase<TakeIterator<Iter>>;
    public:
        using BeginIterType = TakeIterator<Iter>;
        using EndIterType = TakeIterator<Iter>;

        TakeRange(Iter begin, Iter end, const Size remains)
            : Base(BeginIterType{begin, remains}, EndIterType{end, Size{}}) {}
    };

    template<ForwardIterator Iter>
    class SkipRange : public IterRangeBase<Iter>
    {
        static Iter skip(Iter cur, Iter end, Size n) noexcept
        {
            while (n > Size{} && cur != end)
            {
                ++cur;
                --n;
            }
            return cur;
        }

        using Base = IterRangeBase<Iter>;
    public:
        using BeginIterType = Iter;
        using EndIterType = Iter;

        SkipRange(Iter begin, Iter end, const Size skips) noexcept
            : Base(skip(begin, end, skips), end) {}
    };

    template<ForwardIterator Iter>
    class EnumRange : public IterRangeBase<EnumIterator<Iter>>
    {
        using Base = IterRangeBase<EnumIterator<Iter>>;
    public:
        using BeginIterType = EnumIterator<Iter>;
        using EndIterType = EnumIterator<Iter>;

        EnumRange(Iter begin, Iter end, const Size start) noexcept
            : Base(BeginIterType{begin, start}, EndIterType{end, Size{}}) {}
    };

    template<StdInvokable F>
    class RangePipeline
    {
        using FuncType = std::decay_t<F>;

        FuncType func_;
    public:
        explicit RangePipeline(F&& func) noexcept
            : func_(std::forward<F>(func)) {}

        template<Range R>
        friend auto operator|(R&& range, RangePipeline pipeline)
        {
            return pipeline.func_(std::forward<R>(range));
        }
    };
}

export namespace original::range
{
    auto take(const Size n) noexcept
    {
        return details::RangePipeline{
            [n]<Range R>(R&& r)
            {
                using Iter = RangeTraits<R>::BeginIterType;
                using TakeRange = details::TakeRange<Iter>;
                return TakeRange{r.begin(), r.end(), n};
            }
        };
    }

    auto skip(const Size n) noexcept
    {
        return details::RangePipeline{
            [n]<Range R>(R&& r)
            {
                using Iter = RangeTraits<R>::BeginIterType;
                using SkipRange = details::SkipRange<Iter>;
                return SkipRange{r.begin(), r.end(), n};
            }
        };
    }

    auto enumerate(const Size start = Size{}) noexcept
    {
        return details::RangePipeline{
            [start]<Range R>(R&& r)
            {
                using Iter = RangeTraits<R>::BeginIterType;
                using EnumRange = details::EnumRange<Iter>;
                return EnumRange{r.begin(), r.end(), start};
            }
        };
    }
}
