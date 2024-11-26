#ifndef WIS_VK_DEVICE_H
#define WIS_VK_DEVICE_H
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_adapter.h>
#include <wisdom/vulkan/vk_queue_residency.h>
#include <wisdom/vulkan/vk_command_queue.h>
#include <wisdom/vulkan/vk_command_list.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_shader.h>
#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_descriptor_storage.h>
#include <wisdom/vulkan/vk_device_ext.h>
#include <wisdom/generated/vulkan/vk_structs.hpp>

namespace wis {
class VKDevice;

template<>
struct Internal<VKDevice> {
    wis::VKAdapter adapter;
    wis::SharedDevice device;
    wis::VKDeviceExtensionEmbedded1 ext1;

    wis::shared_handle<VmaAllocator> allocator;
    detail::QueueResidency queues;

    struct DefaultLayout {
        constexpr static std::array<VkDescriptorType, Internal<VKDescriptorStorage>::max_sets> desc_types{
            VK_DESCRIPTOR_TYPE_SAMPLER,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
            VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        };

        std::array<VkDescriptorSetLayout, Internal<VKDescriptorStorage>::max_sets> desc_sets{};

    public:
        bool operator==(const DefaultLayout& o) const noexcept
        {
            return std::memcmp(desc_sets.data(), o.desc_sets.data(), sizeof(desc_sets)) == 0;
        }
        bool Valid() const noexcept
        {
            return *this != DefaultLayout{};
        }
        void Destroy(PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout, VkDevice device) noexcept
        {
            if (Valid()) {
                for (auto& set : desc_sets) {
                    vkDestroyDescriptorSetLayout(device, set, nullptr);
                }
            }
        }
        wis::Result Init(PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout, VkDevice device)
        {
            constexpr static size_t num_sets = Internal<VKDescriptorStorage>::max_sets;
            constexpr static VkDescriptorSetLayoutBinding bindings[num_sets] {
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
                    .descriptorCount = 2048,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .descriptorCount = 4096,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                    .descriptorCount = 4096,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .descriptorCount = 4096,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .descriptorCount = 4096,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
                {   .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    .descriptorCount = 4096,
                    .stageFlags = VK_SHADER_STAGE_ALL
                },
            };

            constexpr static VkDescriptorBindingFlags flags = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT | VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT;
            constexpr static VkDescriptorSetLayoutBindingFlagsCreateInfoEXT binding_flags_info{
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT,
                .pNext = nullptr,
                .bindingCount = 1,
                .pBindingFlags = &flags,
            };

            std::array<VkDescriptorSetLayoutCreateInfo, num_sets> desc_info{};
            for (size_t i = 0; i < num_sets; i++) {
                desc_info[i] = {
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                    .pNext = &binding_flags_info,
                    .flags = 0,
                    .bindingCount = 1,
                    .pBindings = &bindings[i],
                };
            }
            for (size_t i = 0; i < num_sets; i++) {
                auto res = vkCreateDescriptorSetLayout(device, &desc_info[i], nullptr, &desc_sets[i]);
                if (!succeeded(res))
                    return wis::make_result<FUNC, "Failed to create a descriptor set layout">(res);
            }
            return wis::success;
        }
    } default_layout;

public:
    Internal() noexcept = default;
    Internal(wis::VKAdapter adapter, wis::SharedDevice device, wis::VKDeviceExtensionEmbedded1 ext1) noexcept
        : adapter(std::move(adapter)), device(std::move(device)), ext1(std::move(ext1))
    {
    }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        if (device.get()) {
            default_layout.Destroy(device.table().vkDestroyDescriptorSetLayout, device.get());
        }

        adapter = std::move(o.adapter);
        device = std::move(o.device);
        ext1 = std::move(o.ext1);
        allocator = std::move(o.allocator);
        queues = std::move(o.queues);
        std::memcpy(&default_layout, &o.default_layout, sizeof(default_layout));
        std::memset(&o.default_layout, 0, sizeof(default_layout));
        return *this;
    }
    ~Internal() noexcept
    {
        if (device.get()) {
            default_layout.Destroy(device.table().vkDestroyDescriptorSetLayout, device.get());
        }
    }

public:
    auto& GetInstanceTable() const noexcept
    {
        return adapter.GetInternal().instance.table();
    }
    auto& GetDeviceTable() const noexcept
    {
        return device.table();
    }

    template<typename PFN>
    [[nodiscard]] PFN GetInstanceProcAddr(const char* name) const noexcept
    {
        return reinterpret_cast<PFN>(device.gtable().vkGetInstanceProcAddr(adapter.GetInternal().instance.get(), name));
    }
    template<typename PFN>
    [[nodiscard]] PFN GetDeviceProcAddr(const char* name) const noexcept
    {
        return device.GetDeviceProcAddr<PFN>(name);
    }

    wis::Result InitDefaultLayout() noexcept
    {
        return default_layout.Init(device.table().vkCreateDescriptorSetLayout, device.get());
    }
};

