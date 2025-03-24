module;
#define WISDOM_MODULE_DECL
#include <wisdom/xcb.include.h>
#define WISDOM_EXPORT export
export module wisdom.xcb;

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_xcb.hpp>