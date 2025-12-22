module;
#include <cstddef>
export module original.basic.allocator.traits;
import original.basic.types;
import original.basic.number.traits;


export namespace original
{
    template<typename L>
    concept AllocationLayout =
        StdObject<L> &&
        requires(L l)
    {
        { l.size() } -> StdSame<Size>;
        { l.align() } -> StdSame<Size>;
    };

    struct DefaultLayout
    {
        Size size_;
        Size align_;

        explicit constexpr DefaultLayout(const Size size,
                                         const Size align = Size{alignof(std::max_align_t)}) noexcept
            : size_(size), align_(align) {}

        [[nodiscard]] constexpr Size size() const noexcept
        {
            return this->size_;
        }

        [[nodiscard]] constexpr Size align() const noexcept
        {
            return this->align_;
        }
    };

    template<typename A>
    concept CanAllocate =
        StdObject<A> &&
        requires(A& a, Size size, Size align)
    {
        { a.allocate(size, align) } -> StdSame<void*>;
    };

    template<typename A>
    concept CanDeallocate =
        StdObject<A> &&
        requires(A& a, void* ptr, Size size, Size align)
    {
        { a.deallocate(ptr, size, align) } -> StdSame<void>;
    };

    template<typename A>
    struct AllocatorTraits
    {
        static constexpr bool IS_STATEFUL =
            !StdEmpty<A>;

        static constexpr bool IS_ALWAYS_EQUAL =
            StdEmpty<A>;

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
    };
}
