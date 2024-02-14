#pragma once
#include <tuple>
#include <wisdom/xvulkan/vk_handles.h>
#include <wisvk/vk_loader.hpp>

namespace wis {
using VKAdapterHandle = std::tuple<VkPhysicalDevice, VkInstanceTable*>;
using VKFactoryHandle = std::tuple<wis::shared_handle<VkInstance>, VkInstanceTable*>;
using VKDeviceHandle = std::tuple<wis::SharedDevice>;

using VKFenceView = std::tuple<VkSemaphore>;
using VKShaderView = std::tuple<VkShaderModule>;
using VKRootSignatureView = std::tuple<VkPipelineLayout>;
using VKCommandListView = std::tuple<VkCommandBuffer>;
using VKQueueView = std::tuple<VkQueue>;
} // namespace wis