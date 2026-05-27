# SDL3
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG preview-3.1.3
  OPTIONS
  "SDL_WERROR OFF"
)

if (WISDOM_BUILD_VIDEO)
    # mBMFF
    CPMAddPackage(
        NAME mBMFF
        GITHUB_REPOSITORY Agrael1/mbmff
        GIT_TAG master
    )
endif()
