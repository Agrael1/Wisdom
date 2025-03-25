module;
#define WISDOM_MODULE_DECL
#include <wisdom/x11.include.h>
export module wisdom.x11;

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_x11.hpp>