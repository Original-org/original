module;
#include <format>
#include <string>
export module original.metaInfo;


export namespace original::info
{
    /**
     * @brief Version information structure.
     *
     * Represents a semantic version (major.minor.patch) following semantic versioning rules.
     * Provides comparison operators and string conversion capabilities.
     */
    struct Version
    {
        const int major;  ///< Major version number (incompatible API changes).
        const int minor;  ///< Minor version number (backwards-compatible functionality additions).
        const int patch;  ///< Patch version number (backwards-compatible bug fixes).

        /**
         * @brief Construct a Version object.
         * @param major Major version number.
         * @param minor Minor version number.
         * @param patch Patch version number.
         */
        constexpr Version(const int major, const int minor, const int patch)
            : major(major), minor(minor), patch(patch) {}

        /** @brief Three-way comparison operator for Version objects. */
        constexpr auto operator<=>(const Version&) const noexcept = default;

        /**
         * @brief Convert version to string representation.
         * @return Formatted version string in "major.minor.patch" format.
         * @note Uses std::format for string generation.
         */
        [[nodiscard]] std::string str() const noexcept
        {
            return std::format("{}.{}.{}", this->major, this->minor, this->patch);
        }
    };
}

export namespace original::info
{
    /** @brief Library name constant. */
    constexpr auto NAME = "Original";

    /** @brief Brief description of the library. */
    constexpr auto DESCRIPTION = "A C++ tool library";

    /** @brief Repository URL for the library. */
    constexpr auto REPO_PAGE = "https://github.com/Original-org/original";

    /** @brief Software license under which the library is distributed. */
    constexpr auto LICENSE = "MIT license";

    /** @brief Organization name. */
    constexpr auto ORGANIZATION = "Original-org";

    /** @brief Current library version. */
    constexpr auto VERSION = Version{0, 1, 1};
}

export namespace original::info
{
    /**
     * @brief Check if a given version is at least the current library version.
     * @param version Version to compare against current library version.
     * @return True if the given version is greater than or equal to current version.
     *
     * @note Useful for feature detection or compatibility checks when using
     *       library components that require specific minimum versions.
     *
     * @example
     * @code
     * using original::info::VERSION;
     * constexpr original::info::Version v1{0, 2, 0};
     * static_assert(!atLeastCurrentVersion(v1)); // v1 > current version (0.1.0)
     *
     * constexpr original::info::Version v2{0, 0, 5};
     * static_assert(atLeastCurrentVersion(v2)); // v2 < current version (0.1.0)
     * @endcode
     */
    constexpr bool atLeastCurrentVersion(const Version version)
    {
        return version >= VERSION;
    }
}