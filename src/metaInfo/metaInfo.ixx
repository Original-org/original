module;
export module original.metaInfo;
import :details;


export namespace original::info
{
    constexpr auto NAME = "Original";

    constexpr auto DESCRIPTION = "A C++ tool library";

    constexpr auto REPO_PAGE = "https://github.com/Original-org/original";

    constexpr auto LICENSE = "MIT license";

    constexpr auto ORGANIZATION = "Original-org";

    const auto VERSION = details::CURRENT_VERSION.str();
}