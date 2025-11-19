# Helper function to compile shaders using DXC
# Usage:
#   wis_compile_shader(
#     TARGET <target>
#     SHADER <shader_file>
#     OUTPUT <output_file>
#     [TYPE <shader_type>]  # vs, ps, cs, lib, etc.
#     [SHADER_MODEL <model>]  # e.g., "6.0", "6.3"
#     [ENTRY_POINT <entry>]  # default: "main"
#   )
function(wis_compile_shader)
  cmake_parse_arguments(SHADER "" "TARGET;SHADER;OUTPUT;TYPE;SHADER_MODEL;ENTRY_POINT" "" ${ARGN})
  
  if(NOT SHADER_TARGET)
    message(FATAL_ERROR "TARGET is required for wis_compile_shader")
  endif()
  
  if(NOT SHADER_SHADER)
    message(FATAL_ERROR "SHADER is required for wis_compile_shader")
  endif()
  
  if(NOT SHADER_OUTPUT)
    message(FATAL_ERROR "OUTPUT is required for wis_compile_shader")
  endif()
  
  # Default values
  if(NOT SHADER_ENTRY_POINT)
    set(SHADER_ENTRY_POINT "main")
  endif()
  
  if(NOT SHADER_SHADER_MODEL)
    set(SHADER_SHADER_MODEL "6.0")
  endif()
  
  # Determine shader type from extension if not specified
  if(NOT SHADER_TYPE)
    get_filename_component(SHADER_EXT ${SHADER_SHADER} EXT)
    if(SHADER_EXT MATCHES "\\.vs\\.")
      set(SHADER_TYPE "vs")
    elseif(SHADER_EXT MATCHES "\\.ps\\.")
      set(SHADER_TYPE "ps")
    elseif(SHADER_EXT MATCHES "\\.cs\\.")
      set(SHADER_TYPE "cs")
    elseif(SHADER_EXT MATCHES "\\.gs\\.")
      set(SHADER_TYPE "gs")
    elseif(SHADER_EXT MATCHES "\\.hs\\.")
      set(SHADER_TYPE "hs")
    elseif(SHADER_EXT MATCHES "\\.ds\\.")
      set(SHADER_TYPE "ds")
    elseif(SHADER_EXT MATCHES "\\.lib\\.")
      set(SHADER_TYPE "lib")
    else()
      message(FATAL_ERROR "Cannot determine shader type from extension: ${SHADER_EXT}. Please specify TYPE parameter.")
    endif()
  endif()
  
  # Build DXC command
  set(DXC_ARGS
      -T ${SHADER_TYPE}_${SHADER_SHADER_MODEL}
      -E ${SHADER_ENTRY_POINT}
      ${SHADER_SHADER}
      -Fo ${SHADER_OUTPUT}
  )
  
  add_custom_command(
    OUTPUT ${SHADER_OUTPUT}
    COMMAND ${DXC_EXECUTABLE} ${DXC_ARGS}
    DEPENDS ${SHADER_SHADER}
    COMMENT "Compiling shader: ${SHADER_SHADER}"
    VERBATIM
  )
  
  add_custom_target(${SHADER_TARGET}_shader_${SHADER_TYPE} DEPENDS ${SHADER_OUTPUT})
  add_dependencies(${SHADER_TARGET} ${SHADER_TARGET}_shader_${SHADER_TYPE})
endfunction()
