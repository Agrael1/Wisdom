project(DXC)

cmake_policy(SET CMP0135 NEW)

if(NOT PLUGINS_LOADED)
    message("Loading DXC...")

    if(WIN32)
        set(DXC_FILE https://ci.appveyor.com/api/projects/dnovillo/directxshadercompiler/artifacts/build%2FRelease%2Fdxc-artifacts.zip?branch=main&pr=false&job=image%3A%20Visual%20Studio%202022)
    else()
        set(DXC_FILE https://ci.appveyor.com/api/projects/dnovillo/directxshadercompiler/artifacts/build%2Fdxc-artifacts.tar.gz?branch=main&pr=false&job=image%3A%20Ubuntu)
    endif()

    set(DOWNLOAD_EXTRACT_TIMESTAMP ON)

    FetchContent_Declare(
      dxc
      URL ${DXC_FILE}
    )
    FetchContent_MakeAvailable(dxc)

    set(dxc_EXECUTABLE ${dxc_SOURCE_DIR}/bin/dxc.exe CACHE INTERNAL "")
endif()
