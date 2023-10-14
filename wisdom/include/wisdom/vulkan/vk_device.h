#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_render_pass.h>
#include <wisdom/vulkan/vk_state_builder.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_descriptor_heap.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/global/definitions.h>
#include <bitset>
#include <wisdom/util/small_allocator.h>
#endif

namespace wis {
class VKDevice;

template<>
class Internal<VKDevice>
{
public:
    wis::shared_handle<vk::Instance> instance;
    wis::shared_handle<vk::Device> device;
    vk::PhysicalDevice adapter;
};

WIS_EXPORT class VKDevice : public QueryInternal<VKDevice>
{
    constexpr static inline std::array required_extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_RAY_QUERY_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_MAINTENANCE3_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
        VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
        VK_KHR_MAINTENANCE1_EXTENSION_NAME,
        VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
        VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME,
        VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
        VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        VK_NV_MESH_SHADER_EXTENSION_NAME,
        VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
        VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
        VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
    };
    static inline constexpr const auto max_count = 4;
    struct QueueInfo {
        uint32_t index;
        uint32_t count;
    };
    struct QueueFormat {
    public:
        QueueFormat& operator=(QueueFormat&& o) noexcept
        {
            queue_flags = o.queue_flags;
            count = o.count;
            family_index = o.family_index;
            return *this;
        }

        uint8_t GetNextInLine() const noexcept
        {
            return last.exchange((last + 1) % count);
        }
        bool Empty() const noexcept
        {
            return count == 0u;
        }

        uint16_t queue_flags = 0;
        uint8_t count = 0;
        uint8_t family_index = 0;
        mutable std::atomic<uint8_t> last{ 0 };
    };
    enum class QueueTypes : uint8_t {
        Graphics,
        Compute,
        Copy,
        VideoDecode
    };
    struct QueueResidency {
        static constexpr size_t QueueIndex(QueueType type)
        {
            switch (type) {
            case wis::QueueType::Compute:
                return +QueueTypes::Compute;
            case wis::QueueType::Copy:
                return +QueueTypes::Copy;
            case wis::QueueType::VideoDecode:
                return +QueueTypes::VideoDecode;
            default:
                return +QueueTypes::Graphics;
            }
        }
        static constexpr size_t QueueFlag(QueueTypes type)
        {
            using enum vk::QueueFlagBits;
            switch (type) {
            case QueueTypes::Copy:
                return +eTransfer;
            case QueueTypes::Compute:
                return +eCompute;
            case QueueTypes::Graphics:
                return +eGraphics;
            case QueueTypes::VideoDecode:
                return +eVideoDecodeKHR;
            default:
                return 0;
            }
        }

    public:
        const QueueFormat* GetOfType(QueueType type) const
        {
            auto idx = QueueIndex(type);
            const auto* q = &available_queues[idx];

            if (q->count == 0u) {
                idx = FindResembling(QueueTypes(idx));
                if (idx == -1)
                    return nullptr;
                q = &available_queues[idx];
            }
            return q;
        }
        int32_t FindResembling(QueueTypes type) const
        {
            for (size_t i = 0; i < max_count; i++) {
                const auto& r = available_queues[i];
                if ((r.queue_flags & QueueFlag(type)) != 0u)
                    return static_cast<int>(i);
            }
            return -1;
        }

        std::array<QueueFormat, max_count> available_queues{};
    };

public:
    VKDevice() = default;
    explicit VKDevice(VKFactoryHandle factory, VKAdapterView adapter) noexcept
    {
        Initialize(std::move(factory), adapter);
    }
    operator bool() const noexcept
    {
        return bool(device);
    }

    WIS_INLINE bool Initialize(VKFactoryHandle factory, VKAdapterView adapter) noexcept;

    [[nodiscard]] WIS_INLINE
            VKSwapChain
            CreateSwapchain(
                    VKCommandQueueView render_queue,
                    wis::SwapchainOptions options,
                    wis::SurfaceParameters xsurface,
                    bool vsync = false) const noexcept;

public:
    [[nodiscard]] VKCommandQueue
    CreateCommandQueue(QueueOptions options = QueueOptions{}) const noexcept
    {
        const auto* queue = queues.GetOfType(options.type);
        if (queue == nullptr)
            return {};

        vk::DeviceQueueInfo2 info{
            {},
            queue->family_index,
            queue->GetNextInLine()
        };
        return VKCommandQueue{ device->getQueue2(info) };
    }

    [[nodiscard]] VKCommandList
    CreateCommandList(QueueType list_type) const noexcept
    {
        vk::CommandPoolCreateInfo cmd_pool_create_info{
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queues.GetOfType(list_type)->family_index
        };
        auto [result, ca] = device->createCommandPool(cmd_pool_create_info); // allocator
        if (!succeeded(result))
            return {};

        vk::CommandBufferAllocateInfo cmd_buf_alloc_info{
            ca, vk::CommandBufferLevel::ePrimary, 1
        };

        auto [result2, cb] = device->allocateCommandBuffers(cmd_buf_alloc_info);

        return VKCommandList{ wis::shared_handle<vk::CommandPool>{ ca, device },
                              vk::CommandBuffer{ cb.at(0) } };
    }

