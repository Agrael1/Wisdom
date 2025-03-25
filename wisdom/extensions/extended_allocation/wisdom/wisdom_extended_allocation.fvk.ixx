module;
#define WISDOM_MODULE_DECL
#define WISDOM_FORCE_VULKAN
#include <wisdom/extended_allocation.include.h>
export module wisdom.descriptor_buffer.fvk;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_extended_allocation.hpp>
