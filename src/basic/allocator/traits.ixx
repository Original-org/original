module;
#include <utility>
export module original.basic.allocator.traits;
import original.basic.types;
import original.basic.number.traits;
import original.basic.number.impl;


export namespace original
{
    using namespace literals;

    template<typename L>
    concept AllocationLayout =
        IsObject<L> &&
        requires(L l)
    {
        { l.size() } -> SameType<Size>;
        { l.align() } -> SameType<Size>;
    };

    constexpr bool validAlign(const Size align) noexcept
    {
        return align != 0_size && (align & (align - 1_size)) == 0_size;
    }

    template<typename A>
    concept CanAllocate =
        IsObject<A> &&
        requires(A& a, Size size, Size align)
    {
        { a.allocate(size, align) } -> SameType<void*>;
    };

    template<typename A>
    concept CanDeallocate =
        IsObject<A> &&
        requires(A& a, void* ptr, Size size, Size align)
    {
        { a.deallocate(ptr, size, align) } -> SameType<void>;
    };

    template<typename A>
    struct AllocatorTraits
    {
        template<AllocationLayout L>
        static void* allocate(A& a, L l)
        {
            return a.allocate(l.size(), l.align());
        }

        template<AllocationLayout L>
        static void deallocate(A& a, void* ptr, L l)
        {
            a.deallocate(ptr, l.size(), l.align());
        }

        template<IsObject T, typename... Args>
        static void construct(T* ptr, Args&&... args)
        {
            new (ptr) T{ std::forward<Args>(args)... };
        }

        template<IsObject T>
        static void destroy(T* ptr)
        {
            ptr->~T();
        }
    };

    template<typename A>
    concept Allocator =
        IsObject<A> &&
        requires(A& a)
    {
        requires CanAllocate<A>;
        requires CanDeallocate<A>;
    };

    template<typename A>
    concept StatelessAllocator =
        Allocator<A> && IsEmpty<A>;

    template<typename A>
    concept StatefulAllocator =
        Allocator<A> && !IsEmpty<A>;
}
