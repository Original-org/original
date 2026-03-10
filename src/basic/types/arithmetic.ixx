module;
#include <type_traits>
export module original.basic.types.arithmetic;

import original.basic.types.core;

export namespace original {
template <typename T>
concept StdArithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept StdIntegral = std::is_integral_v<T>;

template <typename T>
concept StdSignedIntegral = StdIntegral<T> && std::is_signed_v<T>;

template <typename T>
concept StdUnsignedIntegral = StdIntegral<T> && std::is_unsigned_v<T>;

template <typename T, typename U>
concept StdSameSignIntegral =
    (StdSignedIntegral<T> && StdSignedIntegral<U>) ||
    (StdUnsignedIntegral<T> && StdUnsignedIntegral<U>);

template <typename T>
concept StdFloating = std::is_floating_point_v<T>;
} // namespace original
