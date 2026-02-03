cmake_minimum_required(VERSION 3.20)

project(BinaryCodeGenMat VERSION 1.0.0 LANGUAGES CXX)

option(BCGM_BUILD_TESTS "Build unit tests" OFF)
option(BCGM_BUILD_EXAMPLES "Build examples" OFF)

# --- Dependency: BinaryCodeWord ---
include(FetchContent)

set(BCW_GIT_REPOSITORY "https://github.com/coding-theory-research/BinaryCodeWord.git" CACHE STRING "Git URL for BinaryCodeWord (required if not provided by parent)")
set(BCW_GIT_TAG "v1.4.0" CACHE STRING "Git tag for BinaryCodeWord")

if(NOT TARGET BinaryCodeWord::BinaryCodeWord)
    if(BCW_GIT_REPOSITORY STREQUAL "")
        message(FATAL_ERROR
            "BinaryCodeWord::BinaryCodeWord target not found. "
            "Provide BinaryCodeWord as a dependency OR set -DBCW_GIT_REPOSITORY=<git-url>."
        )
    endif()

    FetchContent_Declare(
        BinaryCodeWord
        GIT_REPOSITORY ${BCW_GIT_REPOSITORY}
        GIT_TAG ${BCW_GIT_TAG}
    )

    set(BCW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BCW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(BinaryCodeWord)
endif()

add_library(BinaryCodeGenMat
    src/BinaryCodeGenMat.cpp
)
add_library(BinaryCodeGenMat::BinaryCodeGenMat ALIAS BinaryCodeGenMat)

target_compile_features(BinaryCodeGenMat PUBLIC cxx_std_20)

target_include_directories(BinaryCodeGenMat
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(BinaryCodeGenMat PUBLIC BinaryCodeWord::BinaryCodeWord)

if (MSVC)
    target_compile_options(BinaryCodeGenMat PRIVATE /W4)
else()
    target_compile_options(BinaryCodeGenMat PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BCGM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BCGM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---- Optional: install + package config (for find_package) ----
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS BinaryCodeGenMat
    EXPORT BinaryCodeGenMatTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT BinaryCodeGenMatTargets
    FILE BinaryCodeGenMatTargets.cmake
    NAMESPACE BinaryCodeGenMat::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BinaryCodeGenMat
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/BinaryCodeGenMatConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BinaryCodeGenMatConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BinaryCodeGenMat
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BinaryCodeGenMatConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/BinaryCodeGenMatConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/BinaryCodeGenMatConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BinaryCodeGenMat
)
