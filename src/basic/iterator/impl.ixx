module;
#include <memory>
export module orginal.basic.iterator.impl;
import original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


export namespace original
{
    /**
     * @brief Base class for iterator implementations.
     * @tparam Derived CRTP derived class type.
     * @tparam T Value type.
     * @tparam Reference Reference type.
     * @tparam Pointer Pointer type.
     */
    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer
    >
    class IteratorBase
    {
    public:
        using DerivedType = Derived;      ///< CRTP derived type.
        using ValueType = T;              ///< Element value type.
        using ReferenceType = Reference;  ///< Element reference type.
        using PointerType = Pointer;      ///< Element pointer type.

    protected:
        /**
         * @brief Get derived reference.
         * @return Reference to derived object.
         */
        constexpr DerivedType& derived()
        {
            return static_cast<DerivedType&>(*this);
        }

        /**
         * @brief Get const derived reference.
         * @return Const reference to derived object.
         */
        constexpr const DerivedType& derived() const
        {
            return static_cast<const DerivedType&>(*this);
        }

    public:
        /**
         * @brief Dereference operator.
         * @return Reference to current element.
         */
        constexpr ReferenceType operator*() const
        {
            return derived().operator*();
        }

        /**
         * @brief Dereference operator.
         * @return Reference to current element.
         */
        constexpr ReferenceType operator*()
        {
            return derived().operator*();
        }

        /**
         * @brief Member access operator.
         * @return Pointer to current element.
         */
        constexpr PointerType operator->() const
        {
            return std::addressof(**this);
        }

        /**
         * @brief Member access operator.
         * @return Pointer to current element.
         */
        constexpr PointerType operator->()
        {
            return std::addressof(**this);
        }

        /**
         * @brief Equality comparison.
         * @param rhs Other iterator to compare.
         * @return True if iterators are equal.
         */
        constexpr bool operator==(const Derived& rhs) const
        {
            return derived() == rhs;
        }
    };

    /**
     * @brief Base class for forward iterators.
     * @tparam Derived CRTP derived class type.
     * @tparam T Value type.
     * @tparam Reference Reference type.
     * @tparam Pointer Pointer type.
     */
    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer
    >
    class ForwardIteratorBase : public IteratorBase<Derived, T, Reference, Pointer>
    {
    protected:
        constexpr ForwardIteratorBase() = default;

    public:
        using DerivedType = Derived;
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;

        /**
         * @brief Prefix increment.
         * @return Reference to incremented iterator.
         */
        constexpr DerivedType& operator++()
        {
            ++static_cast<DerivedType&>(*this);
            return static_cast<DerivedType&>(*this);
        }

        /**
         * @brief Postfix increment.
         * @return Copy of iterator before increment.
         */
        constexpr DerivedType operator++(int)
        {
            DerivedType tmp = static_cast<DerivedType&>(*this);
            ++static_cast<DerivedType&>(*this);
            return tmp;
        }
    };

    /**
     * @brief Base class for bidirectional iterators.
     * @tparam Derived CRTP derived class type.
     * @tparam T Value type.
     * @tparam Reference Reference type.
     * @tparam Pointer Pointer type.
     */
    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer
    >
    class BidirectionalIteratorBase
    : public ForwardIteratorBase<Derived, T, Reference, Pointer>
    {
    protected:
        constexpr BidirectionalIteratorBase() = default;

    public:
        using DerivedType = Derived;
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;

        /**
         * @brief Prefix decrement.
         * @return Reference to decremented iterator.
         */
        constexpr DerivedType& operator--()
        {
            --static_cast<DerivedType&>(*this);
            return static_cast<DerivedType&>(*this);
        }

        /**
         * @brief Postfix decrement.
         * @return Copy of iterator before decrement.
         */
        constexpr DerivedType operator--(int)
        {
            DerivedType tmp = static_cast<DerivedType&>(*this);
            --static_cast<DerivedType&>(*this);
            return tmp;
        }
    };

