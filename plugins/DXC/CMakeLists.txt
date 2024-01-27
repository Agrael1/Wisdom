project(DXC)

cmake_policy(SET CMP0135 NEW)

if(WISDOM_USE_SYSTEM_DXC)
    if(NOT Vulkan_dxc_EXECUTABLE)
        set(Vulkan_dxc_EXECUTABLE "dxc" CACHE INTERNAL "")
    endif()
    
    find_program(DXCOMPILER dxc HINTS ${Vulkan_dxc_EXECUTABLE})
    if(DXCOMPILER)
        message("Found DXC...")
    	set(dxc_EXECUTABLE ${DXCOMPILER} CACHE INTERNAL "")
    elseif(WISDOM_USE_SYSTEM_DXC)
        set(dxc_EXECUTABLE "dxc" CACHE INTERNAL "")
    endif()
else()
    message("Loading DXC...")
    
    # Download latest DXC from AppVeyor
    if(WISDOM_WINDOWS)
        set(DXC_FILE https://ci.appveyor.com/api/projects/dnovillo/directxshadercompiler/artifacts/build%2FRelease%2Fdxc-artifacts.zip?branch=main&pr=false&job=image%3A%20Visual%20Studio%202022)
    else()
        set(DXC_FILE https://ci.appveyor.com/api/projects/dnovillo/directxshadercompiler/artifacts/build%2Fdxc-artifacts.tar.gz?branch=main&pr=false&job=image%3A%20Ubuntu)
    endif()
    
    set(DOWNLOAD_EXTRACT_TIMESTAMP ON)
    
    FetchContent_Declare(
      dxc
      URL ${DXC_FILE}
    )
    FetchContent_GetProperties(dxc)
    if(WIN32)
        set(dxc_EXECUTABLE ${dxc_SOURCE_DIR}/bin/dxc.exe CACHE INTERNAL "")
    else()
        set(dxc_EXECUTABLE ${dxc_SOURCE_DIR}/bin/dxc CACHE INTERNAL "")
    endif()
endif()
