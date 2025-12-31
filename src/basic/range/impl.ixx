module;
#include <functional>
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
            RemoveCVRefType<typename IterTraits<Iter>::ReferenceType>,
            typename IterTraits<Iter>::ReferenceType,
            void
        >
    {
        Iter cur_;
        Size remains_{};

    public:
        using IterType = IterTraits<Iter>::IterType;
        using ReferenceType = IterTraits<Iter>::ReferenceType;
        using ValueType = RemoveCVRefType<ReferenceType>;
        using PointerType = void;
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
            RemoveCVRefType<std::pair<const Size, typename IterTraits<Iter>::ReferenceType>>,
            std::pair<const Size, typename IterTraits<Iter>::ReferenceType>,
            void
        >
    {
        Iter cur_;
        Size index_{};

    public:
        using IterType = IterTraits<Iter>::IterType;
        using ReferenceType = std::pair<const Size, typename IterTraits<Iter>::ReferenceType>;
        using ValueType = RemoveCVRefType<ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        EnumIterator() noexcept = default;

        EnumIterator(IterType cur, const Size start) noexcept
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

    template<ForwardIterator Iter, Invokable F>
    class TransformIterator
        : public ForwardIteratorBase<
            TransformIterator<Iter, F>,
            RemoveCVRefType<InvokeResultType<F&, typename IterTraits<Iter>::ReferenceType>>,
            InvokeResultType<F, typename IterTraits<Iter>::ReferenceType>,
            void
        >
    {
        Iter cur_;
        F* func_;

    public:
        using IterType = IterTraits<Iter>::IterType;
        using ReferenceType = InvokeResultType<F, typename IterTraits<Iter>::ReferenceType>;
        using ValueType = RemoveCVRefType<ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        TransformIterator() noexcept = default;

        TransformIterator(IterType cur, F* func) noexcept
            : cur_(cur), func_(func) {}

        ReferenceType operator*() const
        {
            return std::invoke(*this->func_, *this->cur_);
        }

        TransformIterator& operator++()
        {
            ++this->cur_;
            return *this;
        }

        TransformIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const TransformIterator& rhs) const noexcept
        {
            return this->cur_ == rhs.cur_;
        }
    };

    template<ForwardIterator Iter, Invokable F>
    class FilterIterator
        : public ForwardIteratorBase<
            FilterIterator<Iter, F>,
            RemoveCVRefType<typename IterTraits<Iter>::ReferenceType>,
            typename IterTraits<Iter>::ReferenceType,
            void
        >
    {
        Iter cur_;
        Iter end_;
        F* func_;

        void skipUntilSatisfy()
        {
            while (this->cur_ != this->end_ && !std::invoke(*this->func_, *this->cur_))
                ++this->cur_;
        }
    public:
        using IterType = IterTraits<Iter>::IterType;
        using ReferenceType = IterTraits<Iter>::ReferenceType;
        using ValueType = RemoveCVRefType<ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        FilterIterator() noexcept = default;

        FilterIterator(IterType cur, IterType end, F* func) noexcept
            : cur_(cur), end_(end), func_(func)
        {
            this->skipUntilSatisfy();
        }

        ReferenceType operator*() const
        {
            return *this->cur_;
        }

        FilterIterator& operator++()
        {
            ++this->cur_;
            this->skipUntilSatisfy();
            return *this;
        }

        FilterIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const FilterIterator& rhs) const noexcept
        {
            return this->cur_ == rhs.cur_;
        }
    };

    template<ForwardIterator Prev, ForwardIterator Next>
    class ConcatIterator
        : public ForwardIteratorBase<
            FilterIterator<Prev, Next>,
            RemoveCVRefType<
                CommonRefType<
                    typename IterTraits<Prev>::ReferenceType,
                    typename IterTraits<Next>::ReferenceType
                >
            >,
            CommonRefType<
                typename IterTraits<Prev>::ReferenceType,
                typename IterTraits<Next>::ReferenceType
            >,
            void
        >
    {
        Prev cur_prev_;
        Prev end_prev_;
        Next cur_next_;
        Next end_next_;
        bool at_next_{};

    public:
        using IterType = ConcatIterator;
        using ReferenceType
            = CommonRefType<
                typename IterTraits<Prev>::ReferenceType,
                typename IterTraits<Next>::ReferenceType
            >;
        using ValueType = RemoveCVRefType<ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<Prev>::DifferenceType;

        ConcatIterator() noexcept = default;

        ConcatIterator(Prev cur_prev, Prev end_prev, Next cur_next, Next end_next) noexcept
            : cur_prev_(cur_prev), end_prev_(end_prev),
              cur_next_(cur_next), end_next_(end_next), at_next_(cur_prev == end_prev) {}

        ReferenceType operator*() const
        {
            return this->at_next_ ?
                static_cast<ReferenceType>(*this->cur_next_) :
                static_cast<ReferenceType>(*this->cur_prev_);
        }

        ConcatIterator& operator++()
        {
            if (this->at_next_)
            {
                ++this->cur_next_;
            } else
            {
                ++this->cur_prev_;
                if (this->cur_prev_ == this->end_prev_)
                    this->at_next_ = true;
            }
            return *this;
        }

        ConcatIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const ConcatIterator& rhs) const noexcept
        {
            if (this->at_next_ != rhs.at_next_)
                return false;

            return this->at_next_ ?
                this->cur_next_ == rhs.cur_next_ : this->cur_prev_ == rhs.cur_prev_;
        }
    };

    template<ForwardIterator LHS, ForwardIterator RHS>
    class ZipIterator
        : public ForwardIteratorBase<
            FilterIterator<LHS, RHS>,
            std::pair<typename IterTraits<LHS>::ValueType, typename IterTraits<RHS>::ValueType>,
            std::pair<typename IterTraits<LHS>::ReferenceType, typename IterTraits<RHS>::ReferenceType>,
            void
        >
    {
        LHS cur_lhs_;
        LHS end_lhs_;
        RHS cur_rhs_;
        RHS end_rhs_;

    public:
        using IterType = ZipIterator;
        using ValueType = std::pair<typename IterTraits<LHS>::ValueType, typename IterTraits<RHS>::ValueType>;
        using ReferenceType = std::pair<typename IterTraits<LHS>::ReferenceType, typename IterTraits<RHS>::ReferenceType>;
        using PointerType = void;
        using DifferenceType = IterTraits<LHS>::DifferenceType;

        ZipIterator() noexcept = default;

        ZipIterator(LHS cur_lhs, LHS end_lhs, RHS cur_rhs, RHS end_rhs) noexcept
            : cur_lhs_(cur_lhs), end_lhs_(end_lhs), cur_rhs_(cur_rhs), end_rhs_(end_rhs) {}

        ReferenceType operator*() const
        {
            return ReferenceType{*this->cur_lhs_, *this->cur_rhs_};
        }

        ZipIterator& operator++()
        {
            ++this->cur_lhs_;
            ++this->cur_rhs_;
            return *this;
        }

        ZipIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const ZipIterator& rhs) const noexcept
        {
            return this->cur_lhs_ == rhs.cur_lhs_ || this->cur_rhs_ == rhs.cur_rhs_;
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
    class RangeViewBase
    {
    protected:
        R base_;

        explicit RangeViewBase(R r) : base_(std::move(r)) {}

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
    class TakeRange : public RangeViewBase<R> {
        using Base = RangeViewBase<R>;

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
    class SkipRange : public RangeViewBase<R> {
        using Base = RangeViewBase<R>;
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
    class EnumRange : public RangeViewBase<R> {
        using Base = RangeViewBase<R>;
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

    template<Range R, Invokable F>
    class TransformRange : public RangeViewBase<R>
    {
        using Base = RangeViewBase<R>;
        using Func = std::decay_t<F>;

        Func func_;
    public:
        TransformRange(R base, F func) noexcept
            : Base(std::move(base)), func_(std::move(func)) {}

        auto begin()
        {
            using Iter = decltype(this->beginBase());
            return TransformIterator<Iter, Func>{this->beginBase(), &this->func_};
        }

        auto end()
        {
            using Iter = decltype(this->endBase());
            return TransformIterator<Iter, Func>{this->endBase(), &this->func_};
        }

        auto begin() const
        {
            using Iter = decltype(this->beginBase());
            return TransformIterator<Iter, const Func>{this->beginBase(), &this->func_};
        }

        auto end() const
        {
            using Iter = decltype(this->endBase());
            return TransformIterator<Iter, const Func>{this->endBase(), &this->func_};
        }
    };

    template<Range R, Invokable F>
    class FilterRange : public RangeViewBase<R>
    {
        using Base = RangeViewBase<R>;
        using Func = std::decay_t<F>;

        Func func_;
    public:
        FilterRange(R base, F func) noexcept
            : Base(std::move(base)), func_(std::move(func)) {}

        auto begin()
        {
            using Iter = decltype(this->beginBase());
            return FilterIterator<Iter, Func>{this->beginBase(), this->endBase(), &this->func_};
        }

        auto end()
        {
            using Iter = decltype(this->endBase());
            return FilterIterator<Iter, Func>{this->endBase(), this->endBase(), &this->func_};
        }

        auto begin() const
        {
            using Iter = decltype(this->beginBase());
            return FilterIterator<Iter, const Func>{this->beginBase(), this->endBase(), &this->func_};
        }

        auto end() const
        {
            using Iter = decltype(this->endBase());
            return FilterIterator<Iter, const Func>{this->endBase(), this->endBase(), &this->func_};
        }
    };

    template<Range Prev, Range Next>
    class ConcatRange : public RangeViewBase<Prev>
    {
        using Base = RangeViewBase<Prev>;
        Next next_;

    public:
        ConcatRange(Prev prev, Next next) noexcept
            : Base(std::move(prev)), next_(std::move(next)) {}

        auto begin()
        {
            using ItPrev = decltype(this->beginBase());
            using ItNext = decltype(this->next_.begin());

            return ConcatIterator<ItPrev, ItNext>
            {
                this->beginBase(),
                this->endBase(),
                this->next_.begin(),
                this->next_.end()
            };
        }

        auto end()
        {
            using ItPrev = decltype(this->endBase());
            using ItNext = decltype(this->next_.end());

            return ConcatIterator<ItPrev, ItNext>
            {
                this->endBase(),
                this->endBase(),
                this->next_.end(),
                this->next_.end()
            };
        }

        auto begin() const
        {
            using ItPrev = decltype(this->beginBase());
            using ItNext = decltype(this->next_.begin());

            return ConcatIterator<ItPrev, ItNext>
            {
                this->beginBase(),
                this->endBase(),
                this->next_.begin(),
                this->next_.end()
            };
        }

        auto end() const
        {
            using ItPrev = decltype(this->endBase());
            using ItNext = decltype(this->next_.end());

            return ConcatIterator<ItPrev, ItNext>
            {
                this->endBase(),
                this->endBase(),
                this->next_.end(),
                this->next_.end()
            };
        }
    };

    template<Range LHS, Range RHS>
    class ZipRange
    {
        LHS lhs_;
        RHS rhs_;

    public:
        ZipRange(LHS lhs, RHS rhs) noexcept
            : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

        auto begin()
        {
            using ItLHS = decltype(this->lhs_.begin());
            using ItRHS = decltype(this->rhs_.begin());

            return ZipIterator<ItLHS, ItRHS>
            {
                this->lhs_.begin(),
                this->lhs_.end(),
                this->rhs_.begin(),
                this->rhs_.end()
            };
        }

        auto end()
        {
            using ItLHS = decltype(this->lhs_.end());
            using ItRHS = decltype(this->rhs_.end());

            return ZipIterator<ItLHS, ItRHS>
            {
                this->lhs_.end(),
                this->lhs_.end(),
                this->rhs_.end(),
                this->rhs_.end()
            };
        }

        auto begin() const
        {
            using ItLHS = decltype(this->lhs_.begin());
            using ItRHS = decltype(this->rhs_.begin());

            return ZipIterator<ItLHS, ItRHS>
            {
                this->lhs_.begin(),
                this->lhs_.end(),
                this->rhs_.begin(),
                this->rhs_.end()
            };
        }

        auto end() const
        {
            using ItLHS = decltype(this->lhs_.end());
            using ItRHS = decltype(this->rhs_.end());

            return ZipIterator<ItLHS, ItRHS>
            {
                this->lhs_.end(),
                this->lhs_.end(),
                this->rhs_.end(),
                this->rhs_.end()
            };
        }
    };

    template<Invokable F>
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

    template<Invokable F>
    auto transform(F&& func) noexcept
    {
        return details::RangePipeline
        {
            [func = std::forward<F>(func)]<Range R>(R&& r)
            {
                auto all = details::all(std::forward<R>(r));
                using RangeType = decltype(all);

                using Iter = decltype(all.begin());
                using Ref  = IterTraits<Iter>::ReferenceType;

                static_assert(
                    InvokableWith<std::decay_t<F>&, Ref>,
                    "transform(F): F must be invocable with range element"
                );

                return details::TransformRange<RangeType, F>{all, func};
            }
        };
    }

    template<Invokable F>
    auto filter(F&& func) noexcept
    {
        return details::RangePipeline
        {
            [func = std::forward<F>(func)]<Range R>(R&& r)
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
                    InvokableReturnsConvertible<std::decay_t<F>&, bool, Ref>,
                    "transform(F): Invoke result of F must be convertible to bool"
                );

                return details::FilterRange<RangeType, F>{all, func};
            }
        };
    }

    template<Invokable F>
    auto exclude(F&& func) noexcept
    {
        return filter
        (
            [func = std::forward<F>(func)]<typename E>(E&& x)
            requires InvokableReturnsConvertible<std::decay_t<F>&, bool, E&&>
            {
                return !std::invoke(func, std::forward<E>(x));
            }
        );
    }

    template<Range Next>
    auto concat(Next&& next) noexcept
    {
        return details::RangePipeline
        {
            [n = details::all(std::forward<Next>(next))]<Range Prev>(Prev&& prev)
            {
                auto p = details::all(std::forward<Prev>(prev));

                using IterPrev = decltype(p.begin());
                using IterNext = decltype(n.begin());

                static_assert(ForwardIterator<IterPrev>, "Iter type of the prev range must be ForwardIterator");
                static_assert(ForwardIterator<IterNext>, "Iter type of the next range must be ForwardIterator");

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


    template<Range RHS>
    auto zip(RHS&& rhs) noexcept
    {
        return details::RangePipeline
        {
            [r = details::all(std::forward<RHS>(rhs))]<Range LHS>(LHS&& lhs)
            {
                using IterLHS = RangeTraits<LHS>::BeginIterType;
                using IterRHS = RangeTraits<RHS>::BeginIterType;

                static_assert(ForwardIterator<IterLHS>, "Iter type of left hands range must be ForwardIterator");
                static_assert(ForwardIterator<IterRHS>, "Iter type of right hands range must be ForwardIterator");

                auto l = details::all(std::forward<LHS>(lhs));
                return details::ZipRange<decltype(l), decltype(r)>{l, r};
            }
        };
    }
}
