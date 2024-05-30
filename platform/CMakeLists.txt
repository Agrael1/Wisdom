
add_library(wisdom-platform-headers INTERFACE)
add_library(wis::wisdom-platform-headers ALIAS wisdom-platform-headers)

add_library(wisdom-platform INTERFACE)
add_library(wis::wisdom-platform ALIAS wisdom-platform)

if(WISDOM_WINDOWS)
	add_subdirectory("windows")
elseif(WISDOM_LINUX)
	add_subdirectory("xcb")
	add_subdirectory("wayland")
endif()



