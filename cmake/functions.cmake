# Function to detect platform and set relevant variables
function(wisdom_detect_platform)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/ecm")

    # Check the platform
    if (WIN32)
        set(WISDOM_WINDOWS TRUE CACHE INTERNAL "Windows build" FORCE)
        set(WISDOM_WINDOWS_STORE ${WINDOWS_STORE} CACHE INTERNAL "Windows store build" FORCE)
        set(WISDOM_MAC FALSE CACHE INTERNAL "Mac build" FORCE)
        set(WISDOM_LINUX FALSE CACHE INTERNAL "Linux build" FORCE)

        if (WISDOM_WINDOWS_STORE)
            set(WISDOM_PLATFORM "WindowsStore" CACHE STRING "Platform name" FORCE)
        else ()
            set(WISDOM_PLATFORM "Windows" CACHE STRING "Platform name" FORCE)
        endif ()
    elseif (APPLE)
        set(WISDOM_WINDOWS FALSE CACHE INTERNAL "Windows build" FORCE)
        set(WISDOM_WINDOWS_STORE FALSE CACHE INTERNAL "Windows store build" FORCE)
        set(WISDOM_MAC TRUE CACHE INTERNAL "Mac build" FORCE)
        set(WISDOM_LINUX FALSE CACHE INTERNAL "Linux build" FORCE)
        set(WISDOM_PLATFORM "Mac" CACHE STRING "Platform name" FORCE)
    elseif (UNIX AND NOT APPLE)
        set(WISDOM_WINDOWS FALSE CACHE INTERNAL "Windows build" FORCE)
        set(WISDOM_WINDOWS_STORE FALSE CACHE INTERNAL "Windows store build" FORCE)
        set(WISDOM_MAC FALSE CACHE INTERNAL "Mac build" FORCE)
        set(WISDOM_LINUX TRUE CACHE INTERNAL "Linux build" FORCE)
        set(WISDOM_PLATFORM "Linux" CACHE STRING "Platform name" FORCE)
    endif ()

    # Detect underlying graphics system
    if (WISDOM_WINDOWS)
        set(WISDOM_DX12 TRUE CACHE BOOL "Use D3D12 as default graphics API" FORCE)
    endif ()

    # Detect Vulkan
    if (WISDOM_VULKAN_HEADER_PATH)
        set(WISDOM_VULKAN TRUE CACHE BOOL "Vulkan support detected" FORCE)
        set(WISDOM_VULKAN_VERSION ${Vulkan_VERSION} CACHE INTERNAL "Vulkan version detected" FORCE)

        # Create an imported target for Vulkan
        add_library(Vulkan::Headers INTERFACE IMPORTED)
        set_target_properties(Vulkan::Headers PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${WISDOM_VULKAN_HEADER_PATH}"
        )
    elseif (DEFINED ENV{VULKAN_SDK} AND NOT Vulkan_FOUND)
        set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}")
        message(STATUS "VULKAN_SDK environment variable found: ${VULKAN_SDK_PATH}")
        
        if (EXISTS "${VULKAN_SDK_PATH}/include/vulkan/vulkan.h")
            set(WISDOM_VULKAN TRUE CACHE BOOL "Vulkan support detected" FORCE)
            message(STATUS "Vulkan headers found in VULKAN_SDK path")
        else ()
            set(WISDOM_VULKAN FALSE CACHE BOOL "Vulkan support detected" FORCE)
            message(WARNING "Vulkan headers not found in VULKAN_SDK path: ${VULKAN_SDK_PATH}/include/vulkan/vulkan.h")
            message(WARNING "Please verify that VULKAN_SDK is set correctly and contains the Vulkan SDK")
            return()
        endif()

        add_library(Vulkan::Headers INTERFACE IMPORTED)
        set_target_properties(Vulkan::Headers PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_SDK_PATH}/include"
        )
    else()
        # Try to find Vulkan
        find_package(Vulkan QUIET)
        if (Vulkan_FOUND)
            set(WISDOM_VULKAN TRUE CACHE BOOL "Vulkan support detected" FORCE)
            set(WISDOM_VULKAN_VERSION ${Vulkan_VERSION} CACHE INTERNAL "Vulkan version detected" FORCE)
            message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS}")
        else ()
            set(WISDOM_VULKAN FALSE CACHE BOOL "Vulkan support detected" FORCE)
            message(STATUS "Vulkan not found")
        endif ()
    endif ()
