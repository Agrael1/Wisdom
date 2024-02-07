# Check the platform
if(WIN32)
  set(WISDOM_WINDOWS 1)
  set(WISDOM_WINDOWS_STORE ${WINDOWS_STORE})  
elseif(APPLE)
  set(WISDOM_MAC 1)
elseif(UNIX AND NOT APPLE)
  set(WISDOM_LINUX 1)
endif()

option(WISDOM_USE_WAYLAND "Use Wayland window system" ON)
option(WISDOM_USE_X11 "Use X11 window system" ON)

if(WISDOM_LINUX)
  set(WISDOM_WAYLAND ${WISDOM_USE_WAYLAND})
  set(WISDOM_X11 ${WISDOM_USE_X11})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "iOS"
   OR CMAKE_SYSTEM_NAME STREQUAL "tvOS"
   AND NOT WISDOM_FORCE_VULKAN)
  message("Platform Library is Metal")
  set(WISDOMMTL TRUE)
  message(FATAL_ERROR "Metal is not implemented")
endif()