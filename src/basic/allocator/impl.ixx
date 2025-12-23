module;
#include <memory>
#include <new>
export module original.basic.allocator.impl;
import original.basic.number.impl;


namespace original::details
{
    using namespace original::literals;

    void* malloc(const Size size, const Size align)
    {
        if (size == 0_size)
            return nullptr;

        return operator new(size.value(), std::align_val_t{align.value()});
    }

    void free(void* const ptr, const Size align)
    {
        operator delete(ptr, std::align_val_t{align.value()});
    }
}

export namespace original
{
    using namespace literals;

    class NormalAllocator
    {
    public:
        static void* allocate(const Size size, const Size align) noexcept
        {
            try
            {
                return details::malloc(size, align);
            }
            catch (std::bad_alloc&)
            {
                return nullptr;
            }
        }

        static void deallocate(void* const ptr, const Size, const Size align) noexcept
        {
            if (!ptr)
                return;

            details::free(ptr, align);
        }
    };
}