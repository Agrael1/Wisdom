
if(NOT wil_SOURCES)
    message("Loading WIL...")
    load_nuget_dependency(${NUGET_EXE} "Microsoft.Windows.ImplementationLibrary" WIL ${CMAKE_CURRENT_BINARY_DIR})
    add_library(Wil INTERFACE)
    target_install_interface(Wil)
endif()

set(wil_SOURCES ${WIL_DIR} CACHE INTERNAL "")
target_include_directories(Wil SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${wil_SOURCES}/include>
        $<INSTALL_INTERFACE:include>
)

