module;
#include <type_traits>
export module original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.types;


export namespace original
{
    template<typename R>
    concept Range =
    requires(R& r) {
        { r.begin() } -> Iterator;
        { r.end() };
    } ||
    requires(R& r) {
        { begin(r) } -> Iterator;
        { end(r) };
    };

    template<typename R>
    concept IterRange = Range<R> &&
    requires(R& r)
    {
        { r.end() } -> SameType<decltype(std::declval<R&>().begin())>;
    };

    template<typename R>
    struct RangeTraits;

    template<Range R>
    struct RangeTraits<R>
    {
        using BeginIterType = decltype(std::declval<R&>().begin());
        using EndIterType = decltype(std::declval<R&>().end());
    };

    template<Range R>
    struct RangeTraits<const R>
    {
        using BeginIterType = decltype(std::declval<const R&>().begin());
        using EndIterType = decltype(std::declval<const R&>().end());
    };
}