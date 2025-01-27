project(wisdom-extensions)

add_library(wisdom-extension-headers INTERFACE)
add_library(wis::extension-headers ALIAS wisdom-extension-headers)
add_library(wisdom-extension INTERFACE)
add_library(wis::extension ALIAS wisdom-extension)

add_subdirectory(debug_info)
add_subdirectory(extended_allocation)
add_subdirectory(descriptor_buffer)
add_subdirectory(raytracing)

install(
  TARGETS wisdom-extension wisdom-extension-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
