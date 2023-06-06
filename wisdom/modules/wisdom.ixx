module;
#pragma warning(disable: 5244) //includes are interop, and not a mistake
#define WISDOM_MODULES
export module wisdom;

export import wisdom.api;
export import wisdom.dx12;

#if defined(WISDOM_VULKAN_FOUND)
export import wisdom.vk;
#endif

#include <wisdom/wisdom.h>