module;
#include <wisdom/debug.include.h>
#define WISDOM_MODULE_DECL
#define WISDOM_FORCE_VULKAN
#define WISDOM_EXPORT export
export module wisdom.debug.fvk;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif



#include <wisdom/wisdom_debug.hpp>
