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
        constexpr OnCopy() noexcept = default;
    };

    template<>
    class OnCopy<copy::Disabled> {
    protected:
        constexpr OnCopy() noexcept = default;
        constexpr OnCopy(OnCopy&&) noexcept = default;
        constexpr OnCopy& operator=(OnCopy&&) noexcept = default;

    public:
        OnCopy(const OnCopy&) = delete;
        OnCopy& operator=(const OnCopy&) = delete;
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
        constexpr OnMove() noexcept = default;
    };

    template<>
    class OnMove<move::Disabled> {
    protected:
        constexpr OnMove() noexcept = default;
    public:
        constexpr OnMove(const OnMove&) noexcept = default;
        constexpr OnMove& operator=(const OnMove&) noexcept = default;

        constexpr OnMove(OnMove&&) noexcept = delete;
        constexpr OnMove& operator=(OnMove&&) noexcept = delete;
    };
}

namespace original::instantiate::details {
    struct InstantiatePolicyTag : original::details::traitPolicyTag {};

    template<typename T>
    concept InstantiatePolicy = std::is_base_of_v<InstantiatePolicyTag, T>;
}

export namespace original::instantiate {
    struct Enabled : details::InstantiatePolicyTag {};
    struct Disabled : details::InstantiatePolicyTag {};
}

export namespace original {
    template<instantiate::details::InstantiatePolicy = instantiate::Enabled>
    class OnInstantiate;

    template<>
    class OnInstantiate<instantiate::Enabled> {
    protected:
        constexpr OnInstantiate() noexcept = default;
    };

    template<>
    class OnInstantiate<instantiate::Disabled> {
    public:
        constexpr OnInstantiate() noexcept = delete;
    };
}

export namespace original {
    struct Regular : OnCopy<>, OnMove<> {
    protected:
        constexpr Regular() noexcept = default;
    };

    struct MoveOnly : OnCopy<copy::Disabled>, OnMove<> {
    protected:
        constexpr MoveOnly() noexcept = default;
    };

    struct CopyOnly : OnCopy<>, OnMove<move::Disabled> {
    protected:
        constexpr CopyOnly() noexcept = default;
    };

    struct Immobile : OnCopy<copy::Disabled>, OnMove<move::Disabled> {
    protected:
        constexpr Immobile() noexcept = default;
    };

    struct Static : Immobile, OnInstantiate<instantiate::Disabled> {};
}