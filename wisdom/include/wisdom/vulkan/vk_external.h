#pragma once
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/definitions.h>
#include <vulkan/vulkan.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis::detail {
constexpr inline VkExternalSemaphoreHandleTypeFlagsKHR semaphore_handle_type =
#if defined(WIN32)
    VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT
#else // Try FD
    VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT
#endif // defined(WISDOM_WINDOWS)
    ;
constexpr inline VkExternalMemoryHandleTypeFlagsKHR memory_handle_type =
#if defined(WIN32)
    VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT
#else // Try FD
    VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT
#endif // defined(WISDOM_WINDOWS)
    ;
} // namespace wis::detail
