project(DX12Allocator)

if(NOT PLUGINS_LOADED)
    message("Setting up DirectX 12 Allocator...")
    FetchContent_Declare(
      dxma
      GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git
      GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(dxma)
    set(dxma_SOURCES ${dxma_SOURCE_DIR} CACHE INTERNAL "")
endif()

add_library(${PROJECT_NAME} STATIC ${dxma_SOURCES}/include/D3D12MemAlloc.h)
target_sources(${PROJECT_NAME}        
    PRIVATE
        ${dxma_SOURCES}/src/D3D12MemAlloc.cpp
)
if (MSVC)
    # Provides MSVC users nicer debugging support
    target_sources(${PROJECT_NAME} PRIVATE ${dxma_SOURCES}/src/D3D12MemAlloc.natvis)
endif()


target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${dxma_SOURCES}/include>
	    $<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME}
    	PROPERTIES
		    CXX_STANDARD 20
)
target_install(${PROJECT_NAME})
