#ifndef WIS_VK_VIEWS_H
#define WIS_VK_VIEWS_H
#ifndef WISDOM_MODULE_DECL
#include <tuple>
#include <span>
#include <wisdom/vulkan/vk_handles.h>
#include <wisvk/vk_loader.hpp>
#include <wisdom/generated/api/api.hpp>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
using VKFactoryHandle = std::tuple<wis::SharedInstance>;
using VKDeviceHandle = std::tuple<wis::SharedDevice>;

using VKFenceView = std::tuple<VkSemaphore>;
using VKShaderView = std::tuple<VkShaderModule>;
using VKRootSignatureView = std::tuple<VkPipelineLayout>;
using VKRootSignatureView2 = std::tuple<VkDescriptorSetLayout*>;
using VKCommandListView = std::tuple<VkCommandBuffer>;
using VKQueueView = std::tuple<VkQueue>;
using VKTextureView = std::tuple<VkImage, VkFormat, wis::Size2D>;
using VKBufferView = std::tuple<VkBuffer>;
using VKRenderTargetView = std::tuple<VkImageView, wis::Size2D>;
using VKSamplerView = std::tuple<VkSampler>;
using VKShaderResourceView = std::tuple<VkImageView>;
using VKUnorderedAccessTextureView = std::tuple<VkImageView>;
using VKMemoryView = std::tuple<VmaAllocator, VmaAllocation>;
using VKPipelineView = std::tuple<VkPipeline>;
using VKDescriptorStorageView = std::tuple<std::span<const VkDescriptorSet>>;
using VKAcceleratedGeometryDesc = std::pair<VkAccelerationStructureGeometryKHR, VkAccelerationStructureBuildRangeInfoKHR>;
using VKAccelerationStructureView = std::tuple<VkAccelerationStructureKHR>;
} // namespace wis

#endif // VK_VIEWS_H
