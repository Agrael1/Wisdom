if(CPACK_GENERATOR MATCHES "NuGet")
    message(STATUS "Wisdom CPack: NuGet generator detected. Injecting pre-build script.")
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_CURRENT_LIST_DIR}/nuget-prepare.cmake")
    set(CPACK_INSTALL_SCRIPTS "${CMAKE_CURRENT_LIST_DIR}/gen-targets.cmake")
elseif(CPACK_GENERATOR MATCHES "ZIP")
    message(STATUS "Wisdom CPack: ZIP generator detected. Proceeding with standard layout.")
    # No pre-build script needed! The CMake folders stay intact.
endif()