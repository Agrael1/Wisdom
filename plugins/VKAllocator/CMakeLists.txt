project(VKAllocator)

if(NOT PLUGINS_LOADED)
  message("Setting up Vulkan Allocator...")
  FetchContent_Declare(
    vkma
    GIT_REPOSITORY
      https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG origin/master)
  FetchContent_GetProperties(vkma)
  if(NOT vkma_POPULATED)
    FetchContent_Populate(vkma)
  endif()
  set(vkma_SOURCES
      ${vkma_SOURCE_DIR}
      CACHE INTERNAL "")
endif()

add_library(${PROJECT_NAME} STATIC ${vkma_SOURCES}/include/vk_mem_alloc.h)
target_sources(${PROJECT_NAME} PRIVATE ${vkma_SOURCES}/src/VmaUsage.cpp
                                       ${vkma_SOURCES}/src/VmaUsage.h)

if(MSVC)
  # Provides MSVC users nicer debugging support
  target_sources(${PROJECT_NAME}
                 PRIVATE ${vkma_SOURCES}/src/vk_mem_alloc.natvis)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
	${Vulkan_INCLUDE_DIRS}
)
target_compile_definitions(
  ${PROJECT_NAME} PUBLIC VK_NO_PROTOTYPES
)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${vkma_SOURCES}/include>
                         $<INSTALL_INTERFACE:include>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_install(${PROJECT_NAME})
