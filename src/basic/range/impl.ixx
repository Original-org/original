module;
#include <utility>
#include <memory>
export module original.basic.range.impl;
import original.basic.range.traits;
import original.basic.iterator.traits;
import orginal.basic.iterator.impl;
import original.basic.types;
import original.basic.number;


namespace original::details
{
    template<ForwardIterator Iter>
    class TakeIterator
        : public ForwardIteratorBase<
            TakeIterator<Iter>,
            typename IterTraits<Iter>::ValueType,
            typename IterTraits<Iter>::ReferenceType,
            typename IterTraits<Iter>::PointerType
        >
    {
        Iter cur_;
        Size remains_{};

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
        : public ForwardIteratorBase<
            EnumIterator<Iter>,
            typename IterTraits<Iter>::ValueType,
            std::pair<const Size, typename IterTraits<Iter>::ReferenceType>,
            void
        >
    {
        Iter cur_;
        Size index_{};

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

    template<Range R>
    class RefRange {
        R* ptr_;
    public:
        explicit RefRange(R& r) noexcept : ptr_(std::addressof(r)) {}

        decltype(auto) begin() const
        {
            const auto& r = *this->ptr_;
            return r.begin();
        }

        decltype(auto) end() const
        {
            const auto& r = *this->ptr_;
            return r.end();
        }

        decltype(auto) begin()
        {
            auto& r = *this->ptr_;
            return r.begin();
        }

        decltype(auto) end()
        {
            auto& r = *this->ptr_;
            return r.end();
        }
    };

    template<Range R>
    auto all(R& r) noexcept {
        return RefRange<R>(r);
    }

    template<Range R>
    auto all(R&& r) noexcept
    {
        return std::forward<R>(r);
    }

    template<Range R>
    class IterRangeBase
    {
    protected:
        R base_;

        explicit IterRangeBase(R r) : base_(std::move(r)) {}

        decltype(auto) beginBase()
        {
            return this->base_.begin();
        }

        decltype(auto) endBase()
        {
            return this->base_.end();
        }

        decltype(auto) beginBase() const
        {
            return this->base_.begin();
        }

        decltype(auto) endBase() const
        {
            return this->base_.end();
        }
    };

    template<Range R>
    class TakeRange : public IterRangeBase<R> {
        using Base = IterRangeBase<R>;

        Size n_{};
    public:
        TakeRange(R base, const Size n)
            : Base(std::move(base)), n_(n) {}

        auto begin() {
            using Iter = decltype(this->beginBase());
            return TakeIterator<Iter>{this->beginBase(), this->n_};
        }

        auto end() {
            using Iter = decltype(this->endBase());
            return TakeIterator<Iter>{this->endBase(), Size{}};
        }

        auto begin() const {
            using Iter = decltype(this->beginBase());
            return TakeIterator<Iter>{this->beginBase(), this->n_};
        }

        auto end() const {
            using Iter = decltype(this->endBase());
            return TakeIterator<Iter>{this->endBase(), Size{}};
        }
    };

    template<Range R>
    class SkipRange : public IterRangeBase<R> {
        using Base = IterRangeBase<R>;
        Size n_{};

        template<ForwardIterator Iter>
        static Iter skip(Iter it, Iter end, Size n) noexcept {
            while (n > Size{} && it != end) {
                ++it;
                --n;
            }
            return it;
        }
    public:
        SkipRange(R base, const Size n) noexcept
            : Base(std::move(base)), n_(n) {}

        auto begin() {
            auto it  = this->beginBase();
            auto end = this->endBase();
            return skip(it, end, n_);
        }

        auto end() {
            return this->endBase();
        }

        auto begin() const {
            auto it  = this->beginBase();
            auto end = this->endBase();
            return skip(it, end, n_);
        }

        auto end() const {
            return this->endBase();
        }
    };

    template<Range R>
    class EnumRange : public IterRangeBase<R> {
        using Base = IterRangeBase<R>;
        Size start_{};
    public:
        EnumRange(R base, const Size start) noexcept
            : Base(std::move(base)), start_(start) {}

        auto begin() {
            using Iter = decltype(this->beginBase());
            return EnumIterator<Iter>{this->beginBase(), this->start_};
        }

        auto end() {
            using Iter = decltype(this->endBase());
            return EnumIterator<Iter>{this->endBase(), Size{}};
        }

        auto begin() const {
            using Iter = decltype(this->beginBase());
            return EnumIterator<Iter>{this->beginBase(), this->start_};
        }

        auto end() const {
            using Iter = decltype(this->endBase());
            return EnumIterator<Iter>{this->endBase(), Size{}};
        }
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
        return details::RangePipeline
        {
            [n]<Range R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::TakeRange<RangeType>{all, n};
            }
        };
    }

    auto skip(const Size n) noexcept
    {
        return details::RangePipeline
        {
            [n]<Range R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::SkipRange<RangeType>{all, n};
            }
        };
    }

    auto enumerate(const Size start = Size{}) noexcept
    {
        return details::RangePipeline
        {
            [start]<Range R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);
                return details::EnumRange<RangeType>{all, start};
            }
        };
    }
}
