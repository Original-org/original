module;
#include <type_traits>
#include <tuple>
export module original.basic.structural.traits;
import original.basic.number;
import original.basic.types;
import original.basic.structural.utility;


namespace original::details
{
    using std::get;

    template<typename T, Size::Type I, Size::Type N>
    struct StructuralElementCheck
    {
        static constexpr bool value =
        requires
        {
            typename std::tuple_element<I, std::remove_cvref_t<T>>::type; // NOLINT
        }
        && std::tuple_size<std::remove_cvref_t<T>>::value >= 0 // NOLINT
        && StructuralElementCheck<T, I + 1, N>::value;
    };

    template<typename T, Size::Type N>
    struct StructuralElementCheck<T, N, N>
        : std::true_type {};

    template<typename T, Size::Type N>
    inline constexpr bool HasStructuralElementsValue =
        StructuralElementCheck<T, 0, N>::value;

    template<typename T, Size::Type I, Size::Type N>
    struct StructuralGetCheck
    {
        static constexpr bool value =
        requires(T& t) { get<I>(t); } &&
        requires(const T& t) { get<I>(t); } &&
        (std::is_move_constructible_v<T> ?
        requires(T&& t) { get<I>(std::move(t)); } : true) &&
        StructuralGetCheck<T, I + 1, N>::value;
    };

    template<typename T, Size::Type N>
    struct StructuralGetCheck<T, N, N>
        : std::true_type {};

    template<typename T, Size::Type N>
    inline constexpr bool HasStructuralGetValue =
        StructuralGetCheck<T, 0, N>::value;
}

export namespace original
{
    using std::get;

    template<typename T>
    concept HasStructuralSize =
    requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    template<typename T>
    concept HasStructuralElements =
    HasStructuralSize<T> &&
    details::HasStructuralElementsValue<
        T,
        std::tuple_size<std::remove_cvref_t<T>>::value // NOLINT
    >;

    template<typename T>
    concept Structural =
    HasStructuralElements<T> &&
    details::HasStructuralGetValue<
        T,
        std::tuple_size<std::remove_cvref_t<T>>::value // NOLINT
    >;

    template<typename T>
    concept CoupleLike =
    Structural<T> &&
    std::tuple_size<std::remove_cvref_t<T>>::value == 2; // NOLINT

    template<typename T>
    concept TupleLike =
    Structural<T> &&
    std::tuple_size<std::remove_cvref_t<T>>::value >= 1; // NOLINT

    template<Structural T>
    struct StructuralTraits
    {
        using Type = std::remove_cvref_t<T>;
        template<Size::Type I>
        using ElementType = std::tuple_element<I, Type>::type; // NOLINT
        static constexpr Size::Type SIZE = std::tuple_size<Type>::value; // NOLINT
    };

    template<Structural T, Invokable F>
    constexpr void
    forEach(T&& t, F&& f)
    {
        forEach<StructuralTraits<T>::SIZE>
        (
            [&]<Size::Type I>(IndexConstant<I>)
            {
                f(get<I>(std::forward<T>(t)));
            }
        );
    }

    template<Structural T, Invokable F>
    constexpr decltype(auto)
    forAll(T&& t, F&& f)
    {
        return forAll<StructuralTraits<T>::SIZE>
        (
            [&]<Size::Type... I>(IndexConstant<I>...)
            {
                return f(get<I>(std::forward<T>(t))...);
            }
        );
    }
}