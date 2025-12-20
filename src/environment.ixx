module;
#include "macros.h"
export module original.environment;


namespace original::details
{
    constexpr int USING_ORIGINAL_HEADER_FILE = ORIGINAL_HEADER_FILE_INCLUDE; // Placeholder
}

export namespace original
{
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
}
