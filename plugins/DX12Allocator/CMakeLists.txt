project(DX12Allocator)

message("Setting up DirectX 12 Allocator...")
FetchContent_Declare(
  dxma
  GIT_REPOSITORY
    https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git
  GIT_TAG origin/master)
set(D3D12MA_AGILITY_SDK_DIRECTORY "${DXA_DIR}")
if(NOT dxma_POPULATED)
  FetchContent_Populate(dxma)
endif()
set(dxma_SOURCES ${dxma_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${dxma_SOURCES}/include/D3D12MemAlloc.h)
target_sources(${PROJECT_NAME} PRIVATE ${dxma_SOURCES}/src/D3D12MemAlloc.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE DX12Agility)
target_compile_definitions(${PROJECT_NAME} PRIVATE D3D12MA_OPTIONS16_SUPPORTED)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${dxma_SOURCES}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/dxma>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT wisdom-dx12-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${dxma_SOURCES}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dxma)
