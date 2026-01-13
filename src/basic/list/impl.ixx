module;
#include <stdexcept>
export module original.basic.list.impl;
import original.basic.types;
import original.basic.number;
import original.basic.algorithm;
import original.basic.iterator;
import original.basic.range;
import original.basic.allocator;


export namespace original
{
    using namespace original::literals;
    template<IsObject T, Allocator Alloc = NormalAllocator>
    class List
    {
        using Self             = List;
    public:
        using ValueType        = T;
        using ConstValueType   = const ValueType;
        using PointerType      = ValueType*;
        using ConstPointerType = ValueType *const;
        using IterType         = DefaultIterator<ValueType, SpecifiedSource<Self>>;
        using ConstIterType    = DefaultIterator<ConstValueType, SpecifiedSource<Self>>;
        using SizeType         = Size;
        using AllocatorType    = Alloc;
    private:
        using AllocTraits      = AllocatorTraits<AllocatorType>;
        using AllocLayout      = DefaultLayout;

        PointerType data_{};
        SizeType size_{};
        SizeType capacity_{};
        AllocatorType alloc_;

        static AllocLayout layout(const SizeType capacity) noexcept
        {
            return AllocLayout{
                Size{sizeof(ValueType)} * capacity,
                Size{alignof(ValueType)}
            };
        }

        static PointerType ptrAt(PointerType ptr, const SizeType index) noexcept
        {
            return ptr + index.value();
        }

        void setNull() noexcept
        {
            this->size_ = 0_size;
            this->capacity_ = 0_size;
            this->data_ = nullptr;
        }

        static void destroyElements(ConstPointerType data, const SizeType cnt) noexcept
        {
            if (data)
            {
                for(const auto i : range::iota(cnt))
                {
                    AllocTraits::destroy(ptrAt(data, i));
                }
            }
        }

        void deallocateMemory(PointerType data, const SizeType capacity) noexcept
        {
            if (data)
            {
                AllocTraits::deallocate(this->alloc_, data, layout(capacity));
            }
        }

        void destroySelf() noexcept
        {
            destroyElements(this->data_, this->size_);
            this->deallocateMemory(this->data_, this->capacity_);
            this->setNull();
        }

        void* allocateMemory(const SizeType capacity)
        {
            void* new_data = AllocTraits::allocate(this->alloc_, layout(capacity));
            if (!new_data)
                throw std::bad_alloc{};
            return new_data;
        }

        template<bool Copy>
        static decltype(auto) forwardElement(PointerType old_data, const SizeType index)
        {
            if constexpr (Copy)
            {
                return old_data[index.value()];
            }
            else
            {
                return std::move_if_noexcept(old_data[index.value()]);
            }
        }

        template<bool Copy>
        static void forwardElements(ConstPointerType new_data,
                                    ConstPointerType old_data,
                                    const SizeType cnt)
        {
            SizeType cur{};
            try
            {
                for (const auto i: range::iota(cnt))
                {
                    AllocTraits::construct(ptrAt(new_data, i), forwardElement<Copy>(old_data, i));
                    ++cur;
                }
            }
            catch (...)
            {
                destroyElements(new_data, cur);
                throw;
            }
        }

        void init(const SizeType capacity)
        {
            this->size_ = 0_size;
            this->capacity_ = algorithms::maximum(1_size, capacity);
            this->data_ = static_cast<PointerType>(this->allocateMemory(this->capacity_));
        }

        void copyFrom(const List& rhs)
        requires CopyConstructible<ValueType>
        {
            if constexpr (StatefulAllocator<AllocatorType>)
            {
                this->alloc_ = rhs.alloc_;
            }
            this->size_ = rhs.size_;
            this->capacity_ = rhs.capacity_;
            this->data_ = static_cast<PointerType>(this->allocateMemory(this->capacity_));
            forwardElements<true>(this->data_, rhs.data_, this->size_);
        }

        void moveFrom(List&& rhs) noexcept
        {
            if constexpr (StatefulAllocator<AllocatorType>)
            {
                this->alloc_ = std::move(rhs.alloc_);
            }
            this->size_ = rhs.size_;
            this->capacity_ = rhs.capacity_;
            this->data_ = rhs.data_;
            rhs.setNull();
        }

        void reallocate(const SizeType new_capacity)
        {
            if (new_capacity <= this->size_ || new_capacity < 1_size)
                return;
            void* new_data = this->allocateMemory(new_capacity);
            forwardElements<false>(static_cast<PointerType>(new_data), this->data_, this->size_);
            destroyElements(this->data_, this->size_);
            this->deallocateMemory(this->data_, this->capacity_);
            this->capacity_ = new_capacity;
            this->data_ = static_cast<PointerType>(new_data);
        }
    public:
        explicit List(AllocatorType alloc = {}) : alloc_(std::move(alloc))
        {
            this->init(1_size);
        }

        explicit List(const Size capacity, AllocatorType alloc = {}) : alloc_(std::move(alloc))
        {
            this->init(algorithms::maximum(1_size, capacity));
        }

