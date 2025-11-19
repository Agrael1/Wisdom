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

add_library(${PROJECT_NAME} STATIC ${vkma_SOURCE_DIR}/include/vk_mem_alloc.h ${CMAKE_CURRENT_BINARY_DIR}/vma.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Headers)
target_compile_definitions(
  ${PROJECT_NAME} PRIVATE VK_NO_PROTOTYPES VMA_STATIC_VULKAN_FUNCTIONS=0
                          VMA_DYNAMIC_VULKAN_FUNCTIONS=0)
if(WISDOM_WINDOWS)
  target_compile_definitions(${PROJECT_NAME} PUBLIC VK_USE_PLATFORM_WIN32_KHR
                                                    VMA_EXTERNAL_MEMORY_WIN32)
endif(WISDOM_WINDOWS)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${vkma_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vkma>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                 POSITION_INDEPENDENT_CODE ON)
target_compile_options(
  ${PROJECT_NAME}
  PUBLIC $<$<CXX_COMPILER_ID:Clang>:-Wno-nullability-completeness>
         $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT wisdom-vk-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${vkma_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vkma)



# wisvk utils
message("Loading wisvk utils...")

# if release build set WISVK_ONLY_HEADERS to true
if(NOT WISDOM_GENERATE_FUNCTIONS)
  set(WISVK_ONLY_HEADERS TRUE)
endif()

# disable tinyxml2 tests
if(WISDOM_LOAD_SPEC AND NOT WISDOM_WISVK_SPEC_LOADED)
  set(WISVK_LOAD_SPEC TRUE)
  set(WISDOM_WISVK_SPEC_LOADED TRUE CACHE INTERNAL "")
endif()

CPMAddPackage(
  NAME wisvk
  GITHUB_REPOSITORY Agrael1/Wisdom-VkUtils
  GIT_TAG origin/master)