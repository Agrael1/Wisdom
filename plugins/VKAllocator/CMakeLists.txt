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
target_sources(${PROJECT_NAME} PRIVATE vma.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Headers)
target_compile_definitions(
  ${PROJECT_NAME} PRIVATE VK_NO_PROTOTYPES VMA_STATIC_VULKAN_FUNCTIONS=0
                          VMA_DYNAMIC_VULKAN_FUNCTIONS=0)
if(WISDOM_WINDOWS)
  target_compile_definitions(${PROJECT_NAME} PUBLIC VK_USE_PLATFORM_WIN32_KHR
                                                    VMA_EXTERNAL_MEMORY_WIN32)
endif(WISDOM_WINDOWS)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${vkma_SOURCES}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vkma>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                 POSITION_INDEPENDENT_CODE ON)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT wisdom-vk-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${vkma_SOURCES}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vkma)
