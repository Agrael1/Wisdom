if (NOT WISDOM_BUILD_DOCS)
  return()
endif()

# check if Doxygen is installed
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
  return()
endif()


CPMAddPackage(
    NAME doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
    DOWNLOAD_ONLY TRUE
)

# set input and output files
set(AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Copy custom CSS to build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/wisdom-custom.css
    ${CMAKE_CURRENT_BINARY_DIR}/docs/wisdom-custom.css
    COPYONLY
)

# request to configure the file
configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
message("Doxygen build started")

# note the option ALL which allows to build the docs together with the application
add_custom_target(doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )

set(DOC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/DoxygenLayout.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/main_page.h
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/contributing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/getting_started.h
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/wisdom-custom.css
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/dx12_factory_creation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/docs/header.html
)

target_sources(doc_doxygen PRIVATE ${DOC_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DOC_SOURCES})
