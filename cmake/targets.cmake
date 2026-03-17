function(wis_install_target target export_set)
    install(
            TARGETS ${target}
            EXPORT ${export_set}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endfunction()

function(wis_make_unified_targets)
    cmake_parse_arguments(UNIFIED
            ""
            "TARGET;EXPORT_SET;CXX_STANDARD;DEBUG_POSTFIX;DEFINITION_PREFIX;EXPORT_BASE_NAME;EXPORT_MACRO_NAME;EXPORT_FILE_NAME;EXPORT_STATIC_DEFINE;EXPORT_INCLUDE_GUARD_NAME"
            "BACKENDS;HEADER_LINK_TARGETS;HEADER_COMPILE_DEFINITIONS;STATIC_COMPILE_DEFINITIONS;SHARED_COMPILE_DEFINITIONS"
            ${ARGN})

    if (NOT UNIFIED_TARGET)
        message(FATAL_ERROR "TARGET is required for wis_make_unified_targets")
    endif ()

    if (NOT UNIFIED_EXPORT_SET)
        set(UNIFIED_EXPORT_SET wisdom-targets)
    endif ()

    if (NOT UNIFIED_CXX_STANDARD)
        set(UNIFIED_CXX_STANDARD 20)
    endif ()

    if (NOT UNIFIED_DEBUG_POSTFIX)
        set(UNIFIED_DEBUG_POSTFIX d)
    endif ()

    if (NOT UNIFIED_EXPORT_BASE_NAME)
        set(UNIFIED_EXPORT_BASE_NAME WISDOM)
    endif()

    set(HEADERS_TARGET "${UNIFIED_TARGET}-headers")
    add_library(${HEADERS_TARGET} INTERFACE)
    add_library(wis::${HEADERS_TARGET} ALIAS ${HEADERS_TARGET})

    if (UNIFIED_HEADER_LINK_TARGETS)
        target_link_libraries(${HEADERS_TARGET} INTERFACE ${UNIFIED_HEADER_LINK_TARGETS})
    endif ()

    foreach (WIS_BACKEND IN LISTS UNIFIED_BACKENDS)
        target_link_libraries(${HEADERS_TARGET} INTERFACE wisdom-${WIS_BACKEND}-headers)
    endforeach ()

    if (UNIFIED_HEADER_COMPILE_DEFINITIONS)
        target_compile_definitions(${HEADERS_TARGET} INTERFACE ${UNIFIED_HEADER_COMPILE_DEFINITIONS})
    endif ()

    wis_install_target(${HEADERS_TARGET} ${UNIFIED_EXPORT_SET})

    if (WISDOM_BUILD_STATIC)
        add_library(${UNIFIED_TARGET} STATIC)
        add_library(wis::${UNIFIED_TARGET} ALIAS ${UNIFIED_TARGET})
        target_link_libraries(${UNIFIED_TARGET} PUBLIC ${HEADERS_TARGET})
        foreach (WIS_BACKEND IN LISTS UNIFIED_BACKENDS)
            target_sources(${UNIFIED_TARGET} PRIVATE $<TARGET_OBJECTS:wisdom-${WIS_BACKEND}-objects>)
        endforeach ()

        if (UNIFIED_STATIC_COMPILE_DEFINITIONS)
            target_compile_definitions(${UNIFIED_TARGET} PUBLIC ${UNIFIED_STATIC_COMPILE_DEFINITIONS})
        endif ()

        set_target_properties(${UNIFIED_TARGET} PROPERTIES
                CXX_STANDARD ${UNIFIED_CXX_STANDARD}
                DEBUG_POSTFIX ${UNIFIED_DEBUG_POSTFIX})

        wis_install_target(${UNIFIED_TARGET} ${UNIFIED_EXPORT_SET})
    endif ()

    if (WISDOM_BUILD_SHARED)
        add_library(${UNIFIED_TARGET}-shared SHARED)
        add_library(wis::${UNIFIED_TARGET}-shared ALIAS ${UNIFIED_TARGET}-shared)
        target_link_libraries(${UNIFIED_TARGET}-shared PUBLIC ${HEADERS_TARGET})
        foreach (WIS_BACKEND IN LISTS UNIFIED_BACKENDS)
            target_sources(${UNIFIED_TARGET}-shared PRIVATE $<TARGET_OBJECTS:wisdom-${WIS_BACKEND}-objects-shared>)
        endforeach ()

        if (UNIFIED_SHARED_COMPILE_DEFINITIONS)
            target_compile_definitions(${UNIFIED_TARGET}-shared PUBLIC ${UNIFIED_SHARED_COMPILE_DEFINITIONS})
        endif ()

        set_target_properties(${UNIFIED_TARGET}-shared PROPERTIES
                CXX_STANDARD ${UNIFIED_CXX_STANDARD}
                POSITION_INDEPENDENT_CODE ON
                DEBUG_POSTFIX ${UNIFIED_DEBUG_POSTFIX})

        if (NOT UNIFIED_EXPORT_MACRO_NAME)
            set(UNIFIED_EXPORT_MACRO_NAME "${UNIFIED_EXPORT_BASE_NAME}_API")
        endif()

        if (NOT UNIFIED_EXPORT_STATIC_DEFINE)
            set (UNIFIED_EXPORT_STATIC_DEFINE "${UNIFIED_EXPORT_BASE_NAME}_STATIC")
        endif()

        if (NOT UNIFIED_EXPORT_INCLUDE_GUARD_NAME)
            set (UNIFIED_EXPORT_INCLUDE_GUARD_NAME "${UNIFIED_EXPORT_BASE_NAME}_EXPORTS_H")
        endif()

        if (UNIFIED_EXPORT_BASE_NAME AND UNIFIED_EXPORT_FILE_NAME)
            include(GenerateExportHeader)
            generate_export_header(
                    ${UNIFIED_TARGET}-shared
                    BASE_NAME
                    ${UNIFIED_EXPORT_BASE_NAME}
                    EXPORT_MACRO_NAME
                    ${UNIFIED_EXPORT_MACRO_NAME}
                    EXPORT_FILE_NAME
                    ${UNIFIED_EXPORT_FILE_NAME}
                    STATIC_DEFINE
                    ${UNIFIED_EXPORT_STATIC_DEFINE}
                    INCLUDE_GUARD_NAME
                    ${UNIFIED_EXPORT_INCLUDE_GUARD_NAME})
        endif ()

        wis_install_target(${UNIFIED_TARGET}-shared ${UNIFIED_EXPORT_SET})
    endif ()
endfunction()

function(wis_make_target_bundle)
    cmake_parse_arguments(BUNDLE "HEADER_ONLY" "TARGET;DEFINITION_PREFIX;EXPORT_SET" "LINK_TARGETS;SOURCES;HEADERS;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;" ${ARGN})

    if (NOT BUNDLE_TARGET)
        message(FATAL_ERROR "TARGET is required for wis_make_target_bundle")
    endif ()

    if (NOT BUNDLE_EXPORT_SET)
        set(BUNDLE_EXPORT_SET wisdom-targets)
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

    # Definition prefix
    if (BUNDLE_DEFINITION_PREFIX)
        set(DEF_PREF ${BUNDLE_DEFINITION_PREFIX})
    else()
        set(DEF_PREF WISDOM)
    endif()
    string(TOLOWER "${DEF_PREF}" EXPORT_PREF)

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
    wis_install_target(
        wisdom-${BUNDLE_TARGET}-headers
        ${BUNDLE_EXPORT_SET}
    )

    # Create OBJECT targets for use with $<TARGET_OBJECTS:...>
    if (NOT BUNDLE_HEADER_ONLY AND (WISDOM_BUILD_STATIC OR WISDOM_BUILD_SHARED) AND BUNDLE_SOURCES)
        if (WISDOM_BUILD_STATIC)
            add_library(wisdom-${BUNDLE_TARGET}-objects OBJECT ${BUNDLE_HEADERS})
            add_library(wis::wisdom-${BUNDLE_TARGET}-objects ALIAS wisdom-${BUNDLE_TARGET}-objects)
            target_sources(wisdom-${BUNDLE_TARGET}-objects
                    PRIVATE ${BUNDLE_SOURCES}
            )

            target_link_libraries(wisdom-${BUNDLE_TARGET}-objects PUBLIC wisdom-${BUNDLE_TARGET}-headers)
            target_compile_definitions(wisdom-${BUNDLE_TARGET}-objects PUBLIC ${DEF_PREF}_BUILD_BINARIES=1 ${DEF_PREF}_STATIC=1)
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
            )

            target_link_libraries(wisdom-${BUNDLE_TARGET}-objects-shared PUBLIC wisdom-${BUNDLE_TARGET}-headers)
            target_compile_definitions(wisdom-${BUNDLE_TARGET}-objects-shared PUBLIC
                    ${DEF_PREF}_BUILD_BINARIES=1
                    ${DEF_PREF}_SHARED_LIBRARY=1
                    PRIVATE
                    ${EXPORT_PREF}_shared_EXPORTS=1
            )
            set_target_properties(wisdom-${BUNDLE_TARGET}-objects-shared PROPERTIES
                    CXX_STANDARD 20
                    POSITION_INDEPENDENT_CODE ON
                    #UNITY_BUILD ON
            )
        endif ()

    endif ()
endfunction()
