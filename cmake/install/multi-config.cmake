# Include the release config as base (contains package metadata)
include("${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release/CPackConfig.cmake")

# Install from both Debug and Release builds
set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-debug;wisdom;ALL;/"
    "${CMAKE_CURRENT_LIST_DIR}/../../build/msvc-release;wisdom;ALL;/"
)