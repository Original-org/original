module;
#include <type_traits>
export module original.basic.types.ownership;


namespace original::details {
    struct OwnershipTagBase {};

    template<typename T>
    concept OwnershipTag = std::is_base_of_v<OwnershipTagBase, T>;
}

export namespace original {
    struct DefaultOwnerShip : details::OwnershipTagBase {};

    struct MoveOnlyOwnerShip : details::OwnershipTagBase {};

    struct NoMoveOwnerShip : details::OwnershipTagBase {};

    template<details::OwnershipTag = DefaultOwnerShip>
    class Ownership {
        static_assert(false, "");
    };

    template<>
    class Ownership<DefaultOwnerShip> {
    protected:
        constexpr explicit Ownership() = default;
        constexpr Ownership(const Ownership&) = default;
        constexpr Ownership& operator=(const Ownership&) = default;
        constexpr Ownership(Ownership&&) = default;
        constexpr Ownership& operator=(Ownership&&) = default;
    };

    template<>
    class Ownership<MoveOnlyOwnerShip> {
    protected:
        constexpr explicit Ownership() = default;
        constexpr Ownership(Ownership&&) = default;
        constexpr Ownership& operator=(Ownership&&) = default;
    public:
        constexpr Ownership(const Ownership&) = delete;
        constexpr Ownership& operator=(const Ownership&) = delete;
    };

    template<>
    class Ownership<NoMoveOwnerShip> {
    protected:
        constexpr explicit Ownership() = default;
    public:
        constexpr Ownership(const Ownership&) = delete;
        constexpr Ownership& operator=(const Ownership&) = delete;
        constexpr Ownership(Ownership&&) = delete;
        constexpr Ownership& operator=(Ownership&&) = delete;
    };
}