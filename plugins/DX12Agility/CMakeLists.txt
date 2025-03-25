project(DX12Agility)

find_package(DX12Agility QUIET)
if(${DX12Agility_FOUND})
  return()
endif()

if(NOT PLUGINS_LOADED)
  message("Setting up DirectX 12 Agility...")
  load_nuget_dependency(${NUGET_EXE} "Microsoft.Direct3D.D3D12" DXA
                        ${CMAKE_CURRENT_BINARY_DIR})

  string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)$" VERSION_MATCH ${DXA_DIR})

  message("Agility version: ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
  set(DXA_VERSION
      ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}
      CACHE INTERNAL "")
  set(VERSION_MINOR
      ${CMAKE_MATCH_2}
      CACHE INTERNAL "")
endif()

# file(RELATIVE_PATH variable directory file)

set(DXA_HEADERS ${DXA_DIR}/build/native/include)
set(DXA_SRC ${DXA_DIR}/build/native/src)
set(DXA_BIN ${DXA_DIR}/build/native/bin/x64)
set(DXAGILITY_DLL
    ${DXA_BIN}/D3D12Core.dll
    CACHE INTERNAL "")
set(DXAGILITY_DEBUG_DLL
    ${DXA_BIN}/d3d12SDKLayers.dll
    CACHE INTERNAL "")

add_library(${PROJECT_NAME}Core SHARED IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME}Core PROPERTY IMPORTED_LOCATION
                                                 ${DXAGILITY_DLL})

add_library(${PROJECT_NAME}SDKLayers SHARED IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME}SDKLayers PROPERTY IMPORTED_LOCATION
                                                      ${DXAGILITY_DEBUG_DLL})

# Header interface library
add_library(${PROJECT_NAME} STATIC)
add_library(wis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM BEFORE
  PUBLIC $<BUILD_INTERFACE:${DXA_HEADERS}> $<INSTALL_INTERFACE:include/d3dx12>
  PRIVATE $<BUILD_INTERFACE:${DXA_HEADERS}/d3dx12>)
target_sources(${PROJECT_NAME}
               PRIVATE ${DXA_SRC}/d3dx12/d3dx12_property_format_table.cpp)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT wisdom-dx12-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  IMPORTED_RUNTIME_ARTIFACTS
  ${PROJECT_NAME}Core
  ${PROJECT_NAME}SDKLayers
  RUNTIME
  DESTINATION
  ${CMAKE_INSTALL_BINDIR}
  LIBRARY
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${DXA_HEADERS}/ DESTINATION include/d3dx12)

set_target_properties(${PROJECT_NAME} PROPERTIES DX12SDKVER ${VERSION_MINOR})

set_property(
  TARGET ${PROJECT_NAME}
  APPEND
  PROPERTY EXPORT_PROPERTIES DX12SDKVER)
