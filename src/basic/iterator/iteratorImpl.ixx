module;
#include <memory>
export module orginal.basic.iterator.iteratorImpl;
import original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


export namespace original
{
    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer
    >
    class IteratorBase
    {
    public:
        using DerivedType = Derived;
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;
    protected:
        constexpr DerivedType& derived()
        {
            return static_cast<DerivedType&>(*this);
        }

        constexpr const DerivedType& derived() const
        {
            return static_cast<const DerivedType&>(*this);
        }
    public:
        constexpr ReferenceType operator*() const
        {
            return derived().operator*();
        }

        constexpr ReferenceType operator*()
        {
            return derived().operator*();
        }

        constexpr PointerType operator->() const
        {
            return std::addressof(**this);
        }

        constexpr PointerType operator->()
        {
            return std::addressof(**this);
        }

        constexpr bool operator==(const Derived& rhs) const
        {
            return derived() == rhs;
        }
    };

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

        constexpr DerivedType& operator++()
        {
            ++static_cast<DerivedType&>(*this);
            return static_cast<DerivedType&>(*this);
        }

        constexpr DerivedType operator++(int)
        {
            DerivedType tmp = static_cast<DerivedType&>(*this);
            ++static_cast<DerivedType&>(*this);
            return tmp;
        }
    };

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

        constexpr DerivedType& operator--()
        {
            --static_cast<DerivedType&>(*this);
            return static_cast<DerivedType&>(*this);
        }

        constexpr DerivedType operator--(int)
        {
            DerivedType tmp = static_cast<DerivedType&>(*this);
            --static_cast<DerivedType&>(*this);
            return tmp;
        }
    };

    template<
        typename Derived,
        StdObject T,
        StdReference Reference,
        StdPointer Pointer,
        SignedIntegralLike Distance
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

        constexpr DerivedType& operator+=(Distance n)
        {
            static_cast<DerivedType&>(*this) += numberLikeValue(n);
            return static_cast<DerivedType&>(*this);
        }

        constexpr DerivedType& operator-=(Distance n)
        {
            return *this += -numberLikeValue(n);
        }

        constexpr Distance operator-(const RandomAccessIteratorBase& rhs) const
        {
            return static_cast<const DerivedType&>(*this) - static_cast<const DerivedType&>(rhs);
        }

        constexpr ReferenceType operator[](Distance n)
        {
            DerivedType temp = static_cast<DerivedType&>(*this);
            return *(temp + numberLikeValue(n));
        }

        constexpr ReferenceType operator[](Distance n) const
        {
            DerivedType temp = static_cast<const DerivedType&>(*this);
            return *(temp + numberLikeValue(n));
        }

        constexpr auto operator<=>(const RandomAccessIteratorBase& rhs) const
        {
            return static_cast<const DerivedType&>(*this) <=> static_cast<const DerivedType&>(rhs);
        }

        friend constexpr DerivedType operator+(DerivedType it, Distance n)
        {
            it += n;
            return it;
        }

        friend constexpr DerivedType operator+(Distance n, DerivedType it)
        {
            return it + n;
        }

        friend constexpr DerivedType operator-(DerivedType it, Distance n)
        {
            it -= n;
            return it;
        }
    };

    template<StdObject T,
        StdReference Reference,
        StdPointer Pointer,
        SignedIntegralLike Distance>
    class NormalIterator
    : public RandomAccessIteratorBase<
        NormalIterator<T, Reference, Pointer, Distance>,
        T,
        Reference,
        Pointer,
        Distance
    >
    {
        Pointer ptr_{};

    public:
        using ValueType = T;
        using ReferenceType = Reference;
        using PointerType = Pointer;
        using DistanceType = Distance;

        constexpr NormalIterator() = default;

        constexpr explicit NormalIterator(ValueType* ptr) : ptr_{ptr} {}

        constexpr ReferenceType operator*() const
        {
            return *this->ptr_;
        }

        constexpr ReferenceType operator*()
        {
            return *this->ptr_;
        }

        constexpr PointerType operator->() const
        {
            return this->ptr_;
        }

        constexpr PointerType operator->()
        {
            return this->ptr_;
        }

        constexpr bool operator==(const NormalIterator& rhs) const
        {
            return this->ptr_ == rhs.ptr_;
        }

        constexpr NormalIterator& operator++()
        {
            ++this->ptr_;
            return *this;
        }

        constexpr NormalIterator operator++(int)
        {
            NormalIterator temp = *this;
            ++this->ptr_;
            return temp;
        }

        constexpr NormalIterator& operator--()
        {
            --this->ptr_;
            return *this;
        }

        constexpr NormalIterator operator--(int)
        {
            NormalIterator temp = *this;
            --this->ptr_;
            return temp;
        }

        constexpr NormalIterator& operator+=(Distance n)
        {
            this->ptr_ += numberLikeValue(n);
            return *this;
        }

        constexpr NormalIterator& operator-=(Distance n)
        {
            return *this += -n;
        }

        constexpr DistanceType operator-(const NormalIterator& rhs) const
        {
            return DistanceType{this->ptr_ - rhs.ptr_};
        }

        constexpr ReferenceType operator[](Distance n)
        {
            return this->ptr_[numberLikeValue(n)];
        }

        constexpr ReferenceType operator[](Distance n) const
        {
            return this->ptr_[numberLikeValue(n)];
        }

        constexpr auto operator<=>(const NormalIterator& rhs) const
        {
            return this->ptr_ <=> rhs.ptr_;
        }
    };

    template<StdObject T>
    using DefaultIterator = NormalIterator<T, T&, T*, Diff>;
}
