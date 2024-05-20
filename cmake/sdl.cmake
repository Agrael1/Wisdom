cmake_minimum_required(VERSION 3.24)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

find_package(SDL2 CONFIG)

if(NOT SDL2_FOUND)
  FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
  FetchContent_MakeAvailable(SDL2)
endif()
