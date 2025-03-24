module;
#define WISDOM_MODULE_DECL
#define WISVK_MODULE_DECL
#include <wisdom/descriptor_buffer.include.h>
#define WISDOM_EXPORT export
export module wisdom.descriptor_buffer;

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
#include <wisdom/generated/vk_functions.hpp>
#include <wisdom/vk_descriptor_buffer.hpp>
#endif

#include <wisdom/wisdom_descriptor_buffer.hpp>
