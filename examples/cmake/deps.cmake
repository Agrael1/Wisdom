# Examples can also be configured without the library's dependency setup.
find_package(SDL3 CONFIG QUIET)
if((NOT TARGET SDL3::SDL3 OR WISDOM_BUILD_VIDEO) AND NOT COMMAND CPMAddPackage)
  if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_BINARY_DIR}/_deps_cache")
  endif()
  include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)
endif()

if(NOT TARGET SDL3::SDL3)
  CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG preview-3.1.3
    OPTIONS "SDL_WERROR OFF")
endif()

if (WISDOM_BUILD_VIDEO)
    # h265nal sets GCC-specific -W flags in debug mode unconditionally,
    # which MSVC rejects (/Wextra -> D8021 invalid numeric argument).
    # Use DOWNLOAD_ONLY to fetch the source, patch it, then add_subdirectory manually.
    if(MSVC)
        CPMAddPackage(
            NAME h265nal
            GITHUB_REPOSITORY chemag/h265nal
            GIT_TAG master
            DOWNLOAD_ONLY YES
            OPTIONS
            "BUILD_H265_TESTS OFF"
        )

        # Patch GCC debug flags -> MSVC-compatible equivalents
        foreach(CMAKE_FILE
            "${h265nal_SOURCE_DIR}/CMakeLists.txt"
            "${h265nal_SOURCE_DIR}/src/CMakeLists.txt"
        )
            file(READ "${CMAKE_FILE}" _content)
            string(REPLACE
                "-g -O0 -Wall -Wextra -Wunused-parameter -Wshadow -Wformat -Wextra-semi -Wsign-conversion -Werror"
                "/Od /W3"
                _content "${_content}"
            )
            string(REPLACE
                "option(H265NAL_SMALL_FOOTPRINT, \"xmall footprint build\")"
                "option(H265NAL_SMALL_FOOTPRINT \"small footprint build\")"
                _content "${_content}"
            )
            file(WRITE "${CMAKE_FILE}" "${_content}")
        endforeach()

        set(BUILD_H265_TESTS OFF)
        add_subdirectory("${h265nal_SOURCE_DIR}" "${h265nal_BINARY_DIR}")
    else()
        CPMAddPackage(
            NAME h265nal
            GITHUB_REPOSITORY chemag/h265nal
            GIT_TAG master
            DOWNLOAD_ONLY YES
            OPTIONS
            "BUILD_H265_TESTS OFF"
        )

        # Patch debug flags (same as MSVC patch but for clang on Windows)
        foreach(CMAKE_FILE
            "${h265nal_SOURCE_DIR}/CMakeLists.txt"
            "${h265nal_SOURCE_DIR}/src/CMakeLists.txt"
        )
            file(READ "${CMAKE_FILE}" _content)
            string(REPLACE
                "-g -O0 -Wall -Wextra -Wunused-parameter -Wshadow -Wformat -Wextra-semi -Wsign-conversion -Werror"
                "-g -O0 -Wall -Wextra -Wunused-parameter -Wshadow -Wformat -Wextra-semi -Wsign-conversion -Werror -Wno-deprecated-declarations"
                _content "${_content}"
            )
            string(REPLACE
                "option(H265NAL_SMALL_FOOTPRINT, \"xmall footprint build\")"
                "option(H265NAL_SMALL_FOOTPRINT \"small footprint build\")"
                _content "${_content}"
            )
            file(WRITE "${CMAKE_FILE}" "${_content}")
        endforeach()

        set(BUILD_H265_TESTS OFF)
        add_subdirectory("${h265nal_SOURCE_DIR}" "${h265nal_BINARY_DIR}")
    endif()
endif()
