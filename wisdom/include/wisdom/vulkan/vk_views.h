#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/api/api_common.h>
#endif

WIS_EXPORT namespace wis
{
	using VKAdapterView = vk::PhysicalDevice;
	using VKDeviceView = wis::shared_handle<vk::Device>;
	using VKBufferView = vk::Buffer;
	using VKTextureView = struct VKTextureView{ vk::Image image; vk::Format format; };
	using VKFenceView = vk::Semaphore;
	using VKCommandQueueView = vk::Queue;
	using VKCommandListView = vk::CommandBuffer;
	using VKPipelineStateView = vk::Pipeline;
	using VKRootSignatureView = vk::PipelineLayout;
	using VKRenderPassView = struct VKRenderPassView{ vk::RenderPass pass; vk::Framebuffer frame; Size2D frame_size; };
}