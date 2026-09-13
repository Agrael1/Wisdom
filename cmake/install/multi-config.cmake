# Include the release config as base (contains package metadata)
include("${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release-zip/CPackConfig.cmake")

# Install from both Debug and Release builds (ZIP includes Agility SDK)
set(CPACK_INSTALL_CMAKE_PROJECTS
        "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-debug-zip;wisdom;ALL;/"
        "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release-zip;wisdom;ALL;/"
)
