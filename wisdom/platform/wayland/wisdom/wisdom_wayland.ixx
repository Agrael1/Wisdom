module;
#define WISDOM_MODULE_DECL
#include <wisdom/wayland.include.h>
export module wisdom.wayland;

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_wayland.hpp>