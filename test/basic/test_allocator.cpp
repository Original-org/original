#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <cstring> // NOLINT

// Import the modules (assuming the code is compiled as C++20 modules)
import original.basic.allocator;
import original.basic.number;

using namespace original;
using namespace original::literals;

// Test fixture for allocator tests
class AllocatorTest : public ::testing::Test {};

// Test DefaultLayout construction and accessors
TEST_F(AllocatorTest, DefaultLayout_ValidParameters) {
    constexpr auto size = 128_size;
    constexpr auto align = 16_size;

    constexpr DefaultLayout layout(size, align);

    EXPECT_EQ(layout.size(), size);
    EXPECT_EQ(layout.align(), align);
}

TEST_F(AllocatorTest, DefaultLayout_DefaultAlignment) {
    constexpr auto size = 64_size;
    constexpr DefaultLayout layout(size);

    EXPECT_EQ(layout.size(), size);
    EXPECT_GE(layout.align().value(), alignof(std::max_align_t));
    EXPECT_TRUE(validAlign(layout.align()));
}

TEST_F(AllocatorTest, DefaultLayout_MinimumAlignmentEnforced) {
    constexpr auto size = 32_size;
    constexpr auto small_align = 4_size;

    constexpr DefaultLayout layout(size, small_align);

    EXPECT_EQ(layout.size(), size);
    EXPECT_GE(layout.align().value(), alignof(std::max_align_t));
}

TEST_F(AllocatorTest, DefaultLayout_InvalidAlignmentThrows) {
    EXPECT_THROW(DefaultLayout(100_size, 0_size), std::invalid_argument);
    EXPECT_THROW(DefaultLayout(100_size, 6_size), std::invalid_argument); // Not power of 2
}

// Test TypedLayout
TEST_F(AllocatorTest, TypedLayout_SizeAndAlignment) {
    using IntLayout = TypedLayout<int>;
    using DoubleLayout = TypedLayout<double>;

    EXPECT_EQ(IntLayout::size().value(), sizeof(int));
    EXPECT_EQ(IntLayout::align().value(), alignof(int));

    EXPECT_EQ(DoubleLayout::size().value(), sizeof(double));
    EXPECT_EQ(DoubleLayout::align().value(), alignof(double));
}

// Test concepts (compile-time checks via static_assert)
TEST_F(AllocatorTest, Concepts_NormalAllocator) {
    static_assert(Allocator<NormalAllocator>, "NormalAllocator must satisfy Allocator concept");
    static_assert(StatelessAllocator<NormalAllocator>, "NormalAllocator must be stateless");
    static_assert(!StatefulAllocator<NormalAllocator>, "NormalAllocator must not be stateful");

    static_assert(CanAllocate<NormalAllocator>, "NormalAllocator must support allocation");
    static_assert(CanDeallocate<NormalAllocator>, "NormalAllocator must support deallocation");

    static_assert(!Allocator<int>);
}

TEST_F(AllocatorTest, Concepts_Layouts) {
    static_assert(AllocationLayout<DefaultLayout>, "DefaultLayout must satisfy AllocationLayout");
    static_assert(AllocationLayout<TypedLayout<int>>, "TypedLayout must satisfy AllocationLayout");
}

// Test NormalAllocator allocation and deallocation
TEST_F(AllocatorTest, NormalAllocator_AllocateAndDeallocate) {
    constexpr auto size = 100_size;
    constexpr auto align = 32_size;

    void* ptr = NormalAllocator::allocate(size, align);
    ASSERT_NE(ptr, nullptr);

    // Verify alignment
    const auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    EXPECT_EQ(addr % align.value(), 0u);

    // Basic write/read test to ensure memory is usable
    std::memset(ptr, 0xAA, size.value());
    EXPECT_EQ(static_cast<std::uint8_t*>(ptr)[0], 0xAAu);

    NormalAllocator::deallocate(ptr, size, align);
}

TEST_F(AllocatorTest, NormalAllocator_AllocateZeroSize) {
    void* ptr = NormalAllocator::allocate(0_size, 16_size);
    EXPECT_EQ(ptr, nullptr);

    // Deallocating nullptr should be safe
    NormalAllocator::deallocate(ptr, 0_size, 16_size);
}

// Test AllocatorTraits with NormalAllocator
TEST_F(AllocatorTest, AllocatorTraits_AllocateDeallocate) {
    using Traits = AllocatorTraits<NormalAllocator>;
    constexpr DefaultLayout layout(200_size, 64_size);

    NormalAllocator allocator;
    void* ptr = Traits::allocate(allocator, layout);
    ASSERT_NE(ptr, nullptr);

    const auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    EXPECT_EQ(addr % layout.align().value(), 0u);

    Traits::deallocate(allocator, ptr, layout);
}