    [[nodiscard]] VKFence
    CreateFence(uint64_t initial_value = 0ull) const noexcept
    {
        vk::SemaphoreTypeCreateInfo timeline_desc{
            vk::SemaphoreType::eTimeline,
            initial_value
        };
        vk::SemaphoreCreateInfo desc{
            {}, &timeline_desc
        };
        auto [result, sem] = device->createSemaphore(desc);
        return succeeded(result) ? VKFence{ wis::shared_handle<vk::Semaphore>{ sem, device } } : VKFence{};
    }

    [[nodiscard]] WIS_INLINE VKRenderPass
    CreateRenderPass(Size2D frame_size, std::span<const ColorAttachment> rtv_descs,
                     DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
                     SampleCount samples = SampleCount::s1,
                     DataFormat vrs_format = DataFormat::unknown) const noexcept;

    [[nodiscard]] VKRootSignature CreateRootSignature(std::span<VKDescriptorSetLayout> layouts = {}) const noexcept
    {
        wis::internals::uniform_allocator<vk::DescriptorSetLayout> allocator;

        for (auto& i : layouts) {
            allocator.allocate(i.GetInternal().layout.get());
        }
        vk::PipelineLayoutCreateInfo pipeline_layout_info{
            vk::PipelineLayoutCreateFlags{}, uint32_t(allocator.size()), allocator.data(), 0, nullptr
        };

        auto [result, layout] = device->createPipelineLayout(pipeline_layout_info);
        return succeeded(result) ? VKRootSignature{ wis::shared_handle<vk::PipelineLayout>{ layout, device } } : VKRootSignature{};
    }

