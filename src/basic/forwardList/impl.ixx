module;
#include <stdexcept>
#include <utility>
#include <new>
export module original.basic.forwardList.impl;
import original.basic.types;
import original.basic.number;
import original.basic.allocator;
import original.basic.iterator;
import original.basic.range;


export namespace original
{
    using namespace original::literals;
    template<IsObject T, Allocator Alloc = NormalAllocator>
    class ForwardList
    {
        using Self                = ForwardList;
    public:
        using ValueType           = T;
        using ConstValueType      = const ValueType;
        using ReferenceType       = ValueType&;
        using ConstReferenceType  = const ValueType&;
        using SizeType            = Size;
        using AllocatorType       = Alloc;
    private:
        using AllocTraits         = AllocatorTraits<AllocatorType>;
        using AllocLayout         = DefaultLayout;

        struct BaseNode;
        struct Node;
        using BaseNodeType        = BaseNode;
        using BaseNodePointerType = BaseNode*;
        using NodeType            = Node;
        using NodePointerType     = NodeType*;

        struct BaseNode
        {
            BaseNodePointerType next_{};

            BaseNode() = default;

            BaseNode(const BaseNode&)
            {
                this->next_ = nullptr;
            }
        };

        struct Node : BaseNode
        {
            ValueType value_{};

            template<typename... Args>
            explicit Node(Args&&... args) : BaseNode(), value_(std::forward<Args>(args)...) {}
        };

