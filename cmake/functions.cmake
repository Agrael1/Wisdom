if (WIN32)
    # Load NuGet.exe for Windows builds
    function(_ww_load_nuget)
        # Latest NuGet is at https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
        # Secure download with hash verification
        set(FILE_URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe")
        set(FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/NuGet/NuGet.exe")
        file(DOWNLOAD
                ${FILE_URL}
                ${FILE_PATH}
                STATUS download_status
                LOG download_log
                TIMEOUT 300
                TLS_VERIFY ON
                TLS_VERSION 1.2
        )
    
        # Check download status
        list(GET download_status 0 status_code)
        if (NOT status_code EQUAL 0)
            list(GET download_status 1 status_string)
            message(FATAL_ERROR "Download failed: ${status_string}")
        else ()
            message(STATUS "File downloaded successfully to ${FILE_PATH}")
        endif ()
    endfunction(_ww_load_nuget)
    
    # Find NuGet executable
    function(_ww_find_nuget)
        if (NOT WISDOM_WINDOWS)
            return()
        endif ()
    
        find_program(
                NUGET_EXE
                NAMES nuget)
    
        if (NOT NUGET_EXE)
            message("NUGET.EXE not found. Downloading...")
            find_program(
                    NUGET_EXE
                    NAMES nuget
                    PATHS ${CMAKE_CURRENT_BINARY_DIR}/NuGet)
    
            if (NOT NUGET_EXE)
                _ww_load_nuget()
                set(NUGET_EXE "${CMAKE_CURRENT_BINARY_DIR}/NuGet/NuGet.exe" CACHE INTERNAL "Path to NuGet.exe")
            endif ()
        else ()
            message("NUGET.EXE found: ${NUGET_EXE}")
        endif ()
    endfunction(_ww_find_nuget)
    
    # Load a NuGet dependency
    function(_ww_load_nuget_dependency NUGET PLUGIN_NAME ALIAS OUT_DIR)
        if (${ALIAS}_DIR)
            message("${ALIAS}_DIR already set, skipping download.")
            return()
        endif ()
    
        execute_process(COMMAND ${NUGET} install "${PLUGIN_NAME}" -OutputDirectory ${OUT_DIR})
        file(GLOB PLUGIN_DIRS ${OUT_DIR}/${PLUGIN_NAME}.*)
        list(LENGTH PLUGIN_DIRS PLUGIN_DIRS_L)
        if (${PLUGIN_DIRS_L} GREATER 1)
            #Sort directories by version in descending order, so the first dir is top version
            list(SORT PLUGIN_DIRS COMPARE NATURAL ORDER DESCENDING)
            list(GET PLUGIN_DIRS 0 PLUGIN_DIRX)
    
            #Remove older version
            MATH(EXPR PLUGIN_DIRS_L "${PLUGIN_DIRS_L}-1")
            foreach (I RANGE 1 ${PLUGIN_DIRS_L})
                list(GET PLUGIN_DIRS ${I} OLD)
                file(REMOVE_RECURSE ${OLD})
            endforeach ()
        else ()
            list(GET PLUGIN_DIRS 0 PLUGIN_DIRX)
        endif ()
    
        set(${ALIAS}_DIR ${PLUGIN_DIRX} CACHE STRING "${PLUGIN_NAME} PATH" FORCE)
    endfunction(_ww_load_nuget_dependency)
endif()


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

# Function to load DirectX 12 Agility SDK using NuGet
# Creates 3 targets:
# - DX12AgilityCore: The core Agility DLL (D3D12Core.dll)
# - DX12AgilitySDKLayers: The SDK Layers DLL (d3d12SDKLayers.dll)
# - DX12Agility: A helper static library that includes the Agility headers, for easy consumption by users. This is the main target that users should link against.
function(wis_load_agility_sdk)
    if (NOT WISDOM_WINDOWS)
        return()
    endif ()

    _ww_find_nuget()

    # DirectX 12 Agility SDK
    message("Setting up DirectX 12 Agility...")
    _ww_load_nuget_dependency(${NUGET_EXE} "Microsoft.Direct3D.D3D12" DXA
            ${CMAKE_CURRENT_BINARY_DIR})
    
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)$" VERSION_MATCH ${DXA_DIR})
    
    message("Agility version: ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
    set(DXA_VERSION
            ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}
            CACHE INTERNAL "")
    set(VERSION_MINOR
            ${CMAKE_MATCH_2}
            CACHE INTERNAL "")
    
    set(DXA_HEADERS ${DXA_DIR}/build/native/include)
    set(DXA_SRC ${DXA_DIR}/build/native/src)
    set(DXA_BIN ${DXA_DIR}/build/native/bin/x64)
    set(DXAGILITY_DLL
            ${DXA_BIN}/D3D12Core.dll
            CACHE INTERNAL "")
    set(DXAGILITY_DEBUG_DLL
            ${DXA_BIN}/d3d12SDKLayers.dll
            CACHE INTERNAL "")
    
    add_library(DX12AgilityCore MODULE IMPORTED GLOBAL)
    set_property(TARGET DX12AgilityCore PROPERTY IMPORTED_LOCATION
            ${DXAGILITY_DLL})
    
    add_library(DX12AgilitySDKLayers MODULE IMPORTED GLOBAL)
    set_property(TARGET DX12AgilitySDKLayers PROPERTY IMPORTED_LOCATION
            ${DXAGILITY_DEBUG_DLL})
    
    # Header interface library
    add_library(DX12Agility STATIC)
    add_library(wis::DX12Agility ALIAS DX12Agility)
    
    target_include_directories(
            DX12Agility SYSTEM BEFORE
            PUBLIC $<BUILD_INTERFACE:${DXA_HEADERS}> $<INSTALL_INTERFACE:include/d3dx12>
            PRIVATE $<BUILD_INTERFACE:${DXA_HEADERS}/d3dx12>)
    target_sources(DX12Agility
            PRIVATE ${DXA_SRC}/d3dx12/d3dx12_property_format_table.cpp)
    target_compile_definitions(DX12Agility PUBLIC
            DX12SDKVER=${VERSION_MINOR}
    )
    install(
            TARGETS DX12Agility
            EXPORT wisdom-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
    install(
            IMPORTED_RUNTIME_ARTIFACTS
            DX12AgilityCore
            DX12AgilitySDKLayers
            RUNTIME
            DESTINATION
            ${CMAKE_INSTALL_BINDIR}
            LIBRARY
            DESTINATION
            ${CMAKE_INSTALL_BINDIR})
    
    install(DIRECTORY ${DXA_HEADERS}/ DESTINATION include/d3dx12)
    
    set_target_properties(DX12Agility PROPERTIES
        DX12SDKVER ${VERSION_MINOR}
        DEBUG_POSTFIX d
    )

    set_property(
        TARGET DX12Agility
        APPEND
        PROPERTY EXPORT_PROPERTIES DX12SDKVER)

endfunction()

# Function for patching executable to export DX12 Agility symbols on Windows
function(wis_patch_agility_executable TARGET)
    if (NOT WISDOM_WINDOWS)
        return()
    endif()

    get_target_property(target_type ${TARGET_NAME} TYPE)

    if(target_type NOT STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "Target ${TARGET_NAME} is not an executable. DX12 Agility patching can only be applied to executables.")
    endif()

    # Check if the DX12Agility target is available
    if (NOT TARGET DX12Agility)
        message(FATAL_ERROR "DX12Agility target not found. Make sure to call wis_load_agility_sdk() before patching the executable.")
    endif()

    # Generate a source file that exports the required symbols for the DX12 Agility SDK. This is necessary to ensure that the application can load the Agility DLLs at runtime.
    get_property(DX12SDKVER TARGET DX12Agility PROPERTY DX12SDKVER)
    set(EXPORT_AGILITY "_declspec(dllexport) const unsigned D3D12SDKVersion = ${DX12SDKVER};
						_declspec(dllexport) const char* D3D12SDKPath = \".\\\\D3D12\\\\\";"
    )
    file(WRITE ${wis_export_agility_file_PATH} "${EXPORT_AGILITY}")

    # Add the generated file to the target sources to ensure it's compiled and linked into the executable
    target_sources(${TARGET} PRIVATE ${wis_export_agility_file_PATH})
endfunction()

# Function for installing DirectX SDK
# Arguments:
#   TARGET: Target to copy the DLLs to
#   PATCH_EXE: Whether to patch the executable to export the DX12 Agility symbols (default: OFF)
function(wis_install_agility_win32)
    cmake_parse_arguments(wis_install_agility_win32 "PATCH_EXE" "TARGET"
            "" ${ARGN})

    # Check if project is an executable
    if (NOT TARGET ${wis_install_agility_win32_TARGET})
        message(FATAL_ERROR "Target ${PROJECT} not found")
    endif()

    get_target_property(target_type ${TARGET_NAME} TYPE)

    if(target_type NOT STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "Target ${TARGET_NAME} is not an executable. DX12 Agility patching can only be applied to executables.")
    endif()

    message("Installing DirectX Agility SDK Dependency")
    if (EXISTS ${DXAGILITY_DLL})
        message("DX12 Agility Core found: ${DXAGILITY_DLL}")
        get_filename_component(DXAGILITY_DLL_NAME ${DXAGILITY_DLL} NAME)
        add_custom_command(TARGET ${PROJECT} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DXAGILITY_DLL} $<TARGET_FILE_DIR:${PROJECT}>/D3D12/${DXAGILITY_DLL_NAME}
                COMMAND_EXPAND_LISTS
                COMMENT "Copying DX12 Agility Core..."
        )
    endif()

    if (EXISTS ${DXAGILITY_DEBUG_DLL})
        message("DX12 Agility SDKLayers found: ${DXAGILITY_DEBUG_DLL}")
        get_filename_component(DXAGILITY_DEBUG_DLL_NAME ${DXAGILITY_DEBUG_DLL} NAME)
        add_custom_command(TARGET ${PROJECT} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${DXAGILITY_DEBUG_DLL} $<TARGET_FILE_DIR:${PROJECT}>/D3D12/${DXAGILITY_DEBUG_DLL_NAME}
                COMMAND_EXPAND_LISTS
                COMMENT "Copying DX12 Agility SDKLayers..."
        )
    endif()

    if (wis_install_agility_win32_PATCH_EXE)
        wis_patch_agility_executable(${wis_install_agility_win32_TARGET})
    endif()
endfunction()