// Test construction and destruction via traits
TEST_F(AllocatorTest, AllocatorTraits_ConstructDestroy) {
    using Traits = AllocatorTraits<NormalAllocator>;
    constexpr DefaultLayout layout(Size{sizeof(int)}, Size{alignof(int)});

    void* ptr = NormalAllocator::allocate(layout.size(), layout.align());
    ASSERT_NE(ptr, nullptr);

    const auto i_ptr = static_cast<int*>(ptr);
    Traits::construct(i_ptr, 42);

    EXPECT_EQ(*i_ptr, 42);

    Traits::destroy(i_ptr);

    NormalAllocator::deallocate(ptr, layout.size(), layout.align());
}

// Additional test for over-aligned allocation (common in practice)
TEST_F(AllocatorTest, NormalAllocator_OverAligned) {
    struct OverAligned { alignas(64) double data[8]; };

    using Layout = TypedLayout<OverAligned>;
    void* ptr = NormalAllocator::allocate(Layout::size(), Layout::align());
    ASSERT_NE(ptr, nullptr);

    const auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    EXPECT_EQ(addr % Layout::align().value(), 0u);

    NormalAllocator::deallocate(ptr, Layout::size(), Layout::align());
}

TEST_F(AllocatorTest, AllocatorTraits_AllocateDeallocateViaLayout) {
    using Traits = AllocatorTraits<NormalAllocator>;
    NormalAllocator allocator;

    constexpr DefaultLayout layout(256_size, 32_size);

    void* ptr = Traits::allocate(allocator, layout);
    ASSERT_NE(ptr, nullptr);


    const auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    EXPECT_EQ(addr % layout.align().value(), 0u);

    std::memset(ptr, 0x55, layout.size().value());
    EXPECT_EQ(static_cast<std::uint8_t*>(ptr)[0], 0x55u);

    Traits::deallocate(allocator, ptr, layout);
}

TEST_F(AllocatorTest, AllocatorTraits_AllocateDeallocateWithTypedLayout) {
    using Traits = AllocatorTraits<NormalAllocator>;
    using Layout = TypedLayout<std::int64_t[16]>;
    NormalAllocator allocator;

    void* ptr = Traits::allocate(allocator, Layout{});
    ASSERT_NE(ptr, nullptr);

    const auto addr = reinterpret_cast<std::uintptr_t>(ptr);
    EXPECT_EQ(addr % Layout::align().value(), 0u);

    Traits::deallocate(allocator, ptr, Layout{});
}

TEST_F(AllocatorTest, AllocatorTraits_ConstructDestroySingleObject) {
    using Traits = AllocatorTraits<NormalAllocator>;

    struct TestObject {
        int value;
        bool constructed = false;

        explicit TestObject(const int v) : value(v), constructed(true) {}
        ~TestObject() { value = -1; }
    };

    constexpr DefaultLayout layout(Size(sizeof(TestObject)), Size(alignof(TestObject)));
    void* raw = NormalAllocator::allocate(layout.size(), layout.align());
    ASSERT_NE(raw, nullptr);

    const auto obj = static_cast<TestObject*>(raw);

    Traits::construct(obj, 123);
    EXPECT_EQ(obj->value, 123);
    EXPECT_TRUE(obj->constructed);

    Traits::destroy(obj);
    EXPECT_EQ(obj->value, -1);

    NormalAllocator::deallocate(raw, layout.size(), layout.align());
}

TEST_F(AllocatorTest, AllocatorTraits_ConstructWithForwarding) {
    using Traits = AllocatorTraits<NormalAllocator>;

    struct MoveOnly {
        int value;
        explicit MoveOnly(const int v) : value(v) {}
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&& other) noexcept : value(other.value) { other.value = 0; }
    };

    constexpr DefaultLayout layout(Size(sizeof(MoveOnly)), Size(alignof(MoveOnly)));
    void* raw = NormalAllocator::allocate(layout.size(), layout.align());
    ASSERT_NE(raw, nullptr);

    const auto obj = static_cast<MoveOnly*>(raw);

    MoveOnly temp(456);
    Traits::construct(obj, std::move(temp));

    EXPECT_EQ(obj->value, 456);
    EXPECT_EQ(temp.value, 0);

    Traits::destroy(obj);

    NormalAllocator::deallocate(raw, layout.size(), layout.align());
}

TEST_F(AllocatorTest, AllocatorTraits_AllocationFailureReturnsNullptr) {
    using Traits = AllocatorTraits<NormalAllocator>;

    constexpr DefaultLayout huge_layout(Size{static_cast<std::size_t>(-1) / 2}, 16_size);

    NormalAllocator allocator;
    void* ptr = Traits::allocate(allocator, huge_layout);
    EXPECT_EQ(ptr, nullptr);
}
