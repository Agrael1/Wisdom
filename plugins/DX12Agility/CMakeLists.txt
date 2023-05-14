project(DX12Agility)

if(NOT PLUGINS_LOADED)
    message("Setting up DirectX 12 Agility...")
    load_nuget_dependency(${NUGET_EXE} "Microsoft.Direct3D.D3D12" DXA ${CMAKE_CURRENT_BINARY_DIR})

    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)$"
       VERSION_MATCH ${DXA_DIR})

    message("Agility version: ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
    set(VERSION_MINOR ${CMAKE_MATCH_2} CACHE INTERNAL "")
endif()

set(DXA_HEADERS ${DXA_DIR}/build/native/include)
set(DXA_SRC ${DXA_DIR}/build/native/src)
set(DXA_BIN ${DXA_DIR}/build/native/bin/x64)


add_library(${PROJECT_NAME}Core SHARED IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME}Core PROPERTY
             IMPORTED_LOCATION ${DXA_BIN}/D3D12Core.dll)

add_library(${PROJECT_NAME}SDKLayers SHARED IMPORTED GLOBAL)
set_property(TARGET ${PROJECT_NAME}SDKLayers PROPERTY
             IMPORTED_LOCATION ${DXA_BIN}/d3d12SDKLayers.dll)

#Header interface library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${DXA_HEADERS}>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY
             DX12SDKVER ${VERSION_MINOR})
