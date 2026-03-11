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
        constexpr OnMove() noexcept = default;
    };

    template<>
    class OnMove<move::Disabled> {
    protected:
        constexpr OnMove() noexcept = default;
    public:
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
    class Regular : public OnCopy<>, public OnMove<> {
    protected:
        constexpr Regular() noexcept = default;
    };

    class MoveOnly : public OnCopy<copy::Disabled>, public OnMove<> {
    protected:
        constexpr MoveOnly() noexcept = default;
    };

    class CopyOnly : public OnCopy<>, public OnMove<move::Disabled> {
    protected:
        constexpr CopyOnly() noexcept = default;
    };

    class Immobile : public OnCopy<copy::Disabled>, public OnMove<move::Disabled> {
    protected:
        constexpr Immobile() noexcept = default;
    };

    class Static : public Immobile, public OnInstantiate<instantiate::Disabled> {};
}