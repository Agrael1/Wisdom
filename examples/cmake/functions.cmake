function(wis_example_libraries MODE)
  if(MODE STREQUAL "static")
    set(WISDOM_EXAMPLE_CORE wis::wisdom PARENT_SCOPE)
    set(WISDOM_EXAMPLE_PLATFORM wis::wisdom-platform PARENT_SCOPE)
  elseif(MODE STREQUAL "shared")
    set(WISDOM_EXAMPLE_CORE wis::wisdom-shared PARENT_SCOPE)
    set(WISDOM_EXAMPLE_PLATFORM wis::wisdom-platform-shared PARENT_SCOPE)
  elseif(MODE STREQUAL "headers")
    set(WISDOM_EXAMPLE_CORE wis::wisdom-headers PARENT_SCOPE)
    set(WISDOM_EXAMPLE_PLATFORM wis::wisdom-platform-headers PARENT_SCOPE)
  endif()
endfunction()

function(wis_setup_example TARGET MODE)
  wis_example_libraries(${MODE})
  target_link_libraries(${TARGET} PRIVATE ${WISDOM_EXAMPLE_CORE}
                                        ${WISDOM_EXAMPLE_PLATFORM} SDL3::SDL3)
  target_compile_definitions(${TARGET} PRIVATE ${ADD_DEFINITIONS})
  set_target_properties(${TARGET} PROPERTIES C_STANDARD 11 CXX_STANDARD 20
                                            RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT})
  if(MODE STREQUAL "headers")
    set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 23)
  endif()
  add_dependencies(${TARGET} wis_test_compile_shaders wis_examples_copy_assets)

  foreach(LIBRARY SDL3::SDL3 ${WISDOM_EXAMPLE_CORE} ${WISDOM_EXAMPLE_PLATFORM})
    get_target_property(LIBRARY_TYPE ${LIBRARY} TYPE)
    if(LIBRARY_TYPE STREQUAL "SHARED_LIBRARY")
      add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIBRARY}>
                $<TARGET_FILE_DIR:${TARGET}>
        VERBATIM)
    endif()
  endforeach()

  if(POSTFIX STREQUAL "dx12" AND TARGET wis::DX12Agility)
    wis_install_agility_win32(TARGET ${TARGET} ${ARGN})
  endif()
endfunction()
