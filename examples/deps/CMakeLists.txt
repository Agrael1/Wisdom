include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(WISDOM_WINDOWS)
  find_program(
    NUGET_EXE
    NAMES nuget
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/NuGet)
  if(NOT NUGET_EXE)
    message("NUGET.EXE not found.")
    message(FATAL_ERROR "Please install this executable, and run CMake again.")
  endif()

  # Windows Implementation Library (WIL)
  message("Loading WIL...")
  load_nuget_dependency(${NUGET_EXE} "Microsoft.Windows.ImplementationLibrary"
                        WIL ${CMAKE_CURRENT_BINARY_DIR})

  set(wil_SOURCES
      ${WIL_DIR}
      CACHE INTERNAL "")
  add_library(Wil INTERFACE)
  target_include_directories(
    Wil SYSTEM BEFORE INTERFACE $<BUILD_INTERFACE:${wil_SOURCES}/include>
                                $<INSTALL_INTERFACE:include>)
endif()

# glm
find_package(glm QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG master)
  FetchContent_GetProperties(glm)
  if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    # Disable tinyxml2 installation
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
else()

endif()

# KDAB KDUtils
if(NOT WISDOM_WINDOWS_STORE AND NOT WISDOM_EXCLUDE_KDGUI)
  set(BUILD_SHARED_LIBS OFF)
  set(KDUTILS_BUILD_TESTS OFF)

  find_package(KDUtils CONFIG QUIET)
  if(NOT KDUtils_FOUND)
    FetchContent_Declare(
      KDUtils
      GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
      GIT_TAG main)

    set(SPDLOG_FMT_EXTERNAL_HO
        ON
        CACHE BOOL "" FORCE)
    FetchContent_GetProperties(KDUtils)
    if(NOT KDUtils_POPULATED)
      FetchContent_Populate(KDUtils)
      # Disable tinyxml2 installation
      add_subdirectory(${kdutils_SOURCE_DIR} ${kdutils_BINARY_DIR}
                       EXCLUDE_FROM_ALL)
    endif()
  endif()
endif()
