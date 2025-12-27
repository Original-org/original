module;
#include <cstddef>
#include <compare> // NOLINT
#include <stdexcept>
export module original.basic.allocator.layout;
import original.basic.allocator.traits;
import original.basic.types;
import original.basic.number.impl;


export namespace original
{
    struct DefaultLayout
    {
        Size size_;
        Size align_;

        explicit constexpr DefaultLayout(const Size size,
                                         const Size align = Size{alignof(std::max_align_t)})
        {
            if (!validAlign(align))
                throw std::invalid_argument{"align must be power of 2"};
            this->size_ = size;
            this->align_ = align < Size{alignof(std::max_align_t)} ? Size{alignof(std::max_align_t)} : align;
        }

        [[nodiscard]] constexpr Size size() const noexcept
        {
            return this->size_;
        }

        [[nodiscard]] constexpr Size align() const noexcept
        {
            return this->align_;
        }
    };

    template<StdObject T>
    struct TypedLayout
    {
        static constexpr Size size() noexcept
        {
            return Size{sizeof(T)};
        }

        static constexpr Size align() noexcept
        {
            return Size{alignof(T)};
        }
    };
}