project(DXC)

cmake_policy(SET CMP0135 NEW)

if(WISDOM_USE_SYSTEM_DXC)
  if(NOT Vulkan_dxc_EXECUTABLE)
    set(Vulkan_dxc_EXECUTABLE
        "dxc"
        CACHE INTERNAL "")
  endif()

  find_program(DXCOMPILER dxc HINTS ${Vulkan_dxc_EXECUTABLE})
  if(DXCOMPILER)
    message("Found DXC...")
    set(dxc_EXECUTABLE
        ${DXCOMPILER}
        CACHE INTERNAL "")
  elseif(WISDOM_USE_SYSTEM_DXC)
    set(dxc_EXECUTABLE
        "dxc"
        CACHE INTERNAL "")
  endif()
else()
  message("Loading DXC...")

  # Download latest DXC from AppVeyor
  if(WISDOM_WINDOWS)
    set(DXC_FILE
        https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2403/dxc_2024_03_07.zip
    )
  else()
    set(DXC_FILE
        https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2403/linux_dxc_2024_03_07.x86_64.tar.gz
    )
  endif()

  set(DOWNLOAD_EXTRACT_TIMESTAMP ON)

  FetchContent_Declare(dxc URL ${DXC_FILE})
  FetchContent_GetProperties(dxc)
  if(NOT dxc_POPULATED)
    FetchContent_Populate(dxc)
  endif()

  if(WIN32)
    set(dxc_EXECUTABLE
        ${dxc_SOURCE_DIR}/bin/x64/dxc.exe
        CACHE INTERNAL "")
  else()
    set(dxc_EXECUTABLE
        ${dxc_SOURCE_DIR}/bin/dxc
        CACHE INTERNAL "")
  endif()
endif()

install(PROGRAMS ${dxc_EXECUTABLE} DESTINATION bin)