        List(std::initializer_list<T> lst, AllocatorType alloc = {}) : alloc_(std::move(alloc))
        {
            this->init(algorithms::maximum(1_size, 2_size * lst.size()));
            SizeType cnt{lst.size()};
            forwardElements<false>(this->data_, const_cast<PointerType>(lst.begin()), cnt);
            this->size_ = cnt;
        }

        List(const List& rhs)
        requires CopyConstructible<ValueType>
        {
            this->copyFrom(rhs);
        }

        List& operator=(const List& rhs)
        requires CopyConstructible<ValueType>
        {
            if (this == &rhs)
                return *this;
            this->destroySelf();
            this->copyFrom(rhs);
            return *this;
        }

        List(List&& rhs) noexcept
        {
            this->moveFrom(std::move(rhs));
        }

        List& operator=(List&& rhs) noexcept
        {
            if (this == &rhs)
                return *this;
            this->destroySelf();
            this->moveFrom(std::move(rhs));
            return *this;
        }

        void reserve(const SizeType new_capacity)
        {
            if (new_capacity < this->capacity_)
                return;
            this->reallocate(new_capacity);
        }

        template<typename... Args>
        void emplaceEnd(Args&&... args)
        {
            if (this->size_ == this->capacity_)
                this->reallocate(2_size * algorithms::maximum(1_size, this->capacity_));
            AllocTraits::construct(ptrAt(this->data_, this->size_), std::forward<Args>(args)...);
            ++this->size_;
        }

        template<UnsignedIntegralLike U, typename... Args>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void emplace(U index, Args&&... args)
        {
            if (index > this->size_)
                throw std::out_of_range{"Index out of range"};
            if (index == this->size_)
                return this->emplaceEnd(std::forward<Args>(args)...);
            if (this->size_ == this->capacity_)
                this->reallocate(2_size * algorithms::maximum(1_size, this->capacity_));
            AllocTraits::construct(ptrAt(this->data_, this->size_));
            algorithms::moveBackwards(this->begin() + numericCast<Diff>(index),
                                      this->end(),
                                      this->end() + 1_diff);
            AllocTraits::construct(ptrAt(this->data_, SizeType{index}), std::forward<Args>(args)...);
            ++this->size_;
        }

        void pushEnd(const ValueType& value)
        {
            this->emplaceEnd(value);
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void push(U index, const ValueType& value)
        {
            this->emplace(index, value);
        }

        void pushEnd(ValueType&& value)
        {
            this->emplaceEnd(std::move(value));
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void push(U index, ValueType&& value)
        {
            this->emplace(index, std::move(value));
        }

        void popEnd()
        {
            if (this->size_ == 0_size)
                throw std::logic_error{"popEnd() called on empty list"};
            AllocTraits::destroy(ptrAt(this->data_, this->size_ - 1_size));
            --this->size_;
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void pop(U index)
        {
            if (this->size_ == 0_size)
                throw std::logic_error{"pop() called on empty list"};
            if (index >= this->size_)
                throw std::out_of_range{"Index out of range"};
            if (index == this->size_ - 1_size)
                return this->popEnd();
            algorithms::move(this->begin() + numericCast<Diff>(index) + 1_diff,
                             this->end(),
                             this->begin() + numericCast<Diff>(index));
            --this->size_;
            AllocTraits::destroy(ptrAt(this->data_, this->size_));
        }

        IterType begin() noexcept
        {
            return IterType{this->data_};
        }

        IterType end() noexcept
        {
            return IterType{ptrAt(this->data_, this->size_)};
        }

        ConstIterType begin() const noexcept
        {
            return ConstIterType{this->data_};
        }

        ConstIterType end() const noexcept
        {
            return ConstIterType{ptrAt(this->data_, this->size_)};
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        const ValueType& at(U index) const
        {
            if (index >= this->size_)
                throw std::out_of_range{"Index out of range"};

            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        ValueType& at(U index)
        {
            if (index >= this->size_)
                throw std::out_of_range{"Index out of range"};

            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        const ValueType& operator[](U index) const noexcept
        {
            return this->data_[numberLikeValue(index)];
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        ValueType& operator[](U index) noexcept
        {
            return this->data_[numberLikeValue(index)];
        }

        [[nodiscard]] PointerType data() const noexcept
        {
            return this->data_;
        }

        [[nodiscard]] SizeType size() const noexcept
        {
            return this->size_;
        }

        [[nodiscard]] SizeType capacity() const noexcept
        {
            return this->capacity_;
        }

        bool operator==(const List& rhs) const
        requires EqualityComparable<ValueType>
        {
            if (this->size_ != rhs.size_)
                return false;

            return algorithms::equal(*this, rhs);
        }

        auto operator<=>(const List& rhs) const
        requires ThreeWayComparable<ValueType>
        {
            return algorithms::lexicographicallyCompare(*this, rhs);
        }

        ~List()
        {
            this->destroySelf();
        }
    };
}