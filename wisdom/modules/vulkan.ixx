module;
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR
#include <ranges>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include <wisdom/global/definitions.h>
#include <coroutine>
#include <utility>
#include <atomic>
#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

#define WISDOM_MODULES
#pragma warning(disable : 5244) // includes are interop, and not a mistake
export module wisdom.vk;

import wisdom.api;
#include <wisdom/vulkan/vk_dynamic_loader.h>
#include <wisdom/vulkan/vk_format.h>
#include <wisdom/vulkan/vk_xshared_handle.h>

#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_views.h>

#include <wisdom/vulkan/vk_adapter.h>

#include <wisdom/vulkan/vk_buffer_views.h>
#include <wisdom/vulkan/vk_resource.h>

#include <wisdom/vulkan/vk_rtv.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_shader.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_command_queue.h>
#include <wisdom/vulkan/vk_render_pass.h>
#include <wisdom/vulkan/vk_state_builder.h>

#include <wisdom/vulkan/vk_factory.h>

#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_device.h>
