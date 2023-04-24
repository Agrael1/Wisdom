cmake_minimum_required (VERSION 3.22)

project(Wil)

if(NOT PLUGINS_LOADED)
    message("Loading WIL...")
    FetchContent_Declare(
      wil
      GIT_REPOSITORY https://github.com/microsoft/wil.git
      GIT_TAG origin/master
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
    )
    FetchContent_GetProperties(wil)
    if(NOT wil_POPULATED)
      set(WIL_BUILD_PACKAGING OFF)
      set(WIL_BUILD_TESTS OFF)
      FetchContent_Populate(wil)
    endif()

    set(wil_SOURCES ${wil_SOURCE_DIR} CACHE INTERNAL "")
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${wil_SOURCES}/include>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(${PROJECT_NAME})
