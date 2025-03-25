module;
#define WISDOM_MODULE_DECL
#define WISDOM_FORCE_VULKAN
#include <wisdom/descriptor_buffer.include.h>
export module wisdom.descriptor_buffer.fvk;

#if defined(WISDOM_DX12)
import wisdom.dx12;
#endif

#if defined(WISDOM_VULKAN)
import wisdom.vk;
#endif

#if defined(WISDOM_DX12)
#include <wisdom/dx12_descriptor_buffer.hpp>
#endif

#if defined(WISDOM_VULKAN)
#include <wisdom/vk_descriptor_buffer.hpp>
#endif

#include <wisdom/wisdom_descriptor_buffer.hpp>
