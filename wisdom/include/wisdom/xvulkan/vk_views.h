#pragma once
#include <tuple>
#include <wisdom/xvulkan/vk_handles.h>
#include <wisvk/vk_loader.hpp>

namespace wis {
using VKFactoryHandle = std::tuple<wis::SharedInstance>;
using VKDeviceHandle = std::tuple<wis::SharedDevice>;
using VKPipelineHandle = std::tuple<wis::SharedPipeline>;


using VKFenceView = std::tuple<VkSemaphore>;
using VKShaderView = std::tuple<VkShaderModule>;
using VKRootSignatureView = std::tuple<VkPipelineLayout>;
using VKCommandListView = std::tuple<VkCommandBuffer>;
using VKQueueView = std::tuple<VkQueue>;
using VKTextureView = std::tuple<VkImage>;
using VKBufferView = std::tuple<VkBuffer>;
} // namespace wis