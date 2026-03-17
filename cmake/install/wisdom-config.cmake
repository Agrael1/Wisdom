@PACKAGE_INIT@

include(CMakeFindDependencyMacro)
include("${CMAKE_CURRENT_LIST_DIR}/functions.cmake")

# Detect platform and graphics APIs
wisdom_detect_platform()
wisdom_detect_platform_extensions()

# Set DXC and DX12 Agility paths
if(WISDOM_WINDOWS)
set(DXC_EXECUTABLE "@PACKAGE_CMAKE_INSTALL_BINDIR@/dxc.exe")
set(DXAGILITY_DLL "@PACKAGE_CMAKE_INSTALL_BINDIR@/D3D12Core.dll")
set(DXAGILITY_DEBUG_DLL "@PACKAGE_CMAKE_INSTALL_BINDIR@/d3d12SDKLayers.dll")
else()
set(DXC_EXECUTABLE "@PACKAGE_CMAKE_INSTALL_BINDIR@/dxc")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/wisdom-targets.cmake")

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/wisdom-platform-targets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/wisdom-platform-targets.cmake")
if(TARGET wis::wisdom-platform-headers)
target_compile_definitions(wis::wisdom-platform-headers INTERFACE 
    WIS_PLATFORM_WIN32_PRESENT=$<BOOL:${WISDOM_PLATFORM_WIN32_AVAILABLE}>
    WIS_PLATFORM_UWP_PRESENT=$<BOOL:${WISDOM_PLATFORM_UWP_AVAILABLE}>
    WIS_PLATFORM_XLIB_PRESENT=$<BOOL:${WISDOM_PLATFORM_XLIB_AVAILABLE}>
    WIS_PLATFORM_XCB_PRESENT=$<BOOL:${WISDOM_PLATFORM_XCB_AVAILABLE}>
    WIS_PLATFORM_WAYLAND_PRESENT=$<BOOL:${WISDOM_PLATFORM_WAYLAND_AVAILABLE}>
)
endif()
endif()

# Add definitions for consumers
if(WISDOM_VULKAN)
target_compile_definitions(wis::wisdom-headers INTERFACE WISDOM_VULKAN=1)
endif()
if(WISDOM_DX12)
target_compile_definitions(wis::wisdom-headers INTERFACE WISDOM_DX12=1)
endif()
if(WISDOM_WINDOWS)
target_compile_definitions(wis::wisdom-headers INTERFACE WISDOM_WINDOWS=1)
endif()
if(WISDOM_LINUX)
target_compile_definitions(wis::wisdom-headers INTERFACE WISDOM_LINUX=1)
endif()
