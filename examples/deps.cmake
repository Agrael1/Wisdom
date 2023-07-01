if(NOT PLUGINS_LOADED)
    message("Loading WIL...")
    FetchContent_Declare(
      wil
      GIT_REPOSITORY https://github.com/microsoft/wil.git
      GIT_TAG origin/master
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
    )
    set(WIL_BUILD_PACKAGING OFF)
    set(WIL_BUILD_TESTS OFF)

    FetchContent_MakeAvailable(wil)
    

    set(wil_SOURCES ${wil_SOURCE_DIR} CACHE INTERNAL "")
endif()

add_library(wil INTERFACE)
target_include_directories(wil SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${wil_SOURCES}/include>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(wil)
