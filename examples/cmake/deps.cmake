# SDL3
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG preview-3.1.3
  OPTIONS
  "SDL_WERROR OFF"
)

if (WISDOM_BUILD_VIDEO)
    # libavif
    CPMAddPackage(
      NAME libavif
      GITHUB_REPOSITORY AOMediaCodec/libavif
      GIT_TAG v1.4.1
      OPTIONS
      "AVIF_CODEC_AOM OFF"
      "AVIF_CODEC_RAV1E OFF"
      "AVIF_CODEC_SVT OFF"
      "AVIF_CODEC_DAV1D OFF"
      "AVIF_CODEC_LIBGAV1 OFF"
      "AVIF_CODEC_AVM OFF"
      "AVIF_BUILD_APPS OFF"
      "AVIF_BUILD_TESTS OFF"
      "AVIF_LIBYUV OFF"
      "BUILD_SHARED_LIBS OFF"
    )
endif()