        template<typename Value, typename Reference>
        class Iterator
        : public ForwardIteratorBase<
            Iterator<Value, Reference>, Value, Reference, void
        >
        {
            BaseNodePointerType cur_{};

            explicit constexpr Iterator(const BaseNodePointerType cur) noexcept
                : cur_{cur} {}
        public:
            friend ForwardList;

            using ValueType      = Value;
            using ReferenceType  = Reference;
            using PointerType    = void;
            using DifferenceType = Diff;

            constexpr Iterator() noexcept = default;

            constexpr Reference operator*() const
            {
                auto cur = static_cast<NodePointerType>(this->cur_);
                return cur->value_;
            }

            constexpr Iterator& operator++()
            {
                if (BaseNodePointerType cur = this->cur_)
                    this->cur_ = cur->next_;
                else
                    throw std::out_of_range{"ForwardList::Iterator::operator++ out of range"};
                return *this;
            }

            constexpr Iterator operator++(int)
            {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr bool operator==(const Iterator& rhs) const
            {
                return this->cur_ == rhs.cur_;
            }
        };

        BaseNode* head_{};
        BaseNode* before_{};
        AllocatorType alloc_;

        template<typename NodeType>
        static AllocLayout layoutNode() noexcept
        {
            return AllocLayout{
                SizeType{sizeof(NodeType)},
                SizeType{alignof(NodeType)},
            };
        }

        template<typename NodeType>
        void* allocateNode()
        {
            void* new_node = AllocTraits::allocate(this->alloc_, layoutNode<NodeType>());
            if (!new_node)
                throw std::bad_alloc{};
            return new_node;
        }

        template<typename NodeType, typename... Args>
        static void constructNode(void* node, Args&&... args)
        {
            AllocTraits::construct(static_cast<NodeType*>(node), std::forward<Args>(args)...);
        }

        template<typename NodeType, typename... Args>
        NodeType* createNode(Args&&... args)
        {
            void* new_node = this->allocateNode<NodeType>();
            constructNode<NodeType>(new_node, std::forward<Args>(args)...);
            return static_cast<NodeType*>(new_node);
        }

        template<typename NodeType>
        static void destroyNode(NodeType* node) noexcept
        {
            AllocTraits::destroy(node);
        }

        template<typename NodeType>
        void deallocateNode(NodeType* node) noexcept
        {
            AllocTraits::deallocate(this->alloc_, node, layoutNode<NodeType>());
        }

        template<typename NodeType>
        void deleteNode(NodeType* node) noexcept
        {
            destroyNode(node);
            this->deallocateNode(node);
        }

        static void connectNodes(BaseNodePointerType prev, BaseNodePointerType next) noexcept
        {
            if (prev)
                prev->next_ = next;
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        BaseNodePointerType getNodePointer(U index) const
        {
            BaseNodePointerType cur = this->before_;
            for (auto i : range::iota(index))
            {
                if (!cur)
                    throw std::out_of_range{"Index out of range"};
                cur = cur->next_;
            }
            if (!cur)
                throw std::out_of_range{"Index out of range"};
            return cur;
        }

        void setNull() noexcept
        {
            this->before_ = nullptr;
            this->head_ = nullptr;
        }

        void init()
        {
            this->before_ = this->createNode<BaseNodeType>();
        }

        void copyFrom(const ForwardList& rhs)
        requires CopyConstructible<ValueType>
        {
            if constexpr (StatefulAllocator<AllocatorType>)
            {
                this->alloc_ = rhs.alloc_;
            }
            BaseNodePointerType prev = this->before_;
            for (BaseNodePointerType cur = rhs.before_->next_; cur; cur = cur->next_)
            {
                NodePointerType new_node = this->template createNode<NodeType>(
                    static_cast<NodeType&>(*cur)
                );
                connectNodes(prev, new_node);
                prev = new_node;
            }
            this->head_ = this->before_->next_;
        }

        void moveFrom(ForwardList& rhs) noexcept
        {
            if constexpr (StatefulAllocator<AllocatorType>)
            {
                this->alloc_ = std::move(rhs.alloc_);
            }
            this->before_ = rhs.before_;
            this->head_ = rhs.head_;
            rhs.setNull();
        }

        void destroySelf() noexcept
        {
            BaseNodePointerType cur_self = this->before_;
            NodePointerType next;
            if (cur_self)
            {
                next = static_cast<NodePointerType>(cur_self->next_);
                this->deleteNode(cur_self);
                cur_self = next;
            }
            while (cur_self)
            {
                next = static_cast<NodePointerType>(cur_self->next_);
                this->deleteNode(static_cast<NodePointerType>(cur_self));
                cur_self = next;
            }
            this->setNull();
        }

        template<UnsignedIntegralLike U, typename V>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void pushImpl(U index, V&& value)
        {
            BaseNodePointerType prev = this->getNodePointer(index);
            NodePointerType new_node = this->template createNode<NodeType>(std::forward<V>(value));
            connectNodes(new_node, prev->next_);
            connectNodes(prev, new_node);
            this->head_ = this->before_->next_;
        }

        template<typename V>
        void pushEndImpl(V&& value)
        {
            BaseNodePointerType prev = this->before_;
            while (prev->next_)
                prev = prev->next_;
            NodePointerType new_node = this->template createNode<NodeType>(std::forward<V>(value));
            connectNodes(new_node, prev->next_);
            connectNodes(prev, new_node);
            this->head_ = this->before_->next_;
        }
    public:
        using IterType            = Iterator<ValueType, ReferenceType>;
        using ConstIterType       = Iterator<ConstValueType, ConstReferenceType>;

        explicit ForwardList(AllocatorType alloc = {}) : alloc_(std::move(alloc))
        {
            this->init();
        }

        ForwardList(const ForwardList& rhs)
        requires CopyConstructible<ValueType>
        {
            this->copyFrom(rhs);
        }

        ForwardList& operator=(const ForwardList& rhs)
        requires CopyConstructible<ValueType>
        {
            if (this == &rhs)
                return *this;
            this->destroySelf();
            this->copyFrom(rhs);
            return *this;
        }

        ForwardList(ForwardList&& rhs) noexcept
        {
            this->moveFrom(rhs);
        }

        ForwardList& operator=(ForwardList&& rhs) noexcept
        {
            if (this == &rhs)
                return *this;
            this->destroySelf();
            this->moveFrom(rhs);
            return *this;
        }

        void pushBegin(const ValueType& value)
        {
            this->push(0_size, value);
        }

        void pushBegin(ValueType&& value)
        {
            this->push(0_size, std::move(value));
        }

        void pushEnd(const ValueType& value)
        {
            this->pushEndImpl(value);
        }

        void pushEnd(ValueType&& value)
        {
            this->pushEndImpl(std::move(value));
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void push(U index, const ValueType& value)
        {
            this->pushImpl(index, value);
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void push(U index, ValueType&& value)
        {
            this->pushImpl(index, std::move(value));
        }

        void popBegin()
        {
            this->pop(0_size);
        }

        void popEnd()
        {
            if (!this->before_->next_)
                throw std::logic_error{"popEnd() called on empty list"};
            BaseNodePointerType prev = this->before_;
            while (prev->next_ && prev->next_->next_)
                prev = prev->next_;
            NodePointerType target = static_cast<NodePointerType>(prev->next_);
            connectNodes(prev, target->next_);
            this->deleteNode(target);
            this->head_ = this->before_->next_;
        }

        template<UnsignedIntegralLike U>
        requires SameType<NumberLikeType<U>, NumberLikeType<SizeType>>
        void pop(U index)
        {
            if (!this->before_->next_)
                throw std::logic_error{"pop() called on empty list"};
            BaseNodePointerType prev = this->getNodePointer(index);
            if (!prev->next_)
                throw std::out_of_range{"Index out of range"};
            NodePointerType target = static_cast<NodePointerType>(prev->next_);
            connectNodes(prev, target->next_);
            this->deleteNode(target);
            this->head_ = this->before_->next_;
        }

        IterType begin() noexcept
        {
            return IterType{this->before_->next_};
        }

        IterType end() noexcept
        {
            return IterType{nullptr};
        }

        ConstIterType begin() const noexcept
        {
            return ConstIterType{this->before_->next_};
        }

        ConstIterType end() const noexcept
        {
            return ConstIterType{nullptr};
        }

        ~ForwardList()
        {
            this->destroySelf();
        }
    };
}
