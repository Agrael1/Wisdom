message("Loading WIL...")
load_nuget_dependency(${NUGET_EXE} "Microsoft.Windows.ImplementationLibrary" WIL ${CMAKE_CURRENT_BINARY_DIR})

set(wil_SOURCES ${WIL_DIR} CACHE INTERNAL "")

add_library(Wil INTERFACE)
target_include_directories(Wil SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${wil_SOURCES}/include>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(Wil)
