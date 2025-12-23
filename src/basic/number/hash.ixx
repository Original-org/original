module;
#include <cstddef>
#include <functional>
export module original.basic.number.hash;
import original.basic.number.traits;
import original.basic.number.casts;


export namespace std // NOLINT
{
    template<original::Number N>
    struct hash<N>
    {
        constexpr size_t operator()(N const& n) const noexcept
        {
            std::hash<original::NumberLikeType<N>> hash;
            return hash(original::numberLikeValue(n));
        }
    };
}