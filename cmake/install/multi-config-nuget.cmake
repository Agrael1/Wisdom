# NuGet-specific multi-config - excludes DXC component
# Users should install Microsoft.Direct3D.DXC NuGet package separately
# NuGet package is built without Agility SDK

# Include the release config as base (contains package metadata)
include("${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release-nuget/CPackConfig.cmake")

# Only include the default component, excluding 'dxc' component
set(CPACK_COMPONENTS_ALL Unspecified)

# Install from both Debug and Release builds (Unspecified component only)
set(CPACK_INSTALL_CMAKE_PROJECTS
        "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-debug-nuget;wisdom;Unspecified;/"
        "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release-nuget;wisdom;Unspecified;/"
)
