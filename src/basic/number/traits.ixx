module;
#include <type_traits>
export module original.basic.number.traits;
import original.basic.types;
import original.basic.number.numberImpl;


export namespace original
{
    template<typename>
    struct NumberTrait : std::false_type {};

    template<StdIntegral T>
    struct NumberTrait<Integer<T>> : std::true_type
    {
        using Type = T;
    };

    template<StdFloating T>
    struct NumberTrait<Floating<T>> : std::true_type
    {
        using Type = T;
    };

    template<typename T>
    constexpr bool IsNumber = NumberTrait<T>::value;

    template<typename T>
    constexpr bool IsNumberLike = IsNumber<T> || IsStdArithmetic<T>;

    template<typename T>
    concept Number = IsNumber<T>;

    template<typename T>
    concept NumberLike = IsNumberLike<T>;

    template<Number T>
    using NumberType = NumberTrait<T>::Type;

    template<typename>
    struct NumberLikeTrait {};

    template<Number T>
    struct NumberLikeTrait<T>
    {
        using Type = NumberType<T>;
    };

    template<StdArithmetic T>
    struct NumberLikeTrait<T>
    {
        using Type = T;
    };

    template<NumberLike T>
    using NumberLikeType = NumberLikeTrait<T>::Type;
}
