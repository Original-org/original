module;
#include <type_traits>
export module original.basic.types.traitPolicy;


namespace original::details {
    struct traitPolicyTag {};
}

namespace original::copy::details {
    struct CopyPolicyTag : original::details::traitPolicyTag {};

    template<typename T>
    concept CopyPolicy = std::is_base_of_v<CopyPolicyTag, T>;
}

export namespace original::copy {
    struct Enabled : details::CopyPolicyTag {};
    struct Disabled : details::CopyPolicyTag {};
}

export namespace original {
    template<copy::details::CopyPolicy = copy::Enabled>
    class OnCopy;

    template<>
    class OnCopy<copy::Enabled> {
    protected:
        constexpr explicit OnCopy() noexcept = default;
        constexpr OnCopy(const OnCopy&) noexcept = default;
        constexpr OnCopy& operator=(const OnCopy&) noexcept = default;
    };

    template<>
    class OnCopy<copy::Disabled> {
    protected:
        constexpr explicit OnCopy() noexcept = default;
    public:
        constexpr OnCopy(const OnCopy&) noexcept = delete;
        constexpr OnCopy& operator=(const OnCopy&) noexcept = delete;
    };
}

namespace original::move::details {
    struct MovePolicyTag : original::details::traitPolicyTag {};

    template<typename T>
    concept MovePolicy = std::is_base_of_v<MovePolicyTag, T>;
}

export namespace original::move {
    struct Enabled : details::MovePolicyTag {};
    struct Disabled : details::MovePolicyTag {};
}

export namespace original {
    template<move::details::MovePolicy = move::Enabled>
    class OnMove;

    template<>
    class OnMove<move::Enabled> {
    protected:
        constexpr explicit OnMove() noexcept = default;
        constexpr OnMove(OnMove&&) noexcept = default;
        constexpr OnMove& operator=(OnMove&&) noexcept = default;
    };

    template<>
    class OnMove<move::Disabled> {
    protected:
        constexpr explicit OnMove() noexcept = default;
    public:
        constexpr OnMove(OnMove&&) noexcept = delete;
        constexpr OnMove& operator=(OnMove&&) noexcept = delete;
    };
}

export namespace original {
    class [[nodiscard]] Trivial : OnCopy<>, OnMove<> {};

    class [[nodiscard]] MoveOnly : OnCopy<copy::Disabled>, OnMove<> {};

    class [[nodiscard]] CopyOnly : OnCopy<>, OnMove<move::Disabled> {};

    class [[nodiscard]] Immobile : OnCopy<copy::Disabled>, OnMove<move::Disabled> {};
}