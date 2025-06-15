# check if Doxygen is installed
find_package(Doxygen)

if(WISDOM_BUILD_DOCS)
    if (DOXYGEN_FOUND)
        
        FetchContent_Declare(
            doxygen-awesome-css
            URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
        )
        FetchContent_MakeAvailable(doxygen-awesome-css)
        FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)




        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )

        target_sources(doc_doxygen PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/DoxygenLayout.xml
        )

        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/DoxygenLayout.xml
        )
    else()
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif()
endif()
