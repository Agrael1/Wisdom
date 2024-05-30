add_custom_target(compile_shaders_kd)

file(GLOB_RECURSE vs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.vs.hlsl")
file(GLOB_RECURSE ps "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ps.hlsl")
file(GLOB_RECURSE hs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.hs.hlsl")
file(GLOB_RECURSE ds "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ds.hlsl")
file(GLOB_RECURSE gs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.gs.hlsl")
file(GLOB_RECURSE cs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cs.hlsl")

foreach(FILE ${vs} ${ps} ${ds} ${hs} ${gs})
  wis_compile_shader(TARGET compile_shaders_kd SHADER ${FILE})
endforeach(FILE)

set(SHADER_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CACHE PATH "")
