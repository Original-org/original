module;
#include <type_traits>

export module original.basic.types.invocable;

import original.basic.types.core;
import original.basic.types.comparison;

export namespace original {
template <typename T>
concept Invokable = requires {
  std::is_function_v<std::remove_pointer_t<T>> || requires(T t) { t(); };
};

template <typename T, typename... Args>
concept InvokableWith = Invokable<T> && std::is_invocable_v<T, Args...>;

template <typename T, typename... Args>
using InvokeResultType = std::invoke_result_t<T, Args...>;

template <typename T, typename R, typename... Args>
concept InvokableReturns =
    InvokableWith<T, Args...> && SameType<InvokeResultType<T, Args...>, R>;

template <typename T, typename R, typename... Args>
concept InvokableReturnsConvertible =
    InvokableWith<T, Args...> && Convertible<InvokeResultType<T, Args...>, R>;

template <typename F, typename... Args>
concept Predicate = InvokableWith<F, Args...> &&
                    (Convertible<InvokeResultType<F, Args...>, bool> ||
                     StdThreeWayCompareResult<InvokeResultType<F, Args...>>);

template <typename F, typename Arg>
concept UnaryPredicate = Predicate<F, Arg>;

template <typename F, typename Arg1, typename Arg2>
concept BinaryPredicate = Predicate<F, Arg1, Arg2>;

template <typename F>
concept Functor = Invokable<F> && std::is_class_v<std::remove_reference_t<F>>;
} // namespace original
