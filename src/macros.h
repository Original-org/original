#ifndef ORIGINAL_MACROS_H
#define ORIGINAL_MACROS_H

namespace original
{
    constexpr int ORIGINAL_HEADER_FILE_INCLUDE = 1; // Placeholder

    // Initialize all platform macros to false
    #define ORIGINAL_PLATFORM_WINDOWS 0
    #define ORIGINAL_PLATFORM_WINDOWS_32 0
    #define ORIGINAL_PLATFORM_WINDOWS_64 0
    #define ORIGINAL_PLATFORM_LINUX 0
    #define ORIGINAL_PLATFORM_MACOS 0
    #define ORIGINAL_PLATFORM_UNIX 0
    #define ORIGINAL_PLATFORM_UNKNOWN 0

    // Override based on actual platform detection
    #if defined(_WIN32) || defined(_WIN64)
    #undef ORIGINAL_PLATFORM_WINDOWS
    #define ORIGINAL_PLATFORM_WINDOWS 1
    #ifdef _WIN64
        #undef ORIGINAL_PLATFORM_WINDOWS_64
        #define ORIGINAL_PLATFORM_WINDOWS_64 1
    #else
        #undef ORIGINAL_PLATFORM_WINDOWS_32
        #define ORIGINAL_PLATFORM_WINDOWS_32 1
    #endif
    #elif defined(__linux__)
    #undef ORIGINAL_PLATFORM_LINUX
    #define ORIGINAL_PLATFORM_LINUX 1
    #elif defined(__APPLE__) && defined(__MACH__)
    #undef ORIGINAL_PLATFORM_MACOS
    #define ORIGINAL_PLATFORM_MACOS 1
    #elif defined(__unix__)
    #undef ORIGINAL_PLATFORM_UNIX
    #define ORIGINAL_PLATFORM_UNIX 1
    #else
    #undef ORIGINAL_PLATFORM_UNKNOWN
    #define ORIGINAL_PLATFORM_UNKNOWN 1
    #endif

    // Initialize all compiler macros to false
    #define ORIGINAL_COMPILER_CLANG 0
    #define ORIGINAL_COMPILER_GCC 0
    #define ORIGINAL_COMPILER_MSVC 0
    #define ORIGINAL_COMPILER_UNKNOWN 0

    // Override based on actual compiler detection
    #if defined(__clang__)
    #undef ORIGINAL_COMPILER_CLANG
    #define ORIGINAL_COMPILER_CLANG 1
    #define ORIGINAL_COMPILER_VERSION __clang_major__.__clang_minor__.__clang_patchlevel__
    #elif defined(__GNUC__) || defined(__GNUG__)
    #undef ORIGINAL_COMPILER_GCC
    #define ORIGINAL_COMPILER_GCC 1
    #define ORIGINAL_COMPILER_VERSION __GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__
    #elif defined(_MSC_VER)
    #undef ORIGINAL_COMPILER_MSVC
    #define ORIGINAL_COMPILER_MSVC 1
    #define ORIGINAL_COMPILER_VERSION _MSC_VER
    #else
    #undef ORIGINAL_COMPILER_UNKNOWN
    #define ORIGINAL_COMPILER_UNKNOWN 1
    #endif
}

#endif //ORIGINAL_MACROS_H