class ImplVKDevice : public QueryInternal<VKDevice>
{
    friend wis::ResultValue<wis::VKDevice>
    ImplVKCreateDevice(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

public:
    ImplVKDevice() noexcept = default;
    WIS_INLINE explicit ImplVKDevice(wis::SharedDevice device,
                                     wis::VKAdapter adapter,
                                     wis::VKDeviceExtensionEmbedded1 ext1) noexcept;

    operator bool() const noexcept
    {
        return bool(device);
    }
    operator VKDeviceHandle() const noexcept
    {
        return device;
    }

public:
    WIS_INLINE wis::Result
    WaitForMultipleFences(const VKFenceView* fences,
                          const uint64_t* values,
                          uint32_t count,
                          MutiWaitFlags wait_all = MutiWaitFlags::All,
                          uint64_t timeout = std::numeric_limits<uint64_t>::max()) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKFence>
    CreateFence(uint64_t initial_value = 0ull, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKCommandQueue>
    CreateCommandQueue(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKCommandList>
    CreateCommandList(wis::QueueType type) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKPipelineState>
    CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKShader>
    CreateShader(void* bytecode, uint32_t size) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKResourceAllocator>
    CreateAllocator() const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRenderTarget>
    CreateRenderTarget(VKTextureView texture, wis::RenderTargetDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRenderTarget>
    CreateDepthStencilTarget(VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return CreateRenderTarget(texture, desc);
    }

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKSampler>
    CreateSampler(const wis::SamplerDesc* desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<VKShaderResource>
    CreateShaderResource(VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE bool
    QueryFeatureSupport(wis::DeviceFeature feature) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDescriptorStorage>
    CreateDescriptorStorage(wis::DescriptorStorageDesc desc) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRootSignature>
    CreateRootSignature(const PushConstant* constants = nullptr,
                        uint32_t constants_size = 0,
                        const PushDescriptor* push_descriptors = nullptr,
                        uint32_t push_descriptors_size = 0,
                        [[maybe_unused]] uint32_t space_overlap_count = 1) const noexcept;

    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKRootSignature>
    CreateRootSignature2(const wis::PushConstant* push_constants = nullptr,
                         uint32_t constants_count = 0,
                         const wis::PushDescriptor* push_descriptors = nullptr,
                         uint32_t push_descriptors_count = 0,
                         const wis::DescriptorSpacing* descriptor_spacing = nullptr) const noexcept;

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKSwapChain>
    VKCreateSwapChain(wis::SharedSurface surface, const SwapchainDesc* desc, VkQueue graphics_queue, void* pNext = nullptr) const noexcept;

protected:
    [[nodiscard]] WIS_INLINE wis::ResultValue<wis::shared_handle<VmaAllocator>>
    VKCreateAllocator(bool interop = false) const noexcept;
};

#pragma region VKDevice
/**
 * @brief Represents logical device.
 * Creates all the resources and commands for rendering.
 * */
class VKDevice : public wis::ImplVKDevice
{
public:
    using wis::ImplVKDevice::ImplVKDevice;

public:
    /**
     * @brief Waits on multiple fences simultaneously.
     * If wait_all is wis::MutiWaitFlags::All, waits for all fences to be signaled.
     * Otherwise waits for any fence to be signaled.
     * @param fences Array of fence views to wait on.
     * @param fence_values Fence values to wait fences to reach.
     * Array must have fence_count values.
     * @param fence_count How many fences to wait on.
     * @param wait_all Specifies the kind of wait.
     * All - waits for all fences to be signaled.
     * Any - waits for any fence to be signaled.
     * Default is wis::MutiWaitFlags::All
     * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
     * */
    [[nodiscard]] inline wis::Result WaitForMultipleFences(const wis::VKFenceView* fences, const uint64_t* fence_values, uint32_t fence_count, wis::MutiWaitFlags wait_all = wis::MutiWaitFlags::All, uint64_t timeout = UINT64_MAX) const noexcept
    {
        return wis::ImplVKDevice::WaitForMultipleFences(fences, fence_values, fence_count, wait_all, timeout);
    }
    /**
     * @brief Creates a fence with initial value and flags.
     * @param initial_value The initial value of the fence.
     * @param flags The flags of the fence.
     * @return wis::VKFence on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKFence> CreateFence(uint64_t initial_value = 0, wis::FenceFlags flags = wis::FenceFlags::None) const noexcept
    {
        return wis::ImplVKDevice::CreateFence(initial_value, flags);
    }
    /**
     * @brief Creates a command queue with specified type.
     * @param type The type of the queue to create.
     * @return wis::VKCommandQueue on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandQueue> CreateCommandQueue(wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandQueue(type);
    }
    /**
     * @brief Creates a command list for specific queue type.
     * @param type The type of the queue to create the command list for.
     * @return wis::VKCommandList on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKCommandList> CreateCommandList(wis::QueueType type) const noexcept
    {
        return wis::ImplVKDevice::CreateCommandList(type);
    }
    /**
     * @brief Creates a graphics pipeline state object.
     * @param desc The description of the graphics pipeline to create.
     * @return wis::VKPipelineState on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKPipelineState> CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept
    {
        return wis::ImplVKDevice::CreateGraphicsPipeline(desc);
    }
    /**
     * @brief Creates a root signature object for use with DescriptorStorage.
     * @param push_constants The root constants to create the root signature with.
     * @param constants_count The number of push constants. Max is 5.
     * @param push_descriptors The root descriptors to create the root signature with.
     * In shader will appear in order of submission. e.g. push_descriptors[5] is [[vk::binding(5,0)]] ... : register(b5/t5/u5)
     * @param descriptors_count The number of push descriptors. Max is 8.
     * @param space_overlap_count Count of descriptor spaces to overlap for each of the DescriptorStorage types.
     * Default is 1. Max is 16. This is used primarily for descriptor type aliasing.
     * Example: If wis::VKDevice is 2, that means that 2 descriptor spaces will be allocated for each descriptor type.
     *     [[vk::binding(0,0)]] SamplerState samplers: register(s0,space1); // space1 can be used for different type of samplers e.g. SamplerComparisonState
     *     [[vk::binding(0,0)]] SamplerComparisonState shadow_samplers: register(s0,space2); // they use the same binding (works like overloading)
     *     [[vk::binding(0,1)]] ConstantBuffer <CB0> cbuffers: register(b0,space3); // this type also has 2 spaces, next will be on space 4 etc.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRootSignature> CreateRootSignature(const wis::PushConstant* push_constants = nullptr, uint32_t constants_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t descriptors_count = 0, uint32_t space_overlap_count = 1) const noexcept
    {
        return wis::ImplVKDevice::CreateRootSignature(push_constants, constants_count, push_descriptors, descriptors_count, space_overlap_count);
    }
    /**
     * @brief Creates a root signature object for use with DescriptorStorage.
     * Supplies number of types for each descriptor type separately.
     * @param push_constants The root constants to create the root signature with.
     * @param constants_count The number of push constants. Max is 5.
     * @param push_descriptors The root descriptors to create the root signature with.
     * In shader will appear in order of submission. e.g. root_descriptors[5] is [[vk::binding(5,0)]] ... : register(b5/t5/u5)
     * @param push_descriptors_count The number of push descriptors. Max is 8.
     * @param descriptor_spacing Descriptor spacing allocation.
     * nullptr means allocate 1 space for each.
     * @return wis::VKRootSignature on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRootSignature> CreateRootSignature2(const wis::PushConstant* push_constants = nullptr, uint32_t constants_count = 0, const wis::PushDescriptor* push_descriptors = nullptr, uint32_t push_descriptors_count = 0, const wis::DescriptorSpacing* descriptor_spacing = nullptr) const noexcept
    {
        return wis::ImplVKDevice::CreateRootSignature2(push_constants, constants_count, push_descriptors, push_descriptors_count, descriptor_spacing);
    }
    /**
     * @brief Creates a shader object.
     * @param data Shader bytecode.
     * @param size The size of the shader data in bytes. For SPIR-V must be multiple of 4.
     * @return wis::VKShader on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShader> CreateShader(void* data, uint32_t size) const noexcept
    {
        return wis::ImplVKDevice::CreateShader(data, size);
    }
    /**
     * @brief Creates a resource allocator object.
     * @return wis::VKResourceAllocator on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKResourceAllocator> CreateAllocator() const noexcept
    {
        return wis::ImplVKDevice::CreateAllocator();
    }
    /**
     * @brief Creates a render target object.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateRenderTarget(wis::VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateRenderTarget(std::move(texture), desc);
    }
    /**
     * @brief Creates a depth stencil target object.
     * Works only with depth formats.
     * Used with render passes.
     * @param texture The texture view to create the render target with.
     * @param desc The description of the render target to create.
     * Does not work with 3D textures.
     * @return wis::VKRenderTarget on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKRenderTarget> CreateDepthStencilTarget(wis::VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateDepthStencilTarget(std::move(texture), desc);
    }
    /**
     * @brief Creates a sampler object.
     * @param desc The description of the sampler to create.
     * @return wis::VKSampler on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKSampler> CreateSampler(const wis::SamplerDesc* desc) const noexcept
    {
        return wis::ImplVKDevice::CreateSampler(desc);
    }
    /**
     * @brief Creates a shader resource object.
     * @param texture The texture view to create the shader resource with.
     * @param desc The description of the shader resource to create.
     * @return wis::VKShaderResource on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKShaderResource> CreateShaderResource(wis::VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept
    {
        return wis::ImplVKDevice::CreateShaderResource(std::move(texture), desc);
    }
    /**
     * @brief Queries if the device supports the feature.
     * @param feature The feature to query.
     * @return true if feature is supported. false otherwise.
     * */
    inline bool QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
    {
        return wis::ImplVKDevice::QueryFeatureSupport(feature);
    }
};
#pragma endregion VKDevice

[[nodiscard]] WIS_INLINE wis::ResultValue<wis::VKDevice>
ImplVKCreateDevice(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept;

} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_device.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_DEVICE_H
