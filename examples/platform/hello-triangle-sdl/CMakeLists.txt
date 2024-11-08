project(hello-triangle-sdl)

add_executable(${PROJECT_NAME})
wis_install_deps(${PROJECT_NAME})

set(SOURCES "src/entry_main.cpp" )
set(HEADERS )

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC PlatformExamplesShared SDL3::SDL3
                                             wis::platform)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
