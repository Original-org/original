module;
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
            return static_cast<size_t>(original::numberLikeValue(n));
        }
    };
}