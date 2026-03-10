module;
export module original.basic.types.incrementable;

import original.basic.types.core;
import original.basic.types.comparison;

export namespace original {
template <typename T>
concept Incrementable = requires(T a, T b) {
  { ++a } -> SameType<T &>;
};

template <typename T>
concept Decrementable = requires(T a, T b) {
  { --a } -> SameType<T &>;
};

template <typename T>
concept IncrementableComparable =
    IsObject<T> && CopyConstructible<T> && MoveConstructible<T> &&
    Incrementable<T> && WeakLessComparable<T>;
} // namespace original
