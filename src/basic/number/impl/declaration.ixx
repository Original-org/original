module;
#include <cstddef> // NOLINT
#include <cstdint>
export module original.basic.number.impl:declaration;
import original.basic.types;


export namespace original
{
    template<StdIntegral T>
    class Integer;

    template<StdFloating T>
    class Floating;

    /** @brief Signed 8-bit integer wrapper. */
    using I8 = Integer<std::int8_t>;
    /** @brief Signed 16-bit integer wrapper. */
    using I16 = Integer<std::int16_t>;
    /** @brief Signed 32-bit integer wrapper. */
    using I32 = Integer<std::int32_t>;
    /** @brief Signed 64-bit integer wrapper. */
    using I64 = Integer<std::int64_t>;
    /** @brief Pointer difference type wrapper. */
    using Diff = Integer<std::ptrdiff_t>;
    /** @brief Unsigned 8-bit integer wrapper. */
    using U8 = Integer<std::uint8_t>;
    /** @brief Unsigned 16-bit integer wrapper. */
    using U16 = Integer<std::uint16_t>;
    /** @brief Unsigned 32-bit integer wrapper. */
    using U32 = Integer<std::uint32_t>;
    /** @brief Unsigned 64-bit integer wrapper. */
    using U64 = Integer<std::uint64_t>;
    /** @brief Alias for U8 representing a byte. */
    using Byte = U8;
    /** @brief Size type wrapper. */
    using Size = Integer<std::size_t>;
    /** @brief Single-precision floating-point wrapper. */
    using F32 = Floating<float>;
    /** @brief Double-precision floating-point wrapper. */
    using F64 = Floating<double>;
    /** @brief Extended-precision floating-point wrapper. */
    using F80 = Floating<long double>;
}