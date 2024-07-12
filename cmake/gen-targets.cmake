message("Hello")

configure_file(${CMAKE_CURRENT_LIST_DIR}/wisdom.targets ${CMAKE_INSTALL_PREFIX}/build/native/wisdom.targets @ONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/wisdom.props ${CMAKE_INSTALL_PREFIX}/build/native/wisdom.props @ONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/wisdom.xml ${CMAKE_INSTALL_PREFIX}/build/native/wisdom.xml @ONLY)