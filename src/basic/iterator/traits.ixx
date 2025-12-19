module;
#include <concepts>
#include <compare>
export module original.basic.iterator.traits;
import original.basic.types;
import original.basic.number;


export namespace original
{
    template<typename T>
    concept Iterator =
        std::copy_constructible<T> &&
        Dereferenceable<T> &&
        requires(T it)
    {
        { *it } -> std::same_as<decltype(*std::declval<T>())>;
        { it == it } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept ForwardIterator = Iterator<T> && requires(T it)
    {
        { ++it } -> std::same_as<T&>;
        { it++ } -> std::same_as<T>;
        requires std::default_initializable<T>;
    };

    template<typename T>
    concept BidirectionalIterator = ForwardIterator<T> && requires(T it)
    {
        { --it } -> std::same_as<T&>;
        { it-- } -> std::same_as<T>;
    };

    template<typename T, typename DistanceType>
    concept RandomAccessIterator
        = BidirectionalIterator<T> &&
          SignedIntegralLike<DistanceType> &&
          requires(T it, DistanceType dis)
    {
        { it + dis } -> std::same_as<T>;
        { dis + it } -> std::same_as<T>;
        { it - dis } -> std::same_as<T>;
        { it - it } -> std::same_as<DistanceType>;
        { it += dis } -> std::same_as<T&>;
        { it -= dis } -> std::same_as<T&>;
        { it[dis] } -> std::same_as<decltype(*it)>;
        { it <=> it } -> std::three_way_comparable;
    };
}
