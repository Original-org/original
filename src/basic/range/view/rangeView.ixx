module;
#include <memory>
export module original.basic.range.view:rangeView;
import :iterators;
import original.basic.range.traits;


namespace original::details
{
        /**
     * @brief Lightweight range reference wrapper.
     *
     * @tparam R Range type
     *
     * Enables pipe syntax on lvalue ranges without moving the underlying range.
     */
    template<Range R>
    class RefRange {
        R* ptr_;
    public:
        explicit RefRange(R& r) noexcept : ptr_(std::addressof(r)) {}

        decltype(auto) begin() const
        {
            const auto& r = *this->ptr_;
            return RangeTraits<decltype(r)>::begin(r);
        }

        decltype(auto) end() const
        {
            const auto& r = *this->ptr_;
            return RangeTraits<decltype(r)>::end(r);
        }

        decltype(auto) begin()
        {
            auto& r = *this->ptr_;
            return RangeTraits<decltype(r)>::begin(r);
        }

        decltype(auto) end()
        {
            auto& r = *this->ptr_;
            return RangeTraits<decltype(r)>::end(r);
        }
    };

    /**
     * @brief Creates a range view over an lvalue range; forwards rvalues unchanged.
     *
     * @tparam R Range type
     * @param r Range object
     * @return RefRange<R> for lvalues, R&& for rvalues
     */
    template<Range R>
    auto all(R& r) noexcept {
        return RefRange<R>(r);
    }

    template<Range R>
    auto all(R&& r) noexcept
    {
        return std::forward<R>(r);
    }

    /**
     * @brief Base class for owning range views.
     *
     * @tparam R Underlying range type
     *
     * Stores the base range by value and provides access to its begin/end.
     */
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

    // Individual view classes (TakeRange, SkipRange, EnumRange, TransformRange,
    // FilterRange, ConcatRange, ZipRange) are documented inline below.

    /**
     * @brief View that yields the first N elements of a range.
     */
    template<ForwardRange R>
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

    /**
     * @brief View that skips the first N elements of a range.
     */
    template<ForwardRange R>
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

    template<BidirectionalRange R>
    class ReversedRange : public RangeViewBase<R>
    {
        using Base = RangeViewBase<R>;
    public:
        explicit ReversedRange(R base) noexcept
            : Base(std::move(base)) {}

        auto begin()
        {
            using Iter = decltype(this->endBase());
            return ReversedIterator<Iter>{this->endBase()};
        }

        auto end()
        {
            using Iter = decltype(this->beginBase());
            return ReversedIterator<Iter>{this->beginBase()};
        }

        auto begin() const
        {
            using Iter = decltype(this->endBase());
            return ReversedIterator<Iter>{this->endBase()};
        }

        auto end() const
        {
            using Iter = decltype(this->beginBase());
            return ReversedIterator<Iter>{this->beginBase()};
        }
    };

    /**
     * @brief View that enumerates elements with an index.
     */
    template<ForwardRange R>
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

    /**
     * @brief View that applies a transformation to each element.
     */
    template<ForwardRange R, Invokable F>
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

    /**
     * @brief View that yields only elements satisfying a predicate.
     */
    template<ForwardRange R, Invokable F>
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

    /**
     * @brief View that concatenates two ranges.
     */
    template<ForwardRange Prev, ForwardRange Next>
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

    /**
     * @brief View that zips two ranges into pairs.
     */
    template<ForwardRange LHS, ForwardRange RHS>
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

    /**
     * @brief Internal type for building pipeline operators.
     */
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