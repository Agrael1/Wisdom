module;
#define WISDOM_MODULE_DECL
#include <wisdom/xcb.include.h>
export module wisdom.xcb;

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_xcb.hpp>