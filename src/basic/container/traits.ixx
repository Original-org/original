module;
#include <type_traits>
export module original.basic.container.traits;
import original.basic.types;
import original.basic.iterator.traits;
import original.basic.number.traits;
import original.basic.number.casts;


export namespace original
{
    template<typename C>
    concept Container =
    IsObject<C> &&
    requires(C c) {
        typename C::IterType;
        typename C::ConstIterType;
        typename C::ValueType;
        typename C::SizeType;
        c.begin();
        c.end();
        c.size();
    };

    template<Container C>
    struct ContainerTraits
    {
        using IterType      = C::IterType;
        using ConstIterType = C::ConstIterType;
        using ValueType     = C::ValueType;
        using SizeType      = C::SizeType;

        static IterType begin(C& c) { return c.begin(); }
        static IterType end(C& c)   { return c.end(); }
        static ConstIterType cBegin(const C& c) { return c.begin(); }
        static ConstIterType cEnd(const C& c) { return c.end(); }
        static SizeType size(const C& c) { return c.size(); }
        static bool empty(const C& c) { return numberLikeValue(c.size()) == 0; }
    };

    template<typename C>
    concept ContiguousContainer = Container<C> &&
    requires(C container)
    {
        requires ContiguousIterator<typename ContainerTraits<C>::IterType>;
        requires ContiguousIterator<typename ContainerTraits<C>::ConstIterType>;

        { container.data() } -> SameType<std::add_pointer_t<typename ContainerTraits<C>::ValueType>>;
    };
}