vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Agrael1/Wisdom
    REF "v${VERSION}"
    SHA512 0  # Will be updated when publishing
    HEAD_REF master
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        vulkan WISDOM_FORCE_VULKAN
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DWISDOM_BUILD_EXAMPLES=OFF
        -DWISDOM_BUILD_TESTS=OFF
        -DWISDOM_BUILD_DOCS=OFF
        -DWISDOM_BUILD_STATIC=ON
        -DWISDOM_BUILD_SHARED=ON
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_build()
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME wisdom CONFIG_PATH lib/cmake/wisdom)
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE.txt")
