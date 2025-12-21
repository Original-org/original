module;
#include <format>
#include <string>
export module original.metaInfo;


export namespace original::info
{
    struct Version
    {
        const int major;
        const int minor;
        const int patch;

        constexpr Version(const int major, const int minor, const int patch)
            : major(major), minor(minor), patch(patch) {}

        constexpr auto operator<=>(const Version&) const noexcept = default;

        std::string str() const noexcept
        {
            return std::format("{}.{}.{}", this->major, this->minor, this->patch);
        }
    };
}

export namespace original::info
{
    constexpr auto NAME = "Original";

    constexpr auto DESCRIPTION = "A C++ tool library";

    constexpr auto REPO_PAGE = "https://github.com/Original-org/original";

    constexpr auto LICENSE = "MIT license";

    constexpr auto ORGANIZATION = "Original-org";

    constexpr auto VERSION = Version{0, 1, 0};
}

export namespace original::info
{
    constexpr bool atLeastCurrentVersion(const Version version)
    {
        return version >= VERSION;
    }
}