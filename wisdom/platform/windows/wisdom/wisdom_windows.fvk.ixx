module;
#define WISDOM_MODULE_DECL
#define WISDOM_FORCE_VULKAN
#include <wisdom/windows.include.h>
export module wisdom.windows.fvk;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_windows.hpp>
