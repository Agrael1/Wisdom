project(test)

set(HEADERS)
set(SOURCES entry_main.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC wis::wisdom-dx12)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

#if(POSTFIX STREQUAL "dx12")
#  wis_make_exports_dx(${PROJECT_NAME}) # install the d3d12 agility sdk, for
#                                       # examples this is enough
#  # for the main project, use wis_installdeps(${PROJECT_NAME}) instead
#endif()