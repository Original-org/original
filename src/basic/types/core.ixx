module;
#include <concepts>
#include <type_traits>
#include <utility>

export module original.basic.types.core;

namespace original::details {
template <typename... Args>
struct HasCommonTypeTrait : std::common_type<Args...> {};

template <typename... Args>
inline constexpr bool HasCommonTypeTraitVal =
    requires { typename HasCommonTypeTrait<Args...>::type; };

template <typename... Args>
struct HasCommonRefTrait : std::common_reference<Args...> {};

template <typename... Args>
inline constexpr bool HasCommonRefTraitVal =
    requires { typename HasCommonRefTrait<Args...>::type; };
} // namespace original::details

export namespace original {
template <typename T>
concept IsObject = std::is_object_v<T>;

template <typename T>
concept IsEmpty = std::is_empty_v<T>;

template <typename T>
concept Dereferenceable = requires(T t) { *t; };

template <typename T>
concept IsPointer = std::is_pointer_v<T>;

template <typename T>
concept IsBuiltinArray = std::is_array_v<std::remove_cvref_t<T>>;

template <typename T>
concept IsReference = std::is_reference_v<T>;

template <typename T>
concept IsLValueReference = std::is_lvalue_reference_v<T>;

template <typename T>
concept IsRValueReference = std::is_rvalue_reference_v<T>;

template <typename From, typename To>
concept Convertible = std::is_convertible_v<From, To>;

template <typename T, typename U>
concept SameType = std::same_as<T, U>;

template <typename T> using RemoveReference = std::remove_reference_t<T>;

template <typename T> using RemoveCVRefType = std::remove_cvref_t<T>;

template <bool Test, typename T>
using MaybeConstType = std::conditional_t<Test, const T, T>;

template <typename... Args>
concept HasCommonType = details::HasCommonTypeTraitVal<Args...>;

template <typename... Args>
concept HasCommonRef = HasCommonType<Args...>;

template <typename... Args> using CommonType = std::common_type_t<Args...>;

template <typename... Args>
using CommonRefType = std::common_reference_t<Args...>;

template <typename T, typename... Args>
concept Constructible = std::is_constructible_v<T, Args...>;

template <typename T>
concept CopyConstructible = std::is_copy_constructible_v<T>;

template <typename T>
concept MoveConstructible = std::is_move_constructible_v<T>;

template <typename T>
concept CopyAssignable = std::is_copy_assignable_v<T>;

template <typename T>
concept MoveAssignable = std::is_move_assignable_v<T>;

template <typename T, typename U>
concept AssignableFrom = requires(T t, U &&u) {
  { t = std::forward<U>(u) } -> std::same_as<T &>;
};
} // namespace original
