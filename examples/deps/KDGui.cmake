# windows store example does not need kdutils
if (NOT WISDOM_WINDOWS_STORE)
    set(BUILD_SHARED_LIBS OFF)
    set(KDUTILS_BUILD_TESTS OFF)

    find_package(KDUtils CONFIG)
    if(NOT KDUtils_FOUND)
        FetchContent_Declare(
            KDUtils
            GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
            GIT_TAG        master
        )
        FetchContent_MakeAvailable(KDUtils)
    endif()

    find_package(KDFoundation CONFIG)
    find_package(KDGui CONFIG)
endif()
