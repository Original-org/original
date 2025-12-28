module;
#include <type_traits>
export module original.basic.range.traits;
import original.basic.iterator.traits;
import original.basic.types;


export namespace original
{
    template<typename R>
    concept Range =
    requires(R& r)
    {
        typename RemoveCVRefType<R>::BeginIterType;
        typename RemoveCVRefType<R>::EndIterType;
    } &&
    (requires(R& r) {
        { r.begin() } -> Iterator;
        { r.end() };
    } ||
    requires(R& r) {
        { begin(r) } -> Iterator;
        { end(r) };
    });

    template<typename R>
    concept IterRange = Range<R> &&
    requires(R& r)
    {
        requires StdSame<typename RemoveCVRefType<R>::BeginIterType, typename RemoveCVRefType<R>::EndIterType>;
        { r.end() } -> StdSame<typename RemoveCVRefType<R>::EndIterType>;
    };

    template<Range R>
    struct RangeTraits
    {
        using BeginIterType = decltype(std::declval<R&>().begin());
        using EndIterType   = decltype(std::declval<R&>().end());
    };
}