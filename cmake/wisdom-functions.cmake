# Function for installing DirectX SDK for UWP
function(wis_export_agility_file)
	set(options )
	set(oneValueArgs PATH)
	set(multiValueArgs )

	cmake_parse_arguments(wis_export_agility_file
							  "${options}" "${oneValueArgs}" "${multiValueArgs}"
						  ${ARGN})

	get_property(DX12SDKVER TARGET wis::DX12Agility PROPERTY DX12SDKVER)

	set(EXPORT_AGILITY "_declspec(dllexport) const unsigned D3D12SDKVersion = ${DX12SDKVER};
						_declspec(dllexport) const char* D3D12SDKPath = \".\\\\D3D12\\\\\";"
	)
	file(WRITE ${wis_export_agility_file_PATH} "${EXPORT_AGILITY}")
endfunction()

function(wis_make_exports_dx PROJECT)
  wis_export_agility_file(PATH ${CMAKE_CURRENT_BINARY_DIR}/exports.c)

  target_sources(${PROJECT} PRIVATE
  	${CMAKE_CURRENT_BINARY_DIR}/exports.c
  )
endfunction()

function(wis_install_dx_uwp PROJECT)
    message("Installing DirectX Agility SDK Dependency")
	wis_export_agility_file(PATH "${CMAKE_CURRENT_BINARY_DIR}/exports.c")

	target_sources(${PROJECT} PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/exports.c
	)

	message("DX12AgilityCore: ${DXAGILITY_DLL}")
	set_property(SOURCE ${DXAGILITY_DLL} PROPERTY VS_DEPLOYMENT_CONTENT 1)
	set_property(SOURCE ${DXAGILITY_DLL} PROPERTY VS_DEPLOYMENT_LOCATION "D3D12")
	target_sources(${PROJECT} PRIVATE ${DXAGILITY_DLL})

	message("DX12AgilitySDKLayers: ${DXAGILITY_DEBUG_DLL}")
	set_property(SOURCE ${DXAGILITY_DEBUG_DLL} PROPERTY VS_DEPLOYMENT_CONTENT 1)
	set_property(SOURCE ${DXAGILITY_DEBUG_DLL} PROPERTY VS_DEPLOYMENT_LOCATION "D3D12")
	target_sources(${PROJECT} PRIVATE ${DXAGILITY_DEBUG_DLL})
endfunction()

# Function for installing DirectX SDK
function(wis_install_dx_win32 PROJECT)
	message("Installing DirectX Agility SDK Dependency")
	wis_export_agility_file(PATH "${CMAKE_CURRENT_BINARY_DIR}/exports.c")

	target_sources(${PROJECT} PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/exports.c
	)

	get_filename_component(DXAGILITY_DLL_NAME ${DXAGILITY_DLL} NAME)
	add_custom_command(TARGET ${PROJECT} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DXAGILITY_DLL} $<TARGET_FILE_DIR:${PROJECT}>/D3D12/${DXAGILITY_DLL_NAME}
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility Core..."
	)


	get_filename_component(DXAGILITY_DEBUG_DLL_NAME ${DXAGILITY_DEBUG_DLL} NAME)
	add_custom_command(TARGET ${PROJECT} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy ${DXAGILITY_DEBUG_DLL} $<TARGET_FILE_DIR:${PROJECT}>/D3D12/${DXAGILITY_DEBUG_DLL_NAME}
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility SDKLayers..."
	)
endfunction()

# Function for installing Wisdom Dependencies
function(wis_install_deps PROJECT)
    if(WIN32 AND NOT WINDOWS_STORE)
		wis_install_dx_win32(${PROJECT})
	elseif(WINDOWS_STORE)
		wis_install_dx_uwp(${PROJECT})
	endif(WIN32 AND NOT WINDOWS_STORE)
endfunction()



