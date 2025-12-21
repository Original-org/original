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
        typename L::SizeType;
        typename L::AlignType;

        requires UnsignedIntegralLike<typename L::SizeType>;
        requires UnsignedIntegralLike<typename L::AlignType>;
        { l.size() } -> StdSame<typename L::SizeType>;
        { l.align() } -> StdSame<typename L::AlignType>;
    };

    template<typename>
    struct AllocLayoutTraits;

    template<AllocationLayout L>
    struct AllocLayoutTraits<L>
    {
        using LayoutType = L;
        using SizeType = L::SizeType;
        using AlignType = L::AlignType;
    };

    struct DefaultLayout
    {
        using SizeType  = std::size_t;
        using AlignType = std::size_t;

        SizeType size_;
        AlignType align_;

        [[nodiscard]] constexpr SizeType size() const noexcept
        {
            return this->size_;
        }

        [[nodiscard]] constexpr AlignType align() const noexcept
        {
            return this->align_;
        }
    };

    template<typename A, typename L>
    concept CanAllocate =
        StdObject<A> && AllocationLayout<L> &&
        requires(A& a, L l)
    {
        { a.allocate(l.size(), l.align()) } -> StdSame<void*>;
    };

    template<typename A, typename L>
    concept CanDeallocate =
        StdObject<A> && AllocationLayout<L> &&
        requires(A& a, void* ptr, L l)
    {
        { a.deallocate(ptr, l.size(), l.align()) } -> StdSame<void>;
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
