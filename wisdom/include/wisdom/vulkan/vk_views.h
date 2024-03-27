#pragma once
#include <tuple>
#include <wisdom/vulkan/vk_handles.h>
#include <wisvk/vk_loader.hpp>
#include <wisdom/generated/api/api.h>

namespace wis {
using VKFactoryHandle = std::tuple<wis::SharedInstance>;
using VKDeviceHandle = std::tuple<wis::SharedDevice>;
using VKPipelineHandle = std::tuple<wis::SharedPipeline>;


using VKFenceView = std::tuple<VkSemaphore>;
using VKShaderView = std::tuple<VkShaderModule>;
using VKRootSignatureView = std::tuple<VkPipelineLayout>;
using VKCommandListView = std::tuple<VkCommandBuffer>;
using VKQueueView = std::tuple<VkQueue>;
using VKTextureView = std::tuple<VkImage, VkFormat, wis::Size2D>;
using VKBufferView = std::tuple<VkBuffer>;
using VKRenderTargetView = std::tuple<VkImageView, wis::Size2D>;
using VKDescriptorBufferView = std::tuple<VkBuffer, uint32_t>;
} // namespace wis