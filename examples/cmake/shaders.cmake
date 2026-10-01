# Function to download the latest DXC release from GitHub API
function(_ww_load_latest_dxc)
    if (dxc_SOURCE_DIR)
        message(STATUS "DXC already downloaded, skipping.")
        return()
    endif ()

    set(DXC_API_FILE "${CMAKE_CURRENT_BINARY_DIR}/dxc_latest_api.json")
    file(DOWNLOAD
        "https://api.github.com/repos/microsoft/DirectXShaderCompiler/releases/latest"
        "${DXC_API_FILE}"
        STATUS api_status
    )

    list(GET api_status 0 api_err)
    if(api_err)
        message(WARNING "Wisdom: Failed to query DXC latest release from GitHub API: ${api_status}")
    endif()

    file(READ "${DXC_API_FILE}" DXC_JSON)

    # Take the first URL that ends with .zip (Windows release) from the JSON response
    if(DXC_JSON AND DXC_JSON MATCHES "\"browser_download_url\":[ \t\r\n]*\"([^\"]+\\.zip)\"")
        set(DXC_WINDOWS_LINK "${CMAKE_MATCH_1}")
    else()
        message(WARNING "Wisdom: Could not parse DXC zip URL from GitHub API response.")
        set(DXC_WINDOWS_LINK "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.9.2602/dxc_2026_02_20.zip")
    endif()

    # Take the first URL that ends with .tar.gz (Linux release) from the JSON response
    if(DXC_JSON AND DXC_JSON MATCHES "\"browser_download_url\":[ \t\r\n]*\"([^\"]+\\.tar\\.gz)\"")
        set(DXC_LINUX_LINK "${CMAKE_MATCH_1}")
    else()
        message(WARNING "Wisdom: Could not parse DXC tar.gz URL from GitHub API response.")
        set(DXC_LINUX_LINK "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.9.2602/linux_dxc_2026_02_20.x86_64.tar.gz")
    endif()

    if (WISDOM_WINDOWS)
        set(DXC_LINK ${DXC_WINDOWS_LINK})
    else ()
        set(DXC_LINK ${DXC_LINUX_LINK})
    endif ()


    # Download DXC using CPM
    include(FetchContent)
    FetchContent_Declare(
        dxc
        URL "${DXC_LINK}"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(dxc)
    set(dxc_SOURCE_DIR ${dxc_SOURCE_DIR} CACHE INTERNAL "")

    if (WIN32)
        set(DXC_EXECUTABLE
                ${dxc_SOURCE_DIR}/bin/x64/dxc.exe
                CACHE INTERNAL "")
    else ()
        set(DXC_EXECUTABLE
                ${dxc_SOURCE_DIR}/bin/dxc
                CACHE INTERNAL "")
    endif ()
endfunction()

# Function to load DXC
# Arguments:
#   DOWNLOAD_LATEST: Download the latest DXC from GitHub
#   DXC_PATH: Custom path to DXC installation (should contain bin/dxc.exe or bin/dxc)
function(wis_load_dxc)
    set(options DOWNLOAD_LATEST)
    set(oneValueArgs DXC_PATH)
    set(multiValueArgs)
    cmake_parse_arguments(wis_load_dxc "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN})

    # If DXC is already configured, skip loading
    if (DXC_EXECUTABLE)
        return()
    endif()

    # Error if none of the above are available

    # Option 1: DOWNLOAD_LATEST (highest priority)
    if (wis_load_dxc_DOWNLOAD_LATEST)
        message(STATUS "DOWNLOAD_LATEST option enabled, downloading latest DXC from GitHub")
        _ww_load_latest_dxc()
        return()
    endif()

    # Option 2: Custom DXC path (DXC_PATH)
    if (WISDOM_DXC_PATH)
        # Verify that the executable exists
        if (NOT EXISTS ${DXC_EXECUTABLE})
            message(WARNING "Custom DXC executable not found at: ${DXC_EXECUTABLE}")
            message(FATAL_ERROR "Please verify WISDOM_DXC_PATH is correct")
        else ()
            message(STATUS "Found custom DXC executable: ${DXC_EXECUTABLE}")
        endif ()

        message(STATUS "Using custom DXC path: ${WISDOM_DXC_PATH}")
        if (WIN32)
            set(DXC_EXECUTABLE "${WISDOM_DXC_PATH}/bin/dxc.exe" CACHE INTERNAL "")
        else ()
            set(DXC_EXECUTABLE "${WISDOM_DXC_PATH}/bin/dxc" CACHE INTERNAL "")
        endif ()
        return()
    endif()

    # Option 3: Try to use Vulkan SDK's DXC (if WISDOM_VULKAN is enabled and no custom path)
    if (WISDOM_VULKAN AND Vulkan_dxc_EXECUTABLE)
        # Use Vulkan SDK's DXC
        find_program(DXCOMPILER dxc HINTS ${Vulkan_dxc_EXECUTABLE} ENV VULKAN_SDK PATH_SUFFIXES bin)

        if (DXCOMPILER)
            message(STATUS "Found Vulkan SDK DXC: ${DXCOMPILER}")
            set(DXC_EXECUTABLE ${DXCOMPILER} CACHE INTERNAL "")
        else ()
            message(FATAL_ERROR "Vulkan SDK DXC not found in Vulkan SDK")
        endif ()
    endif()

    # Error if DXC_EXECUTABLE is still not set
    message(FATAL_ERROR "DXC executable not found. Please configure DXC using wis_load_dxc() with either DOWNLOAD_LATEST or DXC_PATH options, or ensure that the Vulkan SDK is installed and contains DXC.")
endfunction()

# Function for compiling shaders
# Arguments:
#	DXC: Path to the DXC executable (default: stored in ${DXC_EXECUTABLE} then in PATH)
#	TARGET: Target to add the shader to
#	ENTRY: Entry point of the shader (default: main)
#	SHADER: Path to the shader file
#	OUTPUT: Path to the output (default: ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.cso)
#	TYPE: Type of shader (vs, ps, cs, ds, gs, hs, ms, as)
#	SHADER_MODEL: Shader model to compile to (default: 6_1)
#	INCLUDE_DIRS: List of include directories
#	DEFINITIONS: List of preprocessor definitions, e.g. "MY_DEFINE=1"
function(wis_compile_shader)
    set(options)
    set(oneValueArgs DXC TARGET ENTRY SHADER OUTPUT TYPE SHADER_MODEL)
    set(multiValueArgs INCLUDE_DIRS DEFINITIONS FLAGS)
    cmake_parse_arguments(wis_compile_shader "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN})

    if (NOT wis_compile_shader_DXC OR NOT EXISTS ${wis_compile_shader_DXC})
        if (DXC_EXECUTABLE)
            set (wis_compile_shader_DXC ${DXC_EXECUTABLE})
        else ()
            message(FATAL_ERROR "wis_compile_shader: DXC not found. "
                "Please configure DXC using wis_load_dxc(), or provide a valid DXC path via DXC argument.")
        endif()
    endif ()

    if (NOT wis_compile_shader_TARGET)
        message(FATAL_ERROR "wis_compile_shader: TARGET not specified")
    endif ()

    if (NOT wis_compile_shader_SHADER)
        message(FATAL_ERROR "wis_compile_shader: SHADER not specified")
    endif ()

    if (NOT wis_compile_shader_TYPE)
        #try deducing from pattern .x.hlsl
        if (wis_compile_shader_SHADER MATCHES ".*\\.ps\\.hlsl$")
            set(wis_compile_shader_TYPE "ps")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.vs\\.hlsl$")
            set(wis_compile_shader_TYPE "vs")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.cs\\.hlsl$")
            set(wis_compile_shader_TYPE "cs")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.ds\\.hlsl$")
            set(wis_compile_shader_TYPE "ds")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.gs\\.hlsl$")
            set(wis_compile_shader_TYPE "gs")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.hs\\.hlsl$")
            set(wis_compile_shader_TYPE "hs")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.ms\\.hlsl$")
            set(wis_compile_shader_TYPE "ms")
        elseif (wis_compile_shader_SHADER MATCHES ".*\\.as\\.hlsl$")
            set(wis_compile_shader_TYPE "as")
        else ()
            message(FATAL_ERROR "wis_compile_shader: TYPE not specified")
        endif ()
    endif ()

    if (wis_compile_shader_ENTRY)
        set(ENTRY wis_compile_shader_ENTRY)
    else ()
        set(ENTRY "main")
    endif ()

    if (wis_compile_shader_SHADER_MODEL)
        # parse shader model from pattern x.y to x_y
        string(REGEX REPLACE "\\." "_" SHADER_MODEL ${wis_compile_shader_SHADER_MODEL})
    else ()
        set(SHADER_MODEL "6_1")
    endif ()

    foreach (INCLUDE_DIR ${wis_compile_shader_INCLUDE_DIRS})
        list(APPEND INCLUDES "-I${INCLUDE_DIR} ")
    endforeach ()

    foreach (DEFINITION ${wis_compile_shader_DEFINITIONS})
        list(APPEND DEFINES "-D${DEFINITION} ")
    endforeach ()

    foreach (FLAG ${wis_compile_shader_FLAGS})
        list(APPEND FLAGS "${FLAG} ")
    endforeach ()

    #remove trailing space
    string(STRIP "${INCLUDES}" INCLUDES)
    string(STRIP "${DEFINES}" DEFINES)
    string(STRIP "${FLAGS}" FLAGS)


    set(SHADER ${wis_compile_shader_SHADER})
    set(TARGET ${wis_compile_shader_TARGET})
    set(TYPE ${wis_compile_shader_TYPE})


    get_filename_component(FILE_WE ${SHADER} NAME_WLE)
    if (wis_compile_shader_OUTPUT)
        set(OUTPUT_DXIL ${wis_compile_shader_OUTPUT}.cso)
        set(OUTPUT_SPV ${wis_compile_shader_OUTPUT}.spv)
        set(OUTPUT_PDB ${wis_compile_shader_OUTPUT}.pdb)
    else ()
        set(OUTPUT_DXIL ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.cso)
        set(OUTPUT_SPV ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv)
        set(OUTPUT_PDB ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pdb)
    endif ()

    if (WINDOWS_STORE)
        file(TOUCH ${OUTPUT_DXIL})
        target_sources(${TARGET} PRIVATE ${OUTPUT_DXIL})
        set_property(SOURCE ${OUTPUT_DXIL} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    endif ()

    if (WIN32)
        add_custom_command(TARGET ${TARGET} POST_BUILD
                COMMAND "${wis_compile_shader_DXC}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -Wno-ignored-attributes ${FLAGS} ${INCLUDES} ${DEFINES} -DDXIL=1 -Fo${OUTPUT_DXIL} -Fd${OUTPUT_PDB} ${SHADER}
                COMMENT "HLSL ${SHADER}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                VERBATIM)
    endif ()

    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND "${wis_compile_shader_DXC}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -spirv -Wno-ignored-attributes ${FLAGS} -fspv-target-env=vulkan1.3 ${INCLUDES} ${DEFINES} -DSPIRV=1 -Fo${OUTPUT_SPV} ${SHADER}
            COMMENT "SPV ${SHADER}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endfunction()
