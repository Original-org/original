# CMakeLists.txt Main

set(CMAKE_VERSION_MINIMUM 3.31)
cmake_minimum_required(VERSION ${CMAKE_VERSION_MINIMUM})
message(STATUS "Cmake version: ${CMAKE_VERSION} (>= ${CMAKE_VERSION_MINIMUM} ✓)")

project(original LANGUAGES CXX)

set(ORIGINAL_VERSION 0.1.5)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GCC_VERSION_MINIMUM 14.0)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${COMPILER_GCC_VERSION_MINIMUM})
        message(FATAL_ERROR "GCC version too old. Minimum required: ${COMPILER_GCC_VERSION_MINIMUM}")
    else()
        message(STATUS "GCC version: ${CMAKE_CXX_COMPILER_VERSION} (>= ${COMPILER_GCC_VERSION_MINIMUM} ✓)")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_CLANG_VERSION_MINIMUM 17.0)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${COMPILER_CLANG_VERSION_MINIMUM})
        message(FATAL_ERROR "Clang version too old. Minimum required: ${COMPILER_CLANG_VERSION_MINIMUM}")
    else()
        message(STATUS "Clang version: ${CMAKE_CXX_COMPILER_VERSION} (>= ${COMPILER_CLANG_VERSION_MINIMUM} ✓)")
    endif()
elseif (MSVC)
    set(COMPILER_MSVC_VERSION_MINIMUM 1944)
    if (MSVC_VERSION LESS ${COMPILER_MSVC_VERSION_MINIMUM})
        message(FATAL_ERROR "MSVC version too old. Minimum required: ${COMPILER_MSVC_VERSION_MINIMUM}")
    else()
        message(STATUS "MSVC version: ${MSVC_VERSION} (>= ${COMPILER_MSVC_VERSION_MINIMUM} ✓)")
    endif()
endif()

set(ORIGINAL_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_library(original STATIC)

target_sources(original
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        src/original.ixx
        src/basic/metaInfo.ixx
        src/basic/basic.ixx
        src/basic/types/types.ixx
        src/basic/number/number.ixx
        src/basic/number/traits.ixx
        src/basic/number/numberImpl.ixx
        src/basic/number/algorithms.ixx
        src/basic/number/casts.ixx
        src/basic/iterator/iterator.ixx
        src/basic/iterator/traits.ixx
        src/basic/iterator/iteratorImpl.ixx
        src/basic/iterator/adaptor.ixx
        PRIVATE
        src/macros.h
        src/original.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(original PUBLIC -fmodules-ts)
endif()
set_target_properties(original PROPERTIES
        CXX_SCAN_FOR_MODULES ON
        CXX_MODULE_DIRECTORY "${CMAKE_BINARY_DIR}/modules"
)
target_link_libraries(original PUBLIC Threads::Threads)

target_include_directories(original PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS original
        EXPORT originalTargets
        FILE_SET cxx_modules DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT originalTargets
        FILE originalTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/original
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/originalConfig.cmake.in"
        "${CMAKE_BINARY_DIR}/originalConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/original
)

write_basic_package_version_file(
        "${CMAKE_BINARY_DIR}/originalConfigVersion.cmake"
        VERSION ${ORIGINAL_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        "${CMAKE_BINARY_DIR}/originalConfig.cmake"
        "${CMAKE_BINARY_DIR}/originalConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/original
)

option(BUILD_TESTING "Build the testing directories" ON)

if (BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif ()
