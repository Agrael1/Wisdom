project(hello-triangle-winrt
  DESCRIPTION "CMake example for Direct3D 12 (UWP)"
  LANGUAGES CXX
  VERSION 1.0.0.0)

option(ENABLE_CODE_ANALYSIS "Use Static Code Analysis on build" OFF)


if (NOT WINDOWS_STORE)
    message(FATAL_ERROR "ERROR: Must be built for WindowsStore")
endif()

add_executable(${PROJECT_NAME} WIN32
    entry_main.cpp
    "window.h"
 "app.h" "app.cpp")

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Wisdom)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Functions.cmake)
wis_install_dx_uwp(${PROJECT_NAME})


target_compile_definitions(${PROJECT_NAME} 
	PUBLIC 
		SHADER_DIR="."
)


add_dependencies(${PROJECT_NAME}
    compile_shaders_kd
)

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        _WIN32_WINNT=0x0A00 
        WINAPI_FAMILY=WINAPI_FAMILY_APP 
        _UNICODE 
        UNICODE
        NOMINMAX
        WINRT_LEAN_AND_MEAN
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 23    
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall /GR /fp:fast /bigobj "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>" /DYNAMICBASE /NXCOMPAT /APPCONTAINER /MANIFEST:NO)

    if((CMAKE_SIZEOF_VOID_P EQUAL 4) AND (NOT ${DIRECTX_ARCH} MATCHES "^arm"))
        target_compile_options(${PROJECT_NAME} PRIVATE /arch:SSE2)
        target_link_options(${PROJECT_NAME} PRIVATE /SAFESEH)
    endif()

    if((MSVC_VERSION GREATER_EQUAL 1924)
       AND ((NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang")) OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16.0)))
      target_compile_options(${PROJECT_NAME} PRIVATE /ZH:SHA_256)
    endif()

    if((MSVC_VERSION GREATER_EQUAL 1928)
       AND (CMAKE_SIZEOF_VOID_P EQUAL 8)
       AND ((NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang")) OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)))
        target_compile_options(${PROJECT_NAME} PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
        target_link_options(${PROJECT_NAME} PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
    endif()
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(UWP_MIN_VERSION 10.0.22621.0)
set(UWP_MAX_TESTED_VERSION 10.0.22621.0)
set(UWP_MIN_CRT_VERSION 14.0.30704.0)
configure_file(package.appxmanifest package.appxmanifest @ONLY)


