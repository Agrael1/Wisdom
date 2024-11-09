add_custom_target(compile_shaders_basic)

set(SHADER_DIR
    ${EXAMPLE_BIN_OUTPUT}/shaders
    CACHE PATH "")

# create folder for shaders
file(MAKE_DIRECTORY ${SHADER_DIR})

file(GLOB_RECURSE vs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.vs.hlsl")
file(GLOB_RECURSE ps "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ps.hlsl")
file(GLOB_RECURSE hs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.hs.hlsl")
file(GLOB_RECURSE ds "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ds.hlsl")
file(GLOB_RECURSE gs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.gs.hlsl")
file(GLOB_RECURSE cs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cs.hlsl")

foreach(FILE ${vs} ${ps} ${ds} ${hs} ${gs})
  get_filename_component(FILE_WE ${FILE} NAME_WLE)
  wis_compile_shader(TARGET compile_shaders_basic SHADER ${FILE} OUTPUT ${SHADER_DIR}/${FILE_WE})
endforeach(FILE)


