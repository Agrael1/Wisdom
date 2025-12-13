include(${CMAKE_CURRENT_LIST_DIR}/nuget.cmake)

_ww_find_nuget()

# DirectX 12 Agility SDK
message("Setting up DirectX 12 Agility...")
_ww_load_nuget_dependency(${NUGET_EXE} "Microsoft.Direct3D.D3D12" DXA
                      ${CMAKE_CURRENT_BINARY_DIR})

string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)$" VERSION_MATCH ${DXA_DIR})

message("Agility version: ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
set(DXA_VERSION
    ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}
    CACHE INTERNAL "")
set(VERSION_MINOR
    ${CMAKE_MATCH_2}
    CACHE INTERNAL "")

set(DXA_HEADERS ${DXA_DIR}/build/native/include)
set(DXA_SRC ${DXA_DIR}/build/native/src)
set(DXA_BIN ${DXA_DIR}/build/native/bin/x64)
set(DXAGILITY_DLL
    ${DXA_BIN}/D3D12Core.dll
    CACHE INTERNAL "")
set(DXAGILITY_DEBUG_DLL
    ${DXA_BIN}/d3d12SDKLayers.dll
    CACHE INTERNAL "")

add_library(DX12AgilityCore SHARED IMPORTED GLOBAL)
set_property(TARGET DX12AgilityCore PROPERTY IMPORTED_LOCATION
                                                 ${DXAGILITY_DLL})

add_library(DX12AgilitySDKLayers SHARED IMPORTED GLOBAL)
set_property(TARGET DX12AgilitySDKLayers PROPERTY IMPORTED_LOCATION
                                                      ${DXAGILITY_DEBUG_DLL})

# Header interface library
add_library(DX12Agility STATIC)
add_library(wis::DX12Agility ALIAS DX12Agility)

target_include_directories(
  DX12Agility SYSTEM BEFORE
  PUBLIC $<BUILD_INTERFACE:${DXA_HEADERS}> $<INSTALL_INTERFACE:include/d3dx12>
  PRIVATE $<BUILD_INTERFACE:${DXA_HEADERS}/d3dx12>)
target_sources(DX12Agility
               PRIVATE ${DXA_SRC}/d3dx12/d3dx12_property_format_table.cpp)

install(
  TARGETS DX12Agility
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  IMPORTED_RUNTIME_ARTIFACTS
  DX12AgilityCore
  DX12AgilitySDKLayers
  RUNTIME
  DESTINATION
  ${CMAKE_INSTALL_BINDIR}
  LIBRARY
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${DXA_HEADERS}/ DESTINATION include/d3dx12)

set_target_properties(DX12Agility PROPERTIES 
  DX12SDKVER ${VERSION_MINOR}
  DEBUG_POSTFIX d
)

set_property(
  TARGET DX12Agility
  APPEND
  PROPERTY EXPORT_PROPERTIES DX12SDKVER)



#  DirectX 12 Memory Allocator
set(D3D12MA_AGILITY_SDK_DIRECTORY "${DXA_DIR}")
if(NOT dxma_SOURCE_DIR)
  CPMAddPackage(
    NAME dxma
    GITHUB_REPOSITORY GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator
    VERSION 3.0.1
    DOWNLOAD_ONLY TRUE
  )
  # Expose source dir
  set(dxma_SOURCE_DIR ${dxma_SOURCE_DIR} CACHE INTERNAL "")
else()
  message("DirectX 12 Memory Allocator already loaded.")
endif()


add_library(DX12Allocator STATIC ${dxma_SOURCE_DIR}/include/D3D12MemAlloc.h)
target_sources(DX12Allocator PRIVATE ${dxma_SOURCE_DIR}/src/D3D12MemAlloc.cpp)
target_link_libraries(DX12Allocator PRIVATE DX12Agility)
target_compile_definitions(DX12Allocator PRIVATE D3D12MA_OPTIONS16_SUPPORTED)
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
