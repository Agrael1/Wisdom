
function(wisdom_detect_platform)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/ecm")
  
  # Check the platform
  if(WIN32)
    set(WISDOM_WINDOWS TRUE CACHE INTERNAL "Windows build" FORCE)
    set(WISDOM_WINDOWS_STORE ${WINDOWS_STORE} CACHE INTERNAL "Windows store build" FORCE)
    set(WISDOM_MAC FALSE CACHE INTERNAL "Mac build" FORCE)
    set(WISDOM_LINUX FALSE CACHE INTERNAL "Linux build" FORCE)

    if(WISDOM_WINDOWS_STORE)
      set(WISDOM_PLATFORM "WindowsStore" CACHE STRING "Platform name" FORCE)
    else()
      set(WISDOM_PLATFORM "Windows" CACHE STRING "Platform name" FORCE)
    endif()
  elseif(APPLE)
    set(WISDOM_WINDOWS FALSE CACHE INTERNAL "Windows build" FORCE)
    set(WISDOM_WINDOWS_STORE FALSE CACHE INTERNAL "Windows store build" FORCE)
    set(WISDOM_MAC TRUE CACHE INTERNAL "Mac build" FORCE)
    set(WISDOM_LINUX FALSE CACHE INTERNAL "Linux build" FORCE)
    set(WISDOM_PLATFORM "Mac" CACHE STRING "Platform name" FORCE)
  elseif(UNIX AND NOT APPLE)
    set(WISDOM_WINDOWS FALSE CACHE INTERNAL "Windows build" FORCE)
    set(WISDOM_WINDOWS_STORE FALSE CACHE INTERNAL "Windows store build" FORCE)
    set(WISDOM_MAC FALSE CACHE INTERNAL "Mac build" FORCE)
    set(WISDOM_LINUX TRUE CACHE INTERNAL "Linux build" FORCE)
    set(WISDOM_PLATFORM "Linux" CACHE STRING "Platform name" FORCE)
  endif()
  
  # Detect underlying graphics system
  if(WISDOM_WINDOWS)
    set(WISDOM_DX12 TRUE CACHE BOOL "Use D3D12 as default graphics API" FORCE)
  endif()

  # Detect Vulkan
  find_package(Vulkan QUIET)
  if(Vulkan_FOUND)
    set(WISDOM_VULKAN TRUE CACHE BOOL "Vulkan support detected" FORCE)
  else()
    set(WISDOM_VULKAN FALSE CACHE BOOL "Vulkan support detected" FORCE)
  endif()
endfunction()
