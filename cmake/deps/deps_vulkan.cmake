if(NOT vkma_SOURCE_DIR)
  CPMAddPackage(
    NAME vkma
    GITHUB_REPOSITORY GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
    GIT_TAG origin/master
    DOWNLOAD_ONLY TRUE
  )
  set(vkma_SOURCE_DIR ${vkma_SOURCE_DIR} CACHE INTERNAL "")
else()
  message("Vulkan Memory Allocator found, skipping download.")
endif()

# Generate a cpp file that includes the implementation
if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/vma.cpp)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/vma.cpp
       "#define VMA_IMPLEMENTATION\n#include \"vk_mem_alloc.h\"\n")
endif()

add_library(vkma STATIC ${vkma_SOURCE_DIR}/include/vk_mem_alloc.h ${CMAKE_CURRENT_BINARY_DIR}/vma.cpp)
target_link_libraries(vkma PUBLIC Vulkan::Headers)
target_compile_definitions(
  vkma PRIVATE VK_NO_PROTOTYPES VMA_STATIC_VULKAN_FUNCTIONS=0
                          VMA_DYNAMIC_VULKAN_FUNCTIONS=0)
if(WISDOM_WINDOWS)
  target_compile_definitions(vkma PUBLIC VK_USE_PLATFORM_WIN32_KHR
                                                    VMA_EXTERNAL_MEMORY_WIN32)
endif(WISDOM_WINDOWS)

target_include_directories(
  vkma PUBLIC $<BUILD_INTERFACE:${vkma_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vkma>)
set_target_properties(vkma PROPERTIES CXX_STANDARD 20
                                                 POSITION_INDEPENDENT_CODE ON)
target_compile_options(
  vkma
  PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wno-nullability-completeness>)

install(
  TARGETS vkma
  EXPORT wisdom-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${vkma_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vkma)
