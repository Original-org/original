module;
#include <functional>
export module original.basic.range.view:iterators;
import original.basic.types;
import original.basic.number;
import original.basic.iterator.traits;
import original.basic.iterator.impl;
import original.basic.structural.couple;


namespace original::details
{
        /**
     * @brief Iterator that limits traversal to the first N elements of an underlying range.
     *
     * @tparam Iter Underlying forward iterator type
     *
     * Produces a forward iterator that stops after `remains_` elements have been visited.
     * Equality comparison treats exhausted iterators (remains_ == 0) as equal.
     */
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
        using IterType       = IterTraits<Iter>::IterType;
        using ReferenceType  = IterTraits<Iter>::ReferenceType;
        using ValueType      = RemoveCVRefType<ReferenceType>;
        using PointerType    = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        TakeIterator() noexcept = default;

        /**
         * @brief Constructs the iterator.
         *
         * @param cur Current position in the underlying range
         * @param remains Number of elements remaining to yield
         */
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

    template<BidirectionalIterator Iter>
    class ReversedIterator : public BidirectionalIteratorBase<
            ReversedIterator<Iter>,
            RemoveCVRefType<typename IterTraits<Iter>::ReferenceType>,
            typename IterTraits<Iter>::ReferenceType,
            void
        >
    {
        Iter it_{};
    public:
        using IterType        = IterTraits<Iter>::IterType;
        using ReferenceType   = IterTraits<Iter>::ReferenceType;
        using ValueType       = RemoveCVRefType<ReferenceType>;
        using PointerType     = void;
        using DifferenceType  = IterTraits<Iter>::DifferenceType;

        constexpr ReversedIterator() = default;

        explicit constexpr ReversedIterator(IterType it) noexcept : it_{it} {}

        constexpr bool operator==(const ReversedIterator& rhs) const
        {
            return this->it_ == rhs.it_;
        }

        constexpr ReferenceType operator*() const
        {
            auto tmp = this->it_;
            --tmp;
            return *tmp;
        }

        constexpr ReversedIterator& operator++()
        {
            --this->it_;
            return *this;
        }

        constexpr ReversedIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr ReversedIterator& operator--()
        {
            ++this->it_;
            return *this;
        }

        constexpr ReversedIterator operator--(int)
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }
    };

    /**
     * @brief Iterator that yields index-element pairs during traversal.
     *
     * @tparam Iter Underlying forward iterator type
     *
     * Dereferencing returns `Couple<const Size, ReferenceType>`.
     */
    template<ForwardIterator Iter>
    class EnumIterator
        : public ForwardIteratorBase<
            EnumIterator<Iter>,
            RemoveCVRefType<Couple<const Size, typename IterTraits<Iter>::ReferenceType>>,
            Couple<const Size, typename IterTraits<Iter>::ReferenceType>,
            void
        >
    {
        Iter cur_;
        Size index_{};

    public:
        using IterType       = IterTraits<Iter>::IterType;
        using ReferenceType  = Couple<const Size, typename IterTraits<Iter>::ReferenceType>;
        using ValueType      = RemoveCVRefType<ReferenceType>;
        using PointerType    = void;
        using DifferenceType = IterTraits<Iter>::DifferenceType;

        EnumIterator() noexcept = default;

        /**
         * @brief Constructs the iterator.
         *
         * @param cur Current position
         * @param start Starting index value
         */
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

    /**
     * @brief Iterator that applies a transformation function on dereference.
     *
     * @tparam Iter Underlying forward iterator type
     * @tparam F Callable type
     */
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
        using IterType       = IterTraits<Iter>::IterType;
        using ReferenceType  = InvokeResultType<F, typename IterTraits<Iter>::ReferenceType>;
        using ValueType      = RemoveCVRefType<ReferenceType>;
        using PointerType    = void;
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

    /**
     * @brief Iterator that skips elements not satisfying a predicate.
     *
     * @tparam Iter Underlying forward iterator type
     * @tparam F Predicate type
     */
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
        using IterType       = IterTraits<Iter>::IterType;
        using ReferenceType  = IterTraits<Iter>::ReferenceType;
        using ValueType      = RemoveCVRefType<ReferenceType>;
        using PointerType    = void;
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

    /**
     * @brief Iterator for concatenating two ranges.
     *
     * @tparam Prev Iterator type of the first range
     * @tparam Next Iterator type of the second range
     */
    template<ForwardIterator Prev, ForwardIterator Next>
    class ConcatIterator
        : public ForwardIteratorBase<
            ConcatIterator<Prev, Next>,
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
        using ValueType      = RemoveCVRefType<ReferenceType>;
        using PointerType    = void;
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

    /**
     * @brief Iterator that yields pairs of elements from two ranges.
     *
     * @tparam LHS Left-hand iterator type
     * @tparam RHS Right-hand iterator type
     *
     * Stops when either underlying range is exhausted.
     */
    template<ForwardIterator LHS, ForwardIterator RHS>
    class ZipIterator
        : public ForwardIteratorBase<
            ZipIterator<LHS, RHS>,
            Couple<typename IterTraits<LHS>::ValueType, typename IterTraits<RHS>::ValueType>,
            Couple<typename IterTraits<LHS>::ReferenceType, typename IterTraits<RHS>::ReferenceType>,
            void
        >
    {
        LHS cur_lhs_;
        LHS end_lhs_;
        RHS cur_rhs_;
        RHS end_rhs_;

    public:
        using IterType       = ZipIterator;
        using ValueType      = Couple<typename IterTraits<LHS>::ValueType, typename IterTraits<RHS>::ValueType>;
        using ReferenceType  = Couple<typename IterTraits<LHS>::ReferenceType, typename IterTraits<RHS>::ReferenceType>;
        using PointerType    = void;
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
}