    [[nodiscard]] WIS_INLINE
            VKPipelineState
            CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc& desc, std::span<const InputLayoutDesc> input_layout) const noexcept;

    [[nodiscard]] VKShader CreateShader(wis::shared_blob blob, ShaderType type) const noexcept
    {
        vk::ShaderModuleCreateInfo desc{
            vk::ShaderModuleCreateFlags{},
            blob.size(),
            blob.data<uint32_t>()
        };
        auto [result, module] = device->createShaderModule(desc);
        return succeeded(result) ? VKShader{ wis::shared_handle<vk::ShaderModule>{ module, device }, type } : VKShader{};
    }

    [[nodiscard]] VKRenderTarget
    CreateRenderTarget(VKTextureView texture, wis::DataFormat format, RenderTargetSelector range = {}) const noexcept
    {
        auto vk_format = convert_vk(format);
        vk::ImageViewCreateInfo desc;
        {
            desc.image = texture.image,
            desc.format = vk_format;
        };

        switch (range.type) {
        case TextureType::Texture1D:
            desc.viewType = vk::ImageViewType::e1D;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = range.mip,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = 0,
                desc.subresourceRange.layerCount = 1;
            };
            break;
        case TextureType::Texture2D:
            desc.viewType = vk::ImageViewType::e2D;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = range.mip,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = 0,
                desc.subresourceRange.layerCount = 1;
            };
            break;
        case TextureType::Texture3D:
            desc.viewType = vk::ImageViewType::e3D;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = range.mip,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = range.base_layer,
                desc.subresourceRange.layerCount = range.extent_layers;
            };
            break;
        case TextureType::Texture1DArray:
            desc.viewType = vk::ImageViewType::e1DArray;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = range.mip,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = range.base_layer,
                desc.subresourceRange.layerCount = range.extent_layers;
            };
            break;
        case TextureType::Texture2DArray:
            desc.viewType = vk::ImageViewType::e2DArray;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = range.mip,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = range.base_layer,
                desc.subresourceRange.layerCount = range.extent_layers;
            };
            break;
        case TextureType::Texture2DMS:
            desc.viewType = vk::ImageViewType::e2D;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = 0,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = 0,
                desc.subresourceRange.layerCount = 1;
            };
            break;
        case TextureType::Texture2DMSArray:
            desc.viewType = vk::ImageViewType::e2DArray;
            {
                desc.subresourceRange.aspectMask = aspect_flags(vk_format),
                desc.subresourceRange.baseMipLevel = 0,
                desc.subresourceRange.levelCount = 1,
                desc.subresourceRange.baseArrayLayer = range.base_layer,
                desc.subresourceRange.layerCount = range.extent_layers;
            };
            break;
        default:
            break;
        }

        auto [result, value] = device->createImageView(desc);
        return succeeded(result)
                ? VKRenderTarget{ shared_handle<vk::ImageView>{ value, device } }
                : VKRenderTarget{};
    }

    [[nodiscard]] VKDepthStencil
    CreateDepthStencil(VKTextureView texture, wis::DataFormat format) const noexcept
    {
        return CreateRenderTarget(texture, format);
    }

    /// @brief Create a Descriptor heap object, where descriptors can be allocated from
    /// @param num_descs maximum number of descriptors that can be allocated from this heap
    /// @param type Type of descriptors that can be allocated from this heap
    /// @return Descriptor heap object
    [[nodiscard]] VKDescriptorHeap CreateDescriptorHeap(uint32_t num_descs, PoolType type) const noexcept
    {
        if (type == PoolType::CBV_SRV_UAV) {
            vk::DescriptorPoolSize size_desc{
                vk::DescriptorType::eMutableVALVE, num_descs
            };
            vk::DescriptorPoolCreateInfo pool_desc{
                vk::DescriptorPoolCreateFlags{}, num_descs, 1u, &size_desc
            };
            auto [result, pool] = device->createDescriptorPool(pool_desc);
            return succeeded(result) ? VKDescriptorHeap{ shared_handle<vk::DescriptorPool>{ pool, device } } : VKDescriptorHeap{};
        }

        vk::DescriptorPoolSize size_desc{
            vk::DescriptorType::eSampler, num_descs
        };
        vk::DescriptorPoolCreateInfo pool_desc{
            vk::DescriptorPoolCreateFlags{}, num_descs, 1u, &size_desc
        };
        auto [result, pool] = device->createDescriptorPool(pool_desc);
        return succeeded(result) ? VKDescriptorHeap{ shared_handle<vk::DescriptorPool>{ pool, device } } : VKDescriptorHeap{};
    }

    [[nodiscard]] VKDescriptorSetLayout CreateDescriptorSetLayout(std::span<BindingDescriptor> descs) const noexcept
    {
        static constexpr auto max_layout_bindings = 32;
        wis::internals::uniform_allocator<vk::DescriptorSetLayoutBinding, max_layout_bindings> bindings;

        constexpr static vk::DescriptorType cbvSrvUavTypes[] = {
            vk::DescriptorType::eSampledImage,
            vk::DescriptorType::eStorageImage,
            vk::DescriptorType::eUniformTexelBuffer,
            vk::DescriptorType::eStorageTexelBuffer,
            vk::DescriptorType::eUniformBuffer,
            vk::DescriptorType::eStorageBuffer,
            // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR /* Need to check support if this is desired. */
        };

        constexpr static vk::MutableDescriptorTypeListVALVE cbvSrvUavTypeList{
            sizeof(cbvSrvUavTypes) / sizeof(VkDescriptorType),
            cbvSrvUavTypes
        };

        constexpr static vk::MutableDescriptorTypeCreateInfoEXT mutableTypeInfo{
            1u, &cbvSrvUavTypeList
        };

        for (auto& desc : descs) {
            bindings.allocate(vk::DescriptorSetLayoutBinding{
                    desc.binding, vk::DescriptorType::eMutableVALVE, desc.count, vk::ShaderStageFlagBits(desc.stages) });
        }
        vk::DescriptorSetLayoutCreateInfo desc{
            vk::DescriptorSetLayoutCreateFlags{}, uint32_t(bindings.size()), bindings.data(), &mutableTypeInfo
        };
        auto [result, value] = device->createDescriptorSetLayout(desc);
        return succeeded(result)
                ? VKDescriptorSetLayout{ shared_handle<vk::DescriptorSetLayout>{ value, device } }
                : VKDescriptorSetLayout{};
    }

    void CreateConstantBufferView(VKBufferView buffer, uint32_t size, VKDescriptorSetView set, VKDescriptorSetLayoutView, uint32_t binding = 0) const noexcept
    {
        vk::DescriptorBufferInfo desc{
            buffer, 0, size
        };
        vk::WriteDescriptorSet write{
            set, binding, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &desc, nullptr
        };
        device->updateDescriptorSets(write, nullptr);
    }

private:
    WIS_INLINE void GetQueueFamilies() noexcept;

    [[nodiscard]] WIS_INLINE
            wis::internals::uniform_allocator<const char*, required_extensions.size()>
            RequestExtensions() noexcept;

    [[nodiscard]] WIS_INLINE
            vk::PresentModeKHR
            GetPresentMode(vk::SurfaceKHR surface, bool vsync) const noexcept;

    WIS_INLINE void FillShaderStages(
            const VKGraphicsPipelineDesc& desc,
            wis::internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages>& shader_stages) const noexcept;

    QueueResidency queues{};
    bool vrs_supported : 1 = false;
    bool mesh_shader_supported : 1 = false;
    bool ray_tracing_supported : 1 = false;
    bool ray_query_supported : 1 = false;
    bool draw_indirect_supported : 1 = false;
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_device.inl"
#endif
