module;
#include <format>
#include <string>
export module original.metaInfo;


namespace original::details
{
    struct Version
    {
        const int major;
        const int minor;
        const int patch;

        constexpr std::string str() const noexcept
        {
            return std::format("{}.{}.{}", this->major, this->minor, this->patch);
        }
    };

    constexpr Version CURRENT_VERSION{0, 1, 0};
}

export namespace original::info
{
    constexpr auto NAME = "Original";

    constexpr auto DESCRIPTION = "A C++ tool library";

    constexpr auto REPO_PAGE = "https://github.com/Original-org/original";

    constexpr auto LICENSE = "MIT license";

    constexpr auto ORGANIZATION = "Original-org";

    const auto VERSION = details::CURRENT_VERSION.str();
}