module;
export module orginal.basic.container.traits;
import original.basic.types;
import original.basic.iterator.traits;
import original.basic.number.traits;


namespace original::details
{
    template<typename T>
    concept HasSize =
    requires(const T& t) {
        { t.size() } -> UnsignedIntegralLike;
    } ||
    requires(const T& t) {
        { size(t) } -> UnsignedIntegralLike;
    };
}

export namespace original
{
    template<typename>
    struct ContainerTraits;

    template<StdObject C>
    requires HasIterator<C> && IterRange<C> && details::HasSize<C>
    struct ContainerTraits<C>
    {
        using IterType      = C::IterType;
        using ConstIterType = C::ConstIterType;
        using ValueType     = C::ValueType;
        using SizeType      = C::SizeType;

        static IterType begin(C& c) { return c.begin(); }
        static IterType end(C& c)   { return c.end(); }
        static SizeType size(const C& c) { return c.size(); }
        static bool empty(const C& c) { return numberLikeValue(c.size()) == 0; }
    };

    template<StdObject C>
    concept Container =
    requires(C container)
    {
        typename ContainerTraits<C>;
    };

    template<StdObject C>
    concept ContiguousContainer = Container<C> &&
    requires(C container)
    {
        requires ContiguousIterator<typename ContainerTraits<C>::IterType>;
        requires ContiguousIterator<typename ContainerTraits<C>::ConstIterType>;

        { container.data() } -> StdSame<ContainerTraits<C>::ValueType*>;
    };
}