    /**
     * @brief Base class for random access iterators.
     * @tparam Derived CRTP derived class type.
     * @tparam T Value type.
     * @tparam Reference Reference type.
     * @tparam Pointer Pointer type.
     * @tparam Difference Signed integral difference type.
     */
    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer,
        SignedIntegralLike Difference
    >
    class RandomAccessIteratorBase
    : public BidirectionalIteratorBase<Derived, T, Reference, Pointer>
    {
    protected:
        constexpr RandomAccessIteratorBase() = default;

    public:
        using DerivedType = Derived;
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;
        using DifferenceType = Difference;

        /**
         * @brief Compound addition assignment.
         * @param n Distance to move forward.
         * @return Reference to moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr DerivedType& operator+=(U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            static_cast<DerivedType&>(*this) += numberLikeValue(n);
            return static_cast<DerivedType&>(*this);
        }

        /**
         * @brief Compound subtraction assignment.
         * @param n Distance to move backward.
         * @return Reference to moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr DerivedType& operator-=(U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            return *this += -numberLikeValue(n);
        }

        /**
         * @brief Distance between iterators.
         * @param rhs Other iterator.
         * @return Number of elements between iterators.
         */
        constexpr DifferenceType operator-(const RandomAccessIteratorBase& rhs) const
        {
            return static_cast<const DerivedType&>(*this) - static_cast<const DerivedType&>(rhs);
        }

        /**
         * @brief Subscript operator.
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr ReferenceType operator[](U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            DerivedType temp = static_cast<DerivedType&>(*this);
            return *(temp + numberLikeValue(n));
        }

        /**
         * @brief Subscript operator (const).
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr ReferenceType operator[](U n) const
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            DerivedType temp = static_cast<const DerivedType&>(*this);
            return *(temp + numberLikeValue(n));
        }

        /**
         * @brief Three-way comparison.
         * @param rhs Other iterator.
         * @return Ordering relationship.
         */
        constexpr auto operator<=>(const RandomAccessIteratorBase& rhs) const
        {
            return static_cast<const DerivedType&>(*this) <=> static_cast<const DerivedType&>(rhs);
        }

        /**
         * @brief Addition with iterator on left.
         * @param it Iterator to move.
         * @param n Distance to move.
         * @return Moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        friend constexpr DerivedType operator+(DerivedType it, U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            it += n;
            return it;
        }

        /**
         * @brief Addition with iterator on right.
         * @param n Distance to move.
         * @param it Iterator to move.
         * @return Moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        friend constexpr DerivedType operator+(U n, DerivedType it)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            return it + n;
        }

        /**
         * @brief Subtraction with iterator.
         * @param it Iterator to move.
         * @param n Distance to move backward.
         * @return Moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        friend constexpr DerivedType operator-(DerivedType it, U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            it -= n;
            return it;
        }
    };

    /**
     * @brief Normal iterator implementation.
     * @tparam T Value type.
     * @tparam Reference Reference type.
     * @tparam Pointer Pointer type.
     * @tparam Difference Signed integral difference type.
     */
    template<StdObject T,
        StdReference Reference,
        StdPointer Pointer,
        SignedIntegralLike Difference>
    class NormalIterator
    : public RandomAccessIteratorBase<
        NormalIterator<T, Reference, Pointer, Difference>,
        T,
        Reference,
        Pointer,
        Difference
    >
    {
        Pointer ptr_{};

    public:
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;
        using DifferenceType = Difference;

        /**
         * @brief Default constructor.
         */
        constexpr NormalIterator() = default;

        /**
         * @brief Constructor from pointer.
         * @param ptr Pointer to element.
         */
        constexpr explicit NormalIterator(ValueType* ptr) : ptr_{ptr} {}

        /**
         * @brief Dereference operator (const).
         * @return Reference to element.
         */
        constexpr ReferenceType operator*() const
        {
            return *this->ptr_;
        }

        /**
         * @brief Dereference operator.
         * @return Reference to element.
         */
        constexpr ReferenceType operator*()
        {
            return *this->ptr_;
        }

        /**
         * @brief Member access operator (const).
         * @return Pointer to element.
         */
        constexpr PointerType operator->() const
        {
            return this->ptr_;
        }

        /**
         * @brief Member access operator.
         * @return Pointer to element.
         */
        constexpr PointerType operator->()
        {
            return this->ptr_;
        }

        /**
         * @brief Equality comparison.
         * @param rhs Other iterator.
         * @return True if pointers are equal.
         */
        constexpr bool operator==(const NormalIterator& rhs) const
        {
            return this->ptr_ == rhs.ptr_;
        }

        /**
         * @brief Prefix increment.
         * @return Reference to incremented iterator.
         */
        constexpr NormalIterator& operator++()
        {
            ++this->ptr_;
            return *this;
        }

        /**
         * @brief Postfix increment.
         * @return Copy of iterator before increment.
         */
        constexpr NormalIterator operator++(int)
        {
            NormalIterator temp = *this;
            ++this->ptr_;
            return temp;
        }

        /**
         * @brief Prefix decrement.
         * @return Reference to decremented iterator.
         */
        constexpr NormalIterator& operator--()
        {
            --this->ptr_;
            return *this;
        }

        /**
         * @brief Postfix decrement.
         * @return Copy of iterator before decrement.
         */
        constexpr NormalIterator operator--(int)
        {
            NormalIterator temp = *this;
            --this->ptr_;
            return temp;
        }

        /**
         * @brief Compound addition assignment.
         * @param n Distance to move.
         * @return Reference to moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr NormalIterator& operator+=(U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            this->ptr_ += numberLikeValue(n);
            return *this;
        }

        /**
         * @brief Compound subtraction assignment.
         * @param n Distance to move.
         * @return Reference to moved iterator.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr NormalIterator& operator-=(U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            return *this += -n;
        }

        /**
         * @brief Distance between iterators.
         * @param rhs Other iterator.
         * @return Number of elements between.
         */
        constexpr DifferenceType operator-(const NormalIterator& rhs) const
        {
            return DifferenceType{this->ptr_ - rhs.ptr_};
        }

        /**
         * @brief Subscript operator.
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr ReferenceType operator[](U n)
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            return this->ptr_[numberLikeValue(n)];
        }

        /**
         * @brief Subscript operator (const).
         * @param n Offset from current position.
         * @return Reference to element at offset.
         * @note Accepts NumberLike types, converted to DifferenceType's number-like type.
         */
        template<SignedIntegralLike U>
        constexpr ReferenceType operator[](U n) const
        requires StdSame<NumberLikeType<U>, NumberLikeType<DifferenceType>>
        {
            return this->ptr_[numberLikeValue(n)];
        }

        /**
         * @brief Three-way comparison.
         * @param rhs Other iterator.
         * @return Ordering of pointers.
         */
        constexpr auto operator<=>(const NormalIterator& rhs) const
        {
            return this->ptr_ <=> rhs.ptr_;
        }
    };

    /**
     * @brief Default iterator type alias.
     * @tparam T Value type.
     */
    template<StdObject T>
    using DefaultIterator = NormalIterator<T, T&, T*, Diff>;
}