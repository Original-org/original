module;
#include <type_traits>
#include <iterator>
export module original.basic.iterator.adaptor;
import orginal.basic.iterator.impl;
import original.basic.types;
import original.basic.number;
import original.basic.iterator.traits;


export namespace original
{
    /**
     * @brief Adapter for custom iterators to standard iterator interface.
     * @tparam Iter Iterator type to adapt.
     * @note Provides standard iterator semantics for non-standard iterator types.
     */
    template<Iterator Iter>
    class StdIteratorAdapter {
    public:
        using IterType        = Iter;
        using DerivedType     = StdIteratorAdapter;
        using ValueType       = Iter::ValueType;
        using ReferenceType   = Iter::ReferenceType;
        using PointerType     = Iter::PointerType;
        using DifferenceType  = IterTrait<Iter>::DifferenceType;

        using iterator_type   = IterTrait<Iter>::IterType;       ///< Original iterator type.
        using value_type      = IterTrait<iterator_type>::ValueType;     ///< Element value type.
        using reference       = IterTrait<iterator_type>::ReferenceType; ///< Element reference type.
        using pointer         = IterTrait<iterator_type>::PointerType;   ///< Element pointer type.
        using difference_type = NumberLikeType<DifferenceType>;        ///< Standard difference type.

        /**
         * @brief Iterator category tag for STL compatibility.
         * Automatically selects the appropriate STL iterator category based on the
         * capabilities of the underlying iterator type.
         */
        using iterator_category = std::conditional_t<
            ContiguousIterator<iterator_type>,
            std::contiguous_iterator_tag,
            std::conditional_t<
                RandomAccessIterator<iterator_type>,
                std::random_access_iterator_tag,
                std::conditional_t<
                    BidirectionalIterator<iterator_type>,
                    std::bidirectional_iterator_tag,
                    std::conditional_t<
                        ForwardIterator<iterator_type>,
                        std::forward_iterator_tag,
                        std::input_iterator_tag
                    >
                >
            >
        >;

    private:
        iterator_type it_;  ///< Underlying iterator.

    public:
        /**
         * @brief Default constructor.
         */
        constexpr StdIteratorAdapter() = default;

        /**
         * @brief Construct from iterator.
         * @param it Iterator to adapt.
         */
        constexpr explicit StdIteratorAdapter(iterator_type it) : it_(it) {}

        /**
         * @brief Dereference operator.
         * @return Reference to current element.
         */
        constexpr reference operator*() const {
            return *this->it_;
        }

        /**
         * @brief Member access operator.
         * @return Pointer to current element.
         */
        constexpr pointer operator->() const {
            return this->it_.operator->();
        }

        /**
         * @brief Equality comparison.
         * @param rhs Other adapter.
         * @return True if underlying iterators are equal.
         */
        constexpr bool operator==(const StdIteratorAdapter& rhs) const {
            return this->it_ == rhs.it_;
        }

        /**
         * @brief Prefix increment.
         * @return Reference to incremented adapter.
         * @requires ForwardIterator<iterator_type>
         */
        constexpr StdIteratorAdapter& operator++()
        requires ForwardIterator<iterator_type>
        {
            ++this->it_;
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @return Copy of adapter before increment.
         * @requires ForwardIterator<iterator_type>
         */
        constexpr StdIteratorAdapter operator++(int)
        requires ForwardIterator<iterator_type>
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to decremented adapter.
         * @requires BidirectionalIterator<iterator_type>
         */
        constexpr StdIteratorAdapter& operator--()
        requires BidirectionalIterator<iterator_type>
        {
            --this->it_;
            return *this;
        }

        /**
         * @brief Postfix decrement.
         * @return Copy of adapter before decrement.
         * @requires BidirectionalIterator<iterator_type>
         */
        constexpr StdIteratorAdapter operator--(int)
        requires BidirectionalIterator<iterator_type>
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        /**
         * @brief Compound addition assignment.
         * @param n Distance to move forward.
         * @return Reference to moved adapter.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        constexpr StdIteratorAdapter& operator+=(const T n)
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            this->it_ += DifferenceType{n};
            return *this;
        }

        /**
         * @brief Compound subtraction assignment.
         * @param n Distance to move backward.
         * @return Reference to moved adapter.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        constexpr StdIteratorAdapter& operator-=(const T n)
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return *this += -n;
        }

        /**
         * @brief Addition operator.
         * @param n Distance to move forward.
         * @return New adapter at offset position.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        constexpr StdIteratorAdapter operator+(const T n) const
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return StdIteratorAdapter{ this->it_ + DifferenceType{n} };
        }

        /**
         * @brief Distance operator.
         * @param rhs Other adapter.
         * @return Number of elements between adapters.
         * @requires RandomAccessIterator<iterator_type>
         * @note Returns standard difference type after NumberLike conversion.
         */
        constexpr difference_type operator-(const StdIteratorAdapter& rhs) const
        requires RandomAccessIterator<iterator_type>
        {
            return numberLikeValue(this->it_ - rhs.it_);
        }

        /**
         * @brief Three-way comparison.
         * @param rhs Other adapter.
         * @return Ordering relationship.
         * @requires RandomAccessIterator<iterator_type>
         */
        constexpr auto operator<=>(const StdIteratorAdapter& rhs) const
        requires RandomAccessIterator<iterator_type>
        {
            return this->it_ <=> rhs.it_;
        }

        /**
         * @brief Subscript operator.
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        constexpr reference operator[](T n)
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return this->it_[DifferenceType{n}];
        }

        /**
         * @brief Subscript operator (const).
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        constexpr reference operator[](T n) const
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return this->it_[DifferenceType{n}];
        }

        /**
         * @brief Addition with distance on left.
         * @param n Distance to move forward.
         * @param it Adapter to move.
         * @return New adapter at offset position.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        friend constexpr StdIteratorAdapter operator+(T n, StdIteratorAdapter it)
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return StdIteratorAdapter{it.it_ + DifferenceType{n}};
        }

        /**
         * @brief Subtraction with distance.
         * @param it Adapter to move.
         * @param n Distance to move backward.
         * @return New adapter at offset position.
         * @requires RandomAccessIterator<iterator_type>
         * @note Accepts NumberLike types, converted to DifferenceType.
         */
        template<SignedIntegralLike T>
        friend constexpr StdIteratorAdapter operator-(StdIteratorAdapter it, T n)
        requires RandomAccessIterator<iterator_type> &&
        StdConvertible<NumberLikeType<T>, difference_type>
        {
            return StdIteratorAdapter{it.it_ - DifferenceType{n}};
        }
    };
}

export namespace original::iterator
{
    template<StdObject T>
    constexpr auto makeIterator(T* ptr)
    {
        return DefaultIterator<T>{ptr};
    }

    template<Iterator It>
    constexpr auto toStd(It it)
    {
        return StdIteratorAdapter<It>{it};
    }
}