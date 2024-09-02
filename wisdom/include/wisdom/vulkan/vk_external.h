#pragma once
#include <vulkan/vulkan.h>

namespace wis::detail {
constexpr static inline VkExternalSemaphoreHandleTypeFlagsKHR semaphore_handle_type =
#if defined(WIN32)
    VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT
#else // Try FD
    VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT
#endif // defined(WISDOM_WINDOWS)
    ;
constexpr static inline VkExternalMemoryHandleTypeFlagsKHR memory_handle_type =
#if defined(WIN32)
    VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT
#else // Try FD
    VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT
#endif // defined(WISDOM_WINDOWS)
    ;
} // namespace wis::detail