endfunction()


# Function for installing DirectX SDK for UWP
function(wis_export_agility_file)
    set(options)
    set(oneValueArgs PATH)
    set(multiValueArgs)

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
    if (WIN32 AND NOT WINDOWS_STORE)
        wis_install_dx_win32(${PROJECT})
    elseif (WINDOWS_STORE)
        wis_install_dx_uwp(${PROJECT})
    endif (WIN32 AND NOT WINDOWS_STORE)
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

    if (NOT wis_compile_shader_DXC)
        if (NOT DXC_EXECUTABLE)
            find_program(wis_compile_shader_DXC dxc)
        else ()
            set(wis_compile_shader_DXC ${DXC_EXECUTABLE})
        endif ()
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

# Function to detect platform extensions and set relevant variables
# This should be called after wisdom_detect_platform to ensure platform variables are set
# It will set WISDOM_PLATFORM_<PLATFORM>_PRESENT variables for each platform and a WISDOM_PRESENT_PLATFORMS variable listing all available platforms
function(wisdom_detect_platform_extensions)
    set(WISDOM_PLATFORM_WIN32_PRESENT FALSE CACHE INTERNAL "Win32 platform extension available" FORCE)
    set(WISDOM_PLATFORM_UWP_PRESENT FALSE CACHE INTERNAL "UWP platform extension available" FORCE)
    set(WISDOM_PLATFORM_XLIB_PRESENT FALSE CACHE INTERNAL "Xlib platform extension available" FORCE)
    set(WISDOM_PLATFORM_XCB_PRESENT FALSE CACHE INTERNAL "XCB platform extension available" FORCE)
    set(WISDOM_PLATFORM_WAYLAND_PRESENT FALSE CACHE INTERNAL "Wayland platform extension available" FORCE)

    set(_wisdom_available_platforms)

    if (WISDOM_WINDOWS)
        set(WISDOM_PLATFORM_WIN32_PRESENT TRUE CACHE INTERNAL "Win32 platform extension available" FORCE)
        set(WISDOM_PLATFORM_UWP_PRESENT TRUE CACHE INTERNAL "UWP platform extension available" FORCE)
        list(APPEND _wisdom_available_platforms "WIN32")
        list(APPEND _wisdom_available_platforms "UWP")
    endif ()

    if (WISDOM_LINUX)
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/ecm")

        find_package(X11 QUIET)
        if (X11_FOUND)
            set(WISDOM_PLATFORM_XLIB_PRESENT TRUE CACHE INTERNAL "Xlib platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "XLIB")
        endif ()

        find_package(XCB COMPONENTS XCB QUIET)
        if (XCB_FOUND)
            set(WISDOM_PLATFORM_XCB_PRESENT TRUE CACHE INTERNAL "XCB platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "XCB")
        endif ()

        find_package(Wayland QUIET)
        if (Wayland_FOUND)
            set(WISDOM_PLATFORM_WAYLAND_PRESENT TRUE CACHE INTERNAL "Wayland platform extension available" FORCE)
            list(APPEND _wisdom_available_platforms "WAYLAND")
        endif ()
    endif ()

    if (_wisdom_available_platforms)
        list(JOIN _wisdom_available_platforms ", " WISDOM_AVAILABLE_PLATFORMS)
    else ()
        set(WISDOM_AVAILABLE_PLATFORMS "None")
    endif ()

    set(WISDOM_AVAILABLE_PLATFORMS ${WISDOM_AVAILABLE_PLATFORMS} CACHE INTERNAL "Available platform extensions" FORCE)
endfunction()
