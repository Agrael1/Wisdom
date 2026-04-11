# Include the release config as base (contains package metadata)
include("${CMAKE_CURRENT_LIST_DIR}/../../out/build/linux-gcc-lib/CPackConfig.cmake")

# Install from both Debug and Release builds
set(CPACK_INSTALL_CMAKE_PROJECTS
        "${CMAKE_CURRENT_LIST_DIR}/../../out/build/linux-gcc-debug-lib;wisdom;ALL;/"
        "${CMAKE_CURRENT_LIST_DIR}/../../out/build/linux-gcc-lib;wisdom;ALL;/"
)
