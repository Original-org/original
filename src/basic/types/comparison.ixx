module;
#include <compare>
#include <concepts>
export module original.basic.types.comparison;

import original.basic.types.core;

export namespace original {
template <typename T>
concept StdThreeWayCompareResult =
    SameType<T, std::strong_ordering> || SameType<T, std::weak_ordering> ||
    SameType<T, std::partial_ordering>;

template <typename... Ts>
using CommonComparisonCategory = std::common_comparison_category_t<Ts...>;

template <typename T>
concept EqualityComparable = std::equality_comparable<T>;

template <typename T>
concept LessComparable = requires(T t1, T t2) {
  { t1 < t2 } -> Convertible<bool>;
};

template <typename T>
concept WeakLessComparable = LessComparable<T> && EqualityComparable<T>;

template <typename T>
concept GreaterComparable = requires(T t1, T t2) {
  { t1 > t2 } -> Convertible<bool>;
};

template <typename T>
concept WeakGreaterComparable = GreaterComparable<T> && EqualityComparable<T>;

template <typename T>
concept ThreeWayComparable = std::three_way_comparable<T>;

template <typename T>
concept Comparable = EqualityComparable<T> && ThreeWayComparable<T>;
} // namespace original
