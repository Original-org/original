#ifndef ORIGINAL_MACROS_H
#define ORIGINAL_MACROS_H

namespace original
{
    /**
    * @defgroup PlatformDetection Platform Detection Macros
    * @brief Macros for identifying the target platform
    * @{
    */

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
    /** @} */ // end of PlatformDetection group

    /**
    * @defgroup CompilerDetection Compiler Detection Macros
    * @brief Macros for identifying the compiler being used
    * @{
    */

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
    /** @} */ // end of CompilerDetection group

    /**
     * @defgroup PlatformDetectionFunctions Platform Detection Functions
     * @brief Compile-time functions for platform detection
     * @{
     */

    /**
     * @brief Checks if compiling for Windows platform
     * @return true if compiling for Windows, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_WINDOWS(){
        #if ORIGINAL_PLATFORM_WINDOWS
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for 32-bit Windows
     * @return true if compiling for Win32, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_WIN32(){
        #if ORIGINAL_PLATFORM_WINDOWS_32
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for 64-bit Windows
     * @return true if compiling for Win64, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_WIN64(){
        #if ORIGINAL_PLATFORM_WINDOWS_64
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for Linux platform
     * @return true if compiling for Linux, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_LINUX(){
        #if ORIGINAL_PLATFORM_LINUX
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for macOS platform
     * @return true if compiling for macOS, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_MACOS(){
        #if ORIGINAL_PLATFORM_MACOS
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for Unix-like platform (excluding Linux/macOS)
     * @return true if compiling for Unix, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_UNIX(){
        #if ORIGINAL_PLATFORM_UNIX
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling for unknown platform
     * @return true if platform detection failed, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool ON_UNKNOWN_PLATFORM(){
        #if ORIGINAL_PLATFORM_UNKNOWN
            return true;
        #else
            return false;
        #endif
    }
    /** @} */ // end of PlatformDetectionFunctions group

    /**
     * @defgroup CompilerDetectionFunctions Compiler Detection Functions
     * @brief Compile-time functions for compiler detection
     * @{
     */

    /**
     * @brief Checks if compiling with Clang
     * @return true if using Clang compiler, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool USING_CLANG(){
        #if ORIGINAL_COMPILER_CLANG
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling with GCC
     * @return true if using GCC compiler, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool USING_GCC(){
        #if ORIGINAL_COMPILER_GCC
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling with MSVC
     * @return true if using Microsoft Visual C++ compiler, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool USING_MSVC(){
        #if ORIGINAL_COMPILER_MSVC
            return true;
        #else
            return false;
        #endif
    }

    /**
     * @brief Checks if compiling with unknown compiler
     * @return true if compiler detection failed, false otherwise
     * @note Evaluated at compile-time
     */
    consteval bool USING_UNKNOWN_COMPLIER(){
        #if ORIGINAL_COMPILER_UNKNOWN
            return true;
        #else
            return false;
        #endif
    }
    /** @} */ // end of CompilerDetectionFunctions group
}

#endif //ORIGINAL_MACROS_H