# Function for compiling shaders
# Arguments:
#	TARGET: Target to add the shader to
#	ENTRY: Entry point of the shader (default: main)
#	SHADER: Path to the shader file
#	OUTPUT: Path to the output (default: ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.cso)
#	TYPE: Type of shader (vs, ps, cs, ds, gs, hs, ms, as)
#	SHADER_MODEL: Shader model to compile to (default: 6_1)
#	INCLUDE_DIRS: List of include directories
#	DEFINITIONS: List of preprocessor definitions, e.g. "MY_DEFINE=1"
function(wis_compile_shader)
	set(options )
    set(oneValueArgs DXC TARGET ENTRY SHADER OUTPUT TYPE SHADER_MODEL)
    set(multiValueArgs INCLUDE_DIRS DEFINITIONS FLAGS)
    cmake_parse_arguments(wis_compile_shader "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

	if(NOT wis_compile_shader_DXC)
		if(NOT DXC_EXECUTABLE)
			find_program(wis_compile_shader_DXC dxc)
		else()
			set(wis_compile_shader_DXC ${DXC_EXECUTABLE})
		endif()
	endif()

	if(NOT wis_compile_shader_TARGET)
		message(FATAL_ERROR "wis_compile_shader: TARGET not specified")
	endif()

	if(NOT wis_compile_shader_SHADER)
		message(FATAL_ERROR "wis_compile_shader: SHADER not specified")
	endif()

	if(NOT wis_compile_shader_TYPE)
		#try deducing from pattern .x.hlsl
		if(wis_compile_shader_SHADER MATCHES ".*\\.ps\\.hlsl$")
			set(wis_compile_shader_TYPE "ps")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.vs\\.hlsl$")
			set(wis_compile_shader_TYPE "vs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.cs\\.hlsl$")
			set(wis_compile_shader_TYPE "cs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.ds\\.hlsl$")
			set(wis_compile_shader_TYPE "ds")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.gs\\.hlsl$")
			set(wis_compile_shader_TYPE "gs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.hs\\.hlsl$")
			set(wis_compile_shader_TYPE "hs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.ms\\.hlsl$")
			set(wis_compile_shader_TYPE "ms")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.as\\.hlsl$")
			set(wis_compile_shader_TYPE "as")
		else()
			message(FATAL_ERROR "wis_compile_shader: TYPE not specified")
		endif()
	endif()

	if(wis_compile_shader_ENTRY)
		set(ENTRY wis_compile_shader_ENTRY)
	else()
		set(ENTRY "main")
	endif()

	if(wis_compile_shader_SHADER_MODEL)
		# parse shader model from pattern x.y to x_y
		string(REGEX REPLACE "\\." "_" SHADER_MODEL ${wis_compile_shader_SHADER_MODEL})
	else()
		set(SHADER_MODEL "6_1")
	endif()

	foreach(INCLUDE_DIR ${wis_compile_shader_INCLUDE_DIRS})
		list(APPEND INCLUDES "-I${INCLUDE_DIR} ")
	endforeach()

	foreach(DEFINITION ${wis_compile_shader_DEFINITIONS})
		list(APPEND DEFINES "-D${DEFINITION} ")
	endforeach()

	foreach(FLAG ${wis_compile_shader_FLAGS})
		list(APPEND FLAGS "${FLAG} ")
	endforeach()

	#remove trailing space
	string(STRIP "${INCLUDES}" INCLUDES)
	string(STRIP "${DEFINES}" DEFINES)
	string(STRIP "${FLAGS}" FLAGS)


	set(SHADER ${wis_compile_shader_SHADER})
	set(TARGET ${wis_compile_shader_TARGET})
	set(TYPE ${wis_compile_shader_TYPE})


	get_filename_component(FILE_WE ${SHADER} NAME_WLE)
	if(wis_compile_shader_OUTPUT)
		set(OUTPUT_DXIL ${wis_compile_shader_OUTPUT}.cso)
		set(OUTPUT_SPV ${wis_compile_shader_OUTPUT}.spv)
		set(OUTPUT_PDB ${wis_compile_shader_OUTPUT}.pdb)
	else()
		set(OUTPUT_DXIL ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.cso)
		set(OUTPUT_SPV ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv)
		set(OUTPUT_PDB ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pdb)
	endif()

    if(WINDOWS_STORE)
        file(TOUCH ${OUTPUT_DXIL})
        target_sources(${TARGET} PRIVATE ${OUTPUT_DXIL})
        set_property(SOURCE ${OUTPUT_DXIL} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    endif()

    if(WIN32)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND "${wis_compile_shader_DXC}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -Wno-ignored-attributes ${FLAGS} ${INCLUDES} ${DEFINES} -DDXIL=1 -Fo${OUTPUT_DXIL} -Fd${OUTPUT_PDB} ${SHADER}
            COMMENT "HLSL ${SHADER}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endif()

    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND "${wis_compile_shader_DXC}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -spirv -Wno-ignored-attributes ${FLAGS} -fspv-target-env=vulkan1.3 ${INCLUDES} ${DEFINES} -DSPIRV=1 -Fo${OUTPUT_SPV} ${SHADER}
        COMMENT "SPV ${SHADER}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endfunction()

function(add_alias_target TARGET ALIAS)
	if(NOT TARGET ${ALIAS})
		add_library(${ALIAS} ALIAS ${TARGET})
	endif()
endfunction(add_alias_target)
