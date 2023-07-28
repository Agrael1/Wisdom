project(VKHpp)

if (NOT PLUGINS_LOADED)
    message("Loading vulkan-hpp headers...")
	FetchContent_Declare(
	  vulkanhpp
      GIT_REPOSITORY https://github.com/Agrael1/Vulkan-Hpp
      GIT_TAG main
      GIT_SUBMODULES ""
	)
	FetchContent_GetProperties(vulkanhpp)
	if(NOT vulkanhpp_POPULATED)
	  FetchContent_Populate(vulkanhpp)
	endif()
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(${PROJECT_NAME})
