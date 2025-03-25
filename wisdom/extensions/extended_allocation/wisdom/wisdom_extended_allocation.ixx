module;
#define WISDOM_MODULE_DECL
#include <wisdom/extended_allocation.include.h>
export module wisdom.descriptor_buffer;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#include <wisdom/wisdom_extended_allocation.hpp>
