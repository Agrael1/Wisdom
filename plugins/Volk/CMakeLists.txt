project(Volk)

if(Vulkan_volk_FOUND)
	message(STATUS "Volk found")
	add_library(Volk::Volk ALIAS volk)
else()
	message(STATUS "Volk not found")
endif()