module;
#include <format>
#include <string>
module original.metaInfo:details;


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
