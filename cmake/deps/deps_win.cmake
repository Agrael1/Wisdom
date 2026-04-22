if (WISDOM_USE_AGILITY_SDK)
    wis_load_agility_sdk()

    # Create helpers library
    add_library(DX12Helpers INTERFACE)
    add_library(wis::DX12Helpers ALIAS DX12Helpers)

    target_link_libraries(DX12Helpers INTERFACE
            DX12Agility)
    install(
        TARGETS DX12Helpers
        EXPORT wisdom-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
else()
    message("DirectX 12 Agility SDK not enabled. Using Headers instead.")

    # Guaranteed backwards compatibility. 
    # Using origin/main to ensure we get the latest headers, 
    # which are compatible with the latest SDKs.
    CPMAddPackage(
            NAME dxheaders
            GITHUB_REPOSITORY microsoft/DirectX-Headers
            GIT_TAG origin/main
    )

    # Create helpers library
    add_library(DX12Helpers INTERFACE)
    add_library(wis::DX12Helpers ALIAS DX12Helpers)

    target_link_libraries(DX12Helpers INTERFACE
            DirectX-Headers
            DirectX-Guids)
    target_compile_definitions(DX12Helpers INTERFACE
            D3D12MA_USING_DIRECTX_HEADERS=1
    )
    install(DIRECTORY ${dxheaders_SOURCE_DIR}/include/directx DESTINATION include)
    install(
        TARGETS DirectX-Headers DirectX-Guids DX12Helpers
        EXPORT wisdom-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()


#  DirectX 12 Memory Allocator
set(D3D12MA_AGILITY_SDK_DIRECTORY "${DXA_DIR}")
if (NOT dxma_SOURCE_DIR)
    CPMAddPackage(
            NAME dxma
            GITHUB_REPOSITORY GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator
            GIT_TAG v3.1.0
            DOWNLOAD_ONLY TRUE
    )
    # Expose source dir
    set(dxma_SOURCE_DIR ${dxma_SOURCE_DIR} CACHE INTERNAL "")
else ()
    message("DirectX 12 Memory Allocator already loaded.")
endif ()


add_library(DX12Allocator STATIC ${dxma_SOURCE_DIR}/include/D3D12MemAlloc.h)
target_sources(DX12Allocator PRIVATE ${dxma_SOURCE_DIR}/src/D3D12MemAlloc.cpp)
target_link_libraries(DX12Allocator PUBLIC DX12Helpers)

target_include_directories(
        DX12Allocator PUBLIC $<BUILD_INTERFACE:${dxma_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/dxma>)

set_target_properties(DX12Allocator PROPERTIES
        CXX_STANDARD 20
        DEBUG_POSTFIX d
)
install(
        TARGETS DX12Allocator
        EXPORT wisdom-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${dxma_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dxma)
