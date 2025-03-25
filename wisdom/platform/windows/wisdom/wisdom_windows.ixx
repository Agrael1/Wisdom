module;
#define WISDOM_MODULE_DECL
#include <wisdom/windows.include.h>
export module wisdom.windows;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_windows.hpp>
