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
    set(DXC_EXECUTABLE
        ${DXCOMPILER}
        CACHE INTERNAL "")
  elseif(WISDOM_USE_SYSTEM_DXC)
    set(DXC_EXECUTABLE
        "dxc"
        CACHE INTERNAL "")
  endif()
else()
  message("Loading DXC...")

  # Download latest DXC from AppVeyor
  if(WISDOM_WINDOWS)
    set(DXC_FILE
        https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505/dxc_2025_05_24.zip
    )
  else()
    set(DXC_FILE
        https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505/linux_dxc_2025_05_24.x86_64.tar.gz
    )
  endif()

  set(DOWNLOAD_EXTRACT_TIMESTAMP ON)

  FetchContent_Declare(dxc URL ${DXC_FILE})
  FetchContent_GetProperties(dxc)
  if(NOT dxc_POPULATED)
    FetchContent_Populate(dxc)
  endif()

  if(WIN32)
    set(DXC_EXECUTABLE
        ${dxc_SOURCE_DIR}/bin/x64/dxc.exe
        CACHE INTERNAL "")
    set(DXC_DLLS
        ${dxc_SOURCE_DIR}/bin/x64/dxcompiler.dll
        ${dxc_SOURCE_DIR}/bin/x64/dxil.dll)

    install(FILES ${DXC_DLLS} DESTINATION bin)
  else()
    set(DXC_EXECUTABLE
        ${dxc_SOURCE_DIR}/bin/dxc
        CACHE INTERNAL "")
    set(DXC_DLLS
        ${dxc_SOURCE_DIR}/lib/libdxcompiler.so
        ${dxc_SOURCE_DIR}/lib/libdxil.so)
    install(FILES ${DXC_DLLS} DESTINATION lib)
  endif()
  install(PROGRAMS ${DXC_EXECUTABLE} DESTINATION bin)
endif()

