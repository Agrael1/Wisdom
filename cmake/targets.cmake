function(wis_make_target_bundle)
    cmake_parse_arguments(BUNDLE "HEADER_ONLY" "TARGET" "LINK_TARGETS;SOURCES;HEADERS;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS" ${ARGN})

    if (NOT BUNDLE_TARGET)
        message(FATAL_ERROR "TARGET is required for wis_make_target_bundle")
    endif ()

    source_group("Headers" FILES ${BUNDLE_HEADERS})
    source_group("Sources" FILES ${BUNDLE_SOURCES})

    # Create header-only interface target
    add_library(wisdom-${BUNDLE_TARGET}-headers INTERFACE ${BUNDLE_HEADERS} ${BUNDLE_SOURCES})
    add_library(wis::wisdom-${BUNDLE_TARGET}-headers ALIAS wisdom-${BUNDLE_TARGET}-headers)

    # Link dependencies
    if (BUNDLE_LINK_TARGETS)
        target_link_libraries(wisdom-${BUNDLE_TARGET}-headers INTERFACE ${BUNDLE_LINK_TARGETS})
    endif ()

    # Include directories
    if (BUNDLE_INCLUDE_DIRECTORIES)
        target_include_directories(wisdom-${BUNDLE_TARGET}-headers
                INTERFACE
                $<BUILD_INTERFACE:${BUNDLE_INCLUDE_DIRECTORIES}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
    endif ()

    # Compile definitions
    if (BUNDLE_COMPILE_DEFINITIONS)
        target_compile_definitions(wisdom-${BUNDLE_TARGET}-headers INTERFACE ${BUNDLE_COMPILE_DEFINITIONS})
    endif ()

    set_target_properties(wisdom-${BUNDLE_TARGET}-headers PROPERTIES
            CXX_STANDARD 20
    )

    # install only headers
    install(
            TARGETS wisdom-${BUNDLE_TARGET}-headers
            EXPORT wisdom-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    # Create OBJECT targets for use with $<TARGET_OBJECTS:...>
    if (NOT BUNDLE_HEADER_ONLY AND (WISDOM_BUILD_STATIC OR WISDOM_BUILD_SHARED) AND BUNDLE_SOURCES)
        if (WISDOM_BUILD_STATIC)
            add_library(wisdom-${BUNDLE_TARGET}-objects OBJECT ${BUNDLE_HEADERS})
            add_library(wis::wisdom-${BUNDLE_TARGET}-objects ALIAS wisdom-${BUNDLE_TARGET}-objects)
            target_sources(wisdom-${BUNDLE_TARGET}-objects
                    PRIVATE ${BUNDLE_SOURCES}
            )

            target_link_libraries(wisdom-${BUNDLE_TARGET}-objects PUBLIC wisdom-${BUNDLE_TARGET}-headers)
            target_compile_definitions(wisdom-${BUNDLE_TARGET}-objects PUBLIC WISDOM_BUILD_BINARIES=1)
            set_target_properties(wisdom-${BUNDLE_TARGET}-objects PROPERTIES
                    CXX_STANDARD 20
                    #UNITY_BUILD ON
            )
        endif ()

        if (WISDOM_BUILD_SHARED)
            add_library(wisdom-${BUNDLE_TARGET}-objects-shared OBJECT ${BUNDLE_HEADERS})
            add_library(wis::wisdom-${BUNDLE_TARGET}-objects-shared ALIAS wisdom-${BUNDLE_TARGET}-objects-shared)
            target_sources(wisdom-${BUNDLE_TARGET}-objects-shared
                    PRIVATE ${BUNDLE_SOURCES}
                    "include/wisdom/generated/wisdom_exports.h"
            )

            target_link_libraries(wisdom-${BUNDLE_TARGET}-objects-shared PUBLIC wisdom-${BUNDLE_TARGET}-headers)
            target_compile_definitions(wisdom-${BUNDLE_TARGET}-objects-shared PUBLIC
                    WISDOM_BUILD_BINARIES=1
                    WISDOM_SHARED_LIBRARY=1
                    PRIVATE
                    wisdom_shared_EXPORTS=1
            )
            set_target_properties(wisdom-${BUNDLE_TARGET}-objects-shared PROPERTIES
                    CXX_STANDARD 20
                    POSITION_INDEPENDENT_CODE ON
                    #UNITY_BUILD ON
            )
        endif ()

    endif ()
endfunction()
