project(shaders_vk)

add_custom_target(compile_shaders_vk)


file(GLOB_RECURSE vs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.vs.hlsl")
file(GLOB_RECURSE ps "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ps.hlsl")
file(GLOB_RECURSE hs "${CMAKE_CURRENT_SOURCE_DIR}/*/*.hs.hlsl")
file(GLOB_RECURSE ds "${CMAKE_CURRENT_SOURCE_DIR}/*/*.ds.hlsl")
file(GLOB_RECURSE ds "${CMAKE_CURRENT_SOURCE_DIR}/*/*.gs.hlsl")

set_source_files_properties(${vs} PROPERTIES ShaderType "vs")
set_source_files_properties(${ps} PROPERTIES ShaderType "ps")
set_source_files_properties(${ds} PROPERTIES ShaderType "ds")
set_source_files_properties(${hs} PROPERTIES ShaderType "hs")
set_source_files_properties(${hs} PROPERTIES ShaderType "gs")
    
add_library(${PROJECT_NAME} INTERFACE)
foreach(FILE ${vs} ${ps} ${ds} ${hs})
    get_filename_component(FILE_WE ${FILE} NAME_WLE)
    get_source_file_property(shadertype ${FILE} ShaderType)

    if(WIN32)
        add_custom_command(TARGET compile_shaders_vk
            COMMAND "${dxc_EXECUTABLE}" -Emain -T${shadertype}_6_5 -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -I"${CMAKE_CURRENT_SOURCE_DIR}/headers" -Fo"${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.cso" -Fd"${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pdb" "${FILE}"
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endif()

    #add_custom_command(TARGET compile_shaders_vk
    #    COMMAND ${dxc_EXECUTABLE} -Emain -T${shadertype}_6_1 -Zi $<IF:$<CONFIG:DEBUG>,/Od,/O3> -spirv -fspv-target-env=vulkan1.3 /I${CMAKE_CURRENT_SOURCE_DIR}/headers /Fo ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv ${FILE}
    #    MAIN_DEPENDENCY ${FILE}
    #    COMMENT "SPV ${FILE}"
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    #    VERBATIM)

endforeach(FILE)

add_dependencies(${PROJECT_NAME}
    compile_shaders_vk
)
