# SDL3
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG preview-3.1.3
  OPTIONS
  "SDL_WERROR OFF"
)

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
            OPTIONS
            "BUILD_H265_TESTS OFF"
        )
    endif()
endif()
