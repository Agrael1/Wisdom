# DXC Deployment Options
# Priority: 1. Custom path -> 2. Vulkan SDK -> 3. Auto-download

# Option 1: Custom DXC path (highest priority)
# Users can specify WISDOM_DXC_PATH to use their own DXC installation
# Example: cmake -DWISDOM_DXC_PATH="C:/custom/dxc" ..
if (WISDOM_DXC_PATH)
    message(STATUS "Using custom DXC path: ${WISDOM_DXC_PATH}")

    if (WIN32)
        set(DXC_EXECUTABLE "${WISDOM_DXC_PATH}/bin/dxc.exe" CACHE INTERNAL "")
        set(DXC_DLLS
                "${WISDOM_DXC_PATH}/bin/dxcompiler.dll"
                "${WISDOM_DXC_PATH}/bin/dxil.dll")
    else ()
        set(DXC_EXECUTABLE "${WISDOM_DXC_PATH}/bin/dxc" CACHE INTERNAL "")
        set(DXC_DLLS
                "${WISDOM_DXC_PATH}/lib/libdxcompiler.so"
                "${WISDOM_DXC_PATH}/lib/libdxil.so")
    endif ()

    # Verify that the executable exists
    if (NOT EXISTS ${DXC_EXECUTABLE})
        message(WARNING "Custom DXC executable not found at: ${DXC_EXECUTABLE}")
        message(WARNING "Please verify WISDOM_DXC_PATH is correct")
    else ()
        message(STATUS "Found custom DXC executable: ${DXC_EXECUTABLE}")
    endif ()

    # Option 2: Try to use Vulkan SDK's DXC (if WISDOM_VULKAN is enabled and no custom path)
elseif (WISDOM_VULKAN AND Vulkan_dxc_EXECUTABLE)
    message(STATUS "Using DXC from Vulkan SDK")

    # Use Vulkan SDK's DXC
    find_program(DXCOMPILER dxc HINTS ${Vulkan_dxc_EXECUTABLE} ENV VULKAN_SDK PATH_SUFFIXES bin)

    if (DXCOMPILER)
        message(STATUS "Found Vulkan SDK DXC: ${DXCOMPILER}")
        set(DXC_EXECUTABLE ${DXCOMPILER} CACHE INTERNAL "")

        # Try to find DLLs alongside the executable for deployment
        get_filename_component(DXC_BIN_DIR ${DXCOMPILER} DIRECTORY)

        if (WIN32)
            set(DXC_DLLS
                    "${DXC_BIN_DIR}/dxcompiler.dll"
                    "${DXC_BIN_DIR}/dxil.dll")
        else ()
            # On Linux, libraries might be in ../lib relative to bin
            get_filename_component(DXC_SDK_DIR ${DXC_BIN_DIR} DIRECTORY)
            set(DXC_DLLS
                    "${DXC_SDK_DIR}/lib/libdxcompiler.so"
                    "${DXC_SDK_DIR}/lib/libdxil.so")
        endif ()
    else ()
        message(STATUS "Vulkan SDK DXC not found, falling back to download")
        set(WISDOM_DOWNLOAD_DXC ON)
    endif ()

    # Option 3: Auto-download latest DXC (fallback)
else ()
    message(STATUS "Auto-downloading DXC...")
    set(WISDOM_DOWNLOAD_DXC ON)
endif ()

# Download DXC if needed
if (WISDOM_DOWNLOAD_DXC)
    if (NOT dxc_SOURCE_DIR)
        if (WISDOM_WINDOWS)
            set(DXC_FILE
                    https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.9.2602/dxc_2026_02_20.zip
            )
        else ()
            set(DXC_FILE
                    https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.9.2602/linux_dxc_2026_02_20.x86_64.tar.gz
            )
        endif ()

        # Download DXC using CPM
        CPMAddPackage(
                NAME dxc
                URL ${DXC_FILE}
        )
        set(dxc_SOURCE_DIR ${dxc_SOURCE_DIR} CACHE INTERNAL "")
    else ()
        message(STATUS "DXC already downloaded, skipping.")
    endif ()

    if (WIN32)
        set(DXC_EXECUTABLE
                ${dxc_SOURCE_DIR}/bin/x64/dxc.exe
                CACHE INTERNAL "")
        set(DXC_DLLS
                ${dxc_SOURCE_DIR}/bin/x64/dxcompiler.dll
                ${dxc_SOURCE_DIR}/bin/x64/dxil.dll)
    else ()
        set(DXC_EXECUTABLE
                ${dxc_SOURCE_DIR}/bin/dxc
                CACHE INTERNAL "")
        set(DXC_DLLS
                ${dxc_SOURCE_DIR}/lib/libdxcompiler.so
                ${dxc_SOURCE_DIR}/lib/libdxil.so)
    endif ()
endif ()

# Install DXC for deployment
if (WIN32)
    install(PROGRAMS ${DXC_EXECUTABLE} DESTINATION bin COMPONENT dxc)
    install(FILES ${DXC_DLLS} DESTINATION bin COMPONENT dxc)
else ()
    install(PROGRAMS ${DXC_EXECUTABLE} DESTINATION bin COMPONENT dxc)
    install(FILES ${DXC_DLLS} DESTINATION lib COMPONENT dxc)
endif ()

# Verify DLLs exist (warning only)
foreach (dll ${DXC_DLLS})
    if (NOT EXISTS ${dll})
        message(WARNING "DXC library not found: ${dll}")
    endif ()
endforeach ()
