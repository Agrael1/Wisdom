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

    # Create helpers library
    add_library(DX12Helpers INTERFACE)
    add_library(wis::DX12Helpers ALIAS DX12Helpers)

    target_compile_definitions(DX12Helpers INTERFACE
            D3D12MA_USING_DIRECTX_HEADERS=1
    )

    # Guaranteed backwards compatibility. 
    # Using origin/main to ensure we get the latest headers, 
    # which are compatible with the latest SDKs.
    CPMAddPackage(
            NAME dxheaders
            GITHUB_REPOSITORY microsoft/DirectX-Headers
            GIT_TAG origin/main
    )

    target_link_libraries(DX12Helpers INTERFACE
            DirectX-Headers
            DirectX-Guids)

    install(DIRECTORY ${dxheaders_SOURCE_DIR}/include/directx DESTINATION include)
    install(DIRECTORY ${dxheaders_SOURCE_DIR}/include/dxguids DESTINATION include)
    install(
        TARGETS DirectX-Headers DirectX-Guids
        EXPORT wisdom-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(TARGETS DX12Helpers EXPORT wisdom-targets)
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


add_library(D3D12MemoryAllocator STATIC ${dxma_SOURCE_DIR}/include/D3D12MemAlloc.h)
add_library(GPUOpen::D3D12MemoryAllocator ALIAS D3D12MemoryAllocator)
target_sources(D3D12MemoryAllocator PRIVATE ${dxma_SOURCE_DIR}/src/D3D12MemAlloc.cpp)
target_link_libraries(D3D12MemoryAllocator PUBLIC DX12Helpers)

target_include_directories(
        D3D12MemoryAllocator PUBLIC $<BUILD_INTERFACE:${dxma_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/dxma>)

set_target_properties(D3D12MemoryAllocator PROPERTIES
        CXX_STANDARD 20
        DEBUG_POSTFIX d
)
install(
        TARGETS D3D12MemoryAllocator
        EXPORT wisdom-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${dxma_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dxma)
