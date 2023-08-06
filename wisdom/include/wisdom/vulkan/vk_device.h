#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_render_pass.h>
#include <wisdom/api/api_input_layout.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_swapchain.h>
#include <wisdom/vulkan/vk_pipeline_state.h>
#include <wisdom/vulkan/vk_render_pass.h>
#include <wisdom/vulkan/vk_state_builder.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_descriptor_heap.h>
#include <wisdom/vulkan/vk_sampler.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/global/definitions.h>
#include <bitset>
#include <wisdom/util/small_allocator.h>
#endif

WIS_EXPORT namespace wis
{
    class VKDevice;

    template<>
    struct Internal<VKDevice>
    {
        shared_handle<vk::Device> device;
        shared_handle<vk::Instance> instance;
        vk::PhysicalDevice adapter;
    };

    class VKDevice : public QueryInternal<VKDevice>
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
            graphics,
            compute,
            copy,
            video_decode
        };
        struct QueueResidency {
            static constexpr size_t QueueIndex(QueueType type)
            {
                switch (type) {
                case QueueType::compute:
                    return +QueueTypes::compute;
                case QueueType::copy:
                    return +QueueTypes::copy;
                case QueueType::video_decode:
                    return +QueueTypes::video_decode;
                default:
                    return +QueueTypes::graphics;
                }
            }
            static constexpr size_t QueueFlag(QueueTypes type)
            {
                using enum vk::QueueFlagBits;
                switch (type) {
                case QueueTypes::copy:
                    return +eTransfer;
                case QueueTypes::compute:
                    return +eCompute;
                case QueueTypes::graphics:
                    return +eGraphics;
                case QueueTypes::video_decode:
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
        explicit VKDevice(const VKFactory& factory, VKAdapterView adapter)
        {
            Initialize(factory, adapter);
        }
        WIS_INLINE bool Initialize(const VKFactory& factory, VKAdapterView adapter);

        [[nodiscard]] WIS_INLINE
                VKSwapChain
                CreateSwapchain(
                        VKCommandQueueView render_queue,
                        SwapchainOptions options,
                        SurfaceParameters xsurface,
                        bool vsync = false) const;

        [[nodiscard]] VKCommandQueue CreateCommandQueue(QueueOptions options = QueueOptions{})
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

        [[nodiscard]] VKCommandList CreateCommandList(QueueType list_type) const
        {
            vk::CommandPoolCreateInfo cmd_pool_create_info{
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                queues.GetOfType(list_type)->family_index
            };
            auto ca = device->createCommandPool(cmd_pool_create_info); // allocator

            vk::CommandBufferAllocateInfo cmd_buf_alloc_info{
                ca, vk::CommandBufferLevel::ePrimary, 1
            };

            return VKCommandList{ shared_handle<vk::CommandPool>{ ca, device },
                                  vk::CommandBuffer{ device->allocateCommandBuffers(cmd_buf_alloc_info).at(0) } };
        }

        [[nodiscard]] VKFence CreateFence() const
        {
            vk::SemaphoreTypeCreateInfo timeline_desc{
                vk::SemaphoreType::eTimeline,
                0
            };
            vk::SemaphoreCreateInfo desc{
                {}, &timeline_desc
            };
            return VKFence{ shared_handle<vk::Semaphore>{ device->createSemaphore(desc), device } };
        }

        [[nodiscard]] WIS_INLINE
                VKRenderPass
                CreateRenderPass(Size2D frame_size, std::span<const ColorAttachment> rtv_descs,
                                 DepthStencilAttachment dsv_desc = DepthStencilAttachment{},
                                 SampleCount samples = SampleCount::s1,
                                 DataFormat vrs_format = DataFormat::unknown) const;

        [[nodiscard]] VKRootSignature CreateRootSignature(std::span<VKDescriptorSetLayout> layouts = {}) const
        {
            internals::uniform_allocator<vk::DescriptorSetLayout> allocator;

            for (auto& i : layouts) {
                allocator.allocate(i.GetInternal().GetDescriptorSetLayout());
            }
            vk::PipelineLayoutCreateInfo pipeline_layout_info{
                vk::PipelineLayoutCreateFlags{}, uint32_t(allocator.size()), allocator.data(), 0, nullptr
            };
            return VKRootSignature{ shared_handle<vk::PipelineLayout>{ device->createPipelineLayout(pipeline_layout_info), device } };
        }

        [[nodiscard]] WIS_INLINE
                VKPipelineState
                CreateGraphicsPipeline(const VKGraphicsPipelineDesc& desc, std::span<const InputLayoutDesc> input_layout) const;

        [[nodiscard]] VKShader CreateShader(shared_blob blob, ShaderType type) const
        {
            vk::ShaderModuleCreateInfo desc{
                vk::ShaderModuleCreateFlags{},
                blob.size(),
                blob.data<uint32_t>()
            };
            return VKShader{ shared_handle<vk::ShaderModule>{ device->createShaderModule(desc), device }, type };
        }

        /// @brief Create a Render target view object
        /// @param texture The texture to create the view for
        /// @param range Select the subresource range to create the view for
        /// @return View object
        [[nodiscard]] WIS_INLINE VKRenderTargetView CreateRenderTargetView(VKTextureView texture, RenderSelector range = {}) const;

        [[nodiscard]] VKDepthStencilView CreateDepthStencilView(VKTextureView texture) const
        {
            vk::ImageViewCreateInfo desc{
                vk::ImageViewCreateFlags{},
                texture.image,
                vk::ImageViewType::e2D,
                texture.format,
                {},
                vk::ImageSubresourceRange{
                        aspect_flags(texture.format),
                        0u,
                        1u,
                        0u,
                        1u,
                }
            };
            return VKDepthStencilView{ shared_handle<vk::ImageView>{ device->createImageView(desc), device } };
        }

        /// @brief Create a Descriptor heap object, where descriptors can be allocated from
        /// @param num_descs maximum number of descriptors that can be allocated from this heap
        /// @param type Type of descriptors that can be allocated from this heap
        /// @return Descriptor heap object
        [[nodiscard]] VKDescriptorHeap CreateDescriptorHeap(uint32_t num_descs, PoolType type) const
        {
            if (type == PoolType::CBV_SRV_UAV) {
                vk::DescriptorPoolSize size_desc{
                    vk::DescriptorType::eMutableVALVE, num_descs
                };
                vk::DescriptorPoolCreateInfo pool_desc{
                    vk::DescriptorPoolCreateFlags{}, num_descs, 1u, &size_desc
                };
                shared_handle<vk::DescriptorPool> pool{ device->createDescriptorPool(pool_desc), device };
                return VKDescriptorHeap{ std::move(pool) };
            }

            vk::DescriptorPoolSize size_desc{
                vk::DescriptorType::eSampler, num_descs
            };
            vk::DescriptorPoolCreateInfo pool_desc{
                vk::DescriptorPoolCreateFlags{}, num_descs, 1u, &size_desc
            };
            shared_handle<vk::DescriptorPool> pool{ device->createDescriptorPool(pool_desc), device };
            return VKDescriptorHeap{ std::move(pool) };
        }

        [[nodiscard]] VKDescriptorSetLayout CreateDescriptorSetLayout(std::span<BindingDescriptor> descs) const
        {
            static constexpr auto max_layout_bindings = 32;
            internals::uniform_allocator<vk::DescriptorSetLayoutBinding, max_layout_bindings> bindings;

            constexpr static vk::DescriptorType cbvSrvUavTypes[] = {
                vk::DescriptorType::eSampledImage,
                vk::DescriptorType::eStorageImage,
                vk::DescriptorType::eUniformTexelBuffer,
                vk::DescriptorType::eStorageTexelBuffer,
                vk::DescriptorType::eUniformBuffer,
                vk::DescriptorType::eStorageBuffer,
                // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR /* Need to check support if this is desired. */
            };

            constexpr static vk::MutableDescriptorTypeListVALVE a{
                sizeof(cbvSrvUavTypes) / sizeof(VkDescriptorType),
                cbvSrvUavTypes
            };

            // clang-format off
            constexpr static std::array<vk::MutableDescriptorTypeListVALVE, 64> cbvSrvUavTypeList{
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
                a,a,a,a,a,a,a,a,
            };
            // clang-format on
            bool sampler = false;
            vk::MutableDescriptorTypeCreateInfoEXT mutableTypeInfo{
                uint32_t(descs.size()), cbvSrvUavTypeList.data()
            };

            for (auto& desc : descs) {
                if (desc.type == BindingType::SAMPLER) {
                    sampler = true;
                    bindings.allocate(vk::DescriptorSetLayoutBinding{
                            desc.binding, vk::DescriptorType::eSampler, desc.count, vk::ShaderStageFlagBits(desc.stages) });
                    continue;
                }

                bindings.allocate(vk::DescriptorSetLayoutBinding{
                        desc.binding, vk::DescriptorType::eMutableVALVE, desc.count, vk::ShaderStageFlagBits(desc.stages) });
            }
            vk::DescriptorSetLayoutCreateInfo desc{
                vk::DescriptorSetLayoutCreateFlags{}, uint32_t(bindings.size()), bindings.data(), !sampler ? &mutableTypeInfo : nullptr
            };
            return VKDescriptorSetLayout{ shared_handle<vk::DescriptorSetLayout>{ device->createDescriptorSetLayout(desc), device } };
        }

        void CreateConstantBufferView(VKBufferView buffer, uint32_t size, VKDescriptorSetView set, VKDescriptorSetLayoutView, uint32_t binding = 0) const
        {
            vk::DescriptorBufferInfo desc{
                buffer, 0, size
            };
            vk::WriteDescriptorSet write{
                set, binding, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &desc, nullptr
            };
            device->updateDescriptorSets(write, nullptr);
        }

        VKShaderResourceView CreateShaderResourceView(VKTextureView texture, SubresourceRange range) const
        {
            vk::ImageViewCreateInfo desc{
                vk::ImageViewCreateFlags{},
                texture.image,
                vk::ImageViewType::e2D,
                texture.format,
                {},
                vk::ImageSubresourceRange{
                        aspect_flags(texture.format),
                        range.base_mip,
                        range.extent_mips,
                        range.base_layer,
                        range.extent_layers,
                }
            };
            return VKShaderResourceView{ shared_handle<vk::ImageView>{ device->createImageView(desc), device } };
        }

        void WriteShaderResourceView(VKShaderResourceView texture, VKDescriptorSetView set, VKDescriptorSetLayoutView, uint32_t binding = 0) const
        {
            vk::DescriptorImageInfo desc{
                {}, texture.GetInternal().GetImageView(), vk::ImageLayout::eShaderReadOnlyOptimal
            };
            vk::WriteDescriptorSet write{
                set, binding, 0, 1, vk::DescriptorType::eSampledImage, &desc, nullptr, nullptr
            };
            device->updateDescriptorSets(write, nullptr);
        }
        
        void WriteSampler(VKSampler sampler, VKDescriptorSetView set, VKDescriptorSetLayoutView, uint32_t binding = 0) const
        {
            vk::DescriptorImageInfo desc{
                sampler.GetInternal().GetSampler()
            };
            vk::WriteDescriptorSet write{
                set, binding, 0, 1, vk::DescriptorType::eSampler, &desc, nullptr, nullptr
            };
            device->updateDescriptorSets(write, nullptr);
        }

        VKSampler CreateSampler()
        {
            vk::SamplerCreateInfo desc{
                vk::SamplerCreateFlags{},
                vk::Filter::eLinear,
                vk::Filter::eLinear,
                vk::SamplerMipmapMode::eLinear,
                vk::SamplerAddressMode::eRepeat,
                vk::SamplerAddressMode::eRepeat,
                vk::SamplerAddressMode::eRepeat,
                0.0f,
                false,
                1.0f,
                false,
                vk::CompareOp::eNever,
                0.0f,
                0.0f,
                vk::BorderColor::eFloatOpaqueWhite,
                false
            };
            return VKSampler{ shared_handle<vk::Sampler>{
                    device->createSampler(desc), device } };
        }

    private:
        WIS_INLINE void GetQueueFamilies(VKAdapterView adapter) noexcept;

        [[nodiscard]] WIS_INLINE
                internals::uniform_allocator<const char*, required_extensions.size()>
                RequestExtensions(VKAdapterView adapter) noexcept;

        [[nodiscard]] WIS_INLINE
                vk::PresentModeKHR
                GetPresentMode(vk::SurfaceKHR surface, bool vsync) const noexcept;

        WIS_INLINE void FillShaderStages(
                const VKGraphicsPipelineDesc& desc,
                internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages>& shader_stages) const noexcept;

        QueueResidency queues{};
        bool vrs_supported : 1 = false;
        bool mesh_shader_supported : 1 = false;
        bool ray_tracing_supported : 1 = false;
        bool ray_query_supported : 1 = false;
        bool draw_indirect_supported : 1 = false;
    };



    bool VKDevice::Initialize(const VKFactory& factory, VKAdapterView adapter)
    {
        this->adapter = adapter;
        this->instance = factory.GetInternal().factory;

        GetQueueFamilies(adapter);
        std::array<vk::DeviceQueueCreateInfo, max_count> queue_infos{};
        size_t queue_count = 0;

        constexpr static auto priorities = []() {
            std::array<float, 64> priorities{};
            priorities.fill(1.0f);
            return priorities;
        }();

        for (size_t queue_info_size = 0; queue_info_size < max_count; queue_info_size++) {
            auto& q_info = queue_infos[queue_info_size];
            auto& q = queues.available_queues[queue_info_size];
            if (q.count == 0u)
                continue;

            q_info.queueFamilyIndex = q.family_index;
            q_info.queueCount = q.count; // hard wired for now
            q_info.pQueuePriorities = priorities.data();
            queue_count++;
        }

        auto exts = RequestExtensions(adapter);

        void* device_create_info_next = nullptr;
        auto add_extension = [&](auto& extension) {
            extension.pNext = device_create_info_next;
            device_create_info_next = &extension;
        };

        auto physical_device_features = adapter.getFeatures();
        vk::PhysicalDeviceFeatures2 device_features;
        {
            device_features.features.robustBufferAccess = physical_device_features.robustBufferAccess;
            device_features.features.fullDrawIndexUint32 = physical_device_features.fullDrawIndexUint32;
            device_features.features.imageCubeArray = physical_device_features.imageCubeArray;
            device_features.features.independentBlend = physical_device_features.independentBlend;
            device_features.features.geometryShader = physical_device_features.geometryShader;
            device_features.features.tessellationShader = physical_device_features.tessellationShader;
            device_features.features.sampleRateShading = physical_device_features.sampleRateShading;
            device_features.features.dualSrcBlend = physical_device_features.dualSrcBlend;
            device_features.features.logicOp = physical_device_features.logicOp;
            device_features.features.multiDrawIndirect = physical_device_features.multiDrawIndirect;
            device_features.features.drawIndirectFirstInstance = physical_device_features.drawIndirectFirstInstance;
            device_features.features.depthClamp = physical_device_features.depthClamp;
            device_features.features.depthBiasClamp = physical_device_features.depthBiasClamp;
            device_features.features.fillModeNonSolid = physical_device_features.fillModeNonSolid;
            device_features.features.depthBounds = physical_device_features.depthBounds;
            device_features.features.wideLines = physical_device_features.wideLines;
            device_features.features.largePoints = physical_device_features.largePoints;
            device_features.features.alphaToOne = physical_device_features.alphaToOne;
            device_features.features.multiViewport = physical_device_features.multiViewport;
            device_features.features.samplerAnisotropy = physical_device_features.samplerAnisotropy;
            device_features.features.textureCompressionETC2 = physical_device_features.textureCompressionETC2;
            device_features.features.textureCompressionASTC_LDR = physical_device_features.textureCompressionASTC_LDR;
            device_features.features.textureCompressionBC = physical_device_features.textureCompressionBC;
            device_features.features.occlusionQueryPrecise = physical_device_features.occlusionQueryPrecise;
            device_features.features.pipelineStatisticsQuery = physical_device_features.pipelineStatisticsQuery;
            device_features.features.vertexPipelineStoresAndAtomics = physical_device_features.vertexPipelineStoresAndAtomics;
            device_features.features.fragmentStoresAndAtomics = physical_device_features.fragmentStoresAndAtomics;
            device_features.features.shaderTessellationAndGeometryPointSize = physical_device_features.shaderTessellationAndGeometryPointSize;
            device_features.features.shaderImageGatherExtended = physical_device_features.shaderImageGatherExtended;
            device_features.features.shaderStorageImageExtendedFormats = physical_device_features.shaderStorageImageExtendedFormats;
            device_features.features.shaderStorageImageMultisample = physical_device_features.shaderStorageImageMultisample;
            device_features.features.shaderStorageImageReadWithoutFormat = physical_device_features.shaderStorageImageReadWithoutFormat;
            device_features.features.shaderStorageImageWriteWithoutFormat = physical_device_features.shaderStorageImageWriteWithoutFormat;
            device_features.features.shaderUniformBufferArrayDynamicIndexing = physical_device_features.shaderUniformBufferArrayDynamicIndexing;
            device_features.features.shaderSampledImageArrayDynamicIndexing = physical_device_features.shaderSampledImageArrayDynamicIndexing;
            device_features.features.shaderStorageBufferArrayDynamicIndexing = physical_device_features.shaderStorageBufferArrayDynamicIndexing;
            device_features.features.shaderStorageImageArrayDynamicIndexing = physical_device_features.shaderStorageImageArrayDynamicIndexing;
            device_features.features.shaderClipDistance = physical_device_features.shaderClipDistance;
            device_features.features.shaderCullDistance = physical_device_features.shaderCullDistance;
            device_features.features.shaderFloat64 = physical_device_features.shaderFloat64;
            device_features.features.shaderInt64 = physical_device_features.shaderInt64;
            device_features.features.shaderInt16 = physical_device_features.shaderInt16;
            device_features.features.shaderResourceResidency = physical_device_features.shaderResourceResidency;
            device_features.features.shaderResourceMinLod = physical_device_features.shaderResourceMinLod;
            device_features.features.sparseBinding = physical_device_features.sparseBinding;
            device_features.features.sparseResidencyBuffer = physical_device_features.sparseResidencyBuffer;
            device_features.features.sparseResidencyImage2D = physical_device_features.sparseResidencyImage2D;
            device_features.features.sparseResidencyImage3D = physical_device_features.sparseResidencyImage3D;
            device_features.features.sparseResidency2Samples = physical_device_features.sparseResidency2Samples;
            device_features.features.sparseResidency4Samples = physical_device_features.sparseResidency4Samples;
            device_features.features.sparseResidency8Samples = physical_device_features.sparseResidency8Samples;
            device_features.features.sparseResidency16Samples = physical_device_features.sparseResidency16Samples;
            device_features.features.sparseResidencyAliased = physical_device_features.sparseResidencyAliased;
            device_features.features.variableMultisampleRate = physical_device_features.variableMultisampleRate;
            device_features.features.inheritedQueries = physical_device_features.inheritedQueries;
        }
        add_extension(device_features);

        vk::PhysicalDeviceSynchronization2Features sync2;
        sync2.synchronization2 = true;
        add_extension(sync2);

        vk::PhysicalDeviceVulkan12Features device_vulkan12_features;
        device_vulkan12_features.drawIndirectCount = draw_indirect_supported;
        device_vulkan12_features.bufferDeviceAddress = true;
        device_vulkan12_features.timelineSemaphore = true;
        device_vulkan12_features.runtimeDescriptorArray = true;
        device_vulkan12_features.descriptorBindingVariableDescriptorCount = true;
        device_vulkan12_features.imagelessFramebuffer = true;
        add_extension(device_vulkan12_features);

        vk::PhysicalDeviceMutableDescriptorTypeFeaturesVALVE mutable_descriptor_type_features;
        mutable_descriptor_type_features.mutableDescriptorType = true;
        add_extension(mutable_descriptor_type_features);

        vk::DeviceCreateInfo desc{
            {}, uint32_t(queue_count), queue_infos.data(), 0, nullptr, uint32_t(exts.size()), exts.data(), nullptr, device_create_info_next
        };

        device = shared_handle<vk::Device>{ adapter.createDevice(desc) };
        return bool(device);
    }

    VKSwapChain VKDevice::CreateSwapchain(VKCommandQueueView render_queue, SwapchainOptions options, SurfaceParameters xsurface, bool vsync) const
    {
        using Type = SurfaceParameters::Type;
        if (xsurface.type == Type::WinRT)
            return {}; // Bail out, no support for UWP from Vulkan

#if defined(WISDOM_WINDOWS)
        vk::Win32SurfaceCreateInfoKHR surface_desc{
            {}, GetModuleHandle(nullptr), xsurface.hwnd
        };
        lib_info("Initializing Win32 Surface");
        shared_handle<vk::SurfaceKHR> surface{ instance->createWin32SurfaceKHR(surface_desc), instance };
#elif defined(WISDOM_MACOS)
        static_assert(false, "No Mac is implemented");
#elif defined(WISDOM_LINUX)
        shared_handle<vk::SurfaceKHR> surface;
        if (xsurface.type == Type::X11) {
            vk::XcbSurfaceCreateInfoKHR surface_desc = {};
            surface_desc.setConnection(xsurface.x11.connection);
            surface_desc.setWindow((ptrdiff_t)xsurface.x11.window);
            lib_info("Initializing XCB Surface");
            surface = shared_handle<vk::SurfaceKHR>{ instance->createXcbSurfaceKHR(surface_desc), instance };
        } else {
            assert(xsurface.type == Type::Wayland);
            vk::WaylandSurfaceCreateInfoKHR surface_desc = {};
            surface_desc.setDisplay(xsurface.wayland.display);
            surface_desc.setSurface(xsurface.wayland.surface);
            lib_info("Initializing Wayland Surface");
            surface = shared_handle<vk::SurfaceKHR>{ instance->createWaylandSurfaceKHR(surface_desc), instance };
        }
#endif
        int32_t present_queue = -1;
        for (uint16_t i = 0; i < max_count; i++) {
            const auto& x = queues.available_queues[i];
            if (x.Empty())
                continue;

            if (adapter.getSurfaceSupportKHR(x.family_index, surface.get())) {
                present_queue = i;
                lib_info(format("Present queue {} selected", i));
                break;
            }
        }
        if (present_queue == -1) {
            lib_error("None of the queues support presenting to the surface");
            return {}; // Presentation is not supported
        }

        const auto& queue = queues.available_queues[present_queue];
        vk::DeviceQueueInfo2 info{
            {},
            queue.family_index,
            queue.GetNextInLine()
        };
        vk::Queue qpresent_queue = device->getQueue2(info);

        auto surface_formats = adapter.getSurfaceFormatsKHR(surface.get());
        auto format = std::ranges::find_if(surface_formats,
                                           [=](const vk::SurfaceFormatKHR& fmt) {
                                               return fmt.format == vk_format(options.format);
                                           });
        if (format == surface_formats.end() || format->format == vk::Format::eUndefined) {
            lib_error(wis::format("Supplied format {} is not supported by surface", data_format_strings[+options.format]));
            return {}; // Format specified is not supported
        }

        auto cap = adapter.getSurfaceCapabilitiesKHR(surface.get());
        bool stereo = cap.maxImageArrayLayers > 1;
        if (options.stereo && stereo)
            lib_info(wis::format("Stereo mode is ativated"));

        uint32_t layers = options.stereo && stereo ? 2u : 1u;

        auto present_mode = GetPresentMode(surface.get(), vsync);

        vk::SwapchainCreateInfoKHR desc{
            vk::SwapchainCreateFlagBitsKHR{}, surface.get(),
            options.frame_count, format->format, format->colorSpace,
            vk::Extent2D{ options.width, options.height },
            layers,
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst,
            vk::SharingMode::eExclusive, 0u, nullptr,
            vk::SurfaceTransformFlagBitsKHR::eIdentity,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            present_mode, true, nullptr
        };

        return VKSwapChain{
            shared_handle<vk::SwapchainKHR>{
                    device->createSwapchainKHR(desc),
                    device, surface },
            render_queue,
            VKCommandQueue{ qpresent_queue },
            CreateCommandList(QueueType::direct),
            *format,
            present_mode,
            stereo
        };
    }

    // NOLINTNEXTLINE
    VKRenderPass VKDevice::CreateRenderPass(Size2D frame_size, std::span<const ColorAttachment> rtv_descs,
                                                      DepthStencilAttachment dsv_desc,
                                                      SampleCount samples,
                                                      DataFormat /*vrs_format*/) const
    {
        std::array<vk::AttachmentDescription2, max_render_targets + 2> attachment_descriptions{};
        std::array<vk::AttachmentReference2, max_render_targets + 2> attachment_references{};
        internals::uniform_allocator<vk::FramebufferAttachmentImageInfo, max_render_targets + 2> image_md;

        size_t size = 0;
        static constexpr size_t max_attachment = 8;

        for (auto& i : rtv_descs) {
            auto& desc = attachment_descriptions[size];
            auto& ref = attachment_references[size++];
            if (i.format == DataFormat::unknown) {
                ref.attachment = VK_ATTACHMENT_UNUSED;
                continue;
            }

            desc.format = vk_format(i.format);
            desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
            desc.loadOp = convert(i.load);
            desc.storeOp = convert(i.store);
            desc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
            desc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

            ref.attachment = size - 1;
            ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

            image_md.allocate(vk::ImageCreateFlags{}, vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits(i.usage_flags),
                              frame_size.width, frame_size.height, i.array_levels, 1u, &desc.format);

            if (size == max_attachment)
                break;
        }

        vk::SubpassDescription2 sub_pass;
        sub_pass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        sub_pass.colorAttachmentCount = size;
        sub_pass.pColorAttachments = attachment_references.data();

        vk::AttachmentReference2 depth_reference;
        if (dsv_desc.format != DataFormat::unknown) {
            auto& desc = attachment_descriptions[size];
            auto& ref = attachment_references[size++];

            desc.format = vk_format(dsv_desc.format);
            desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
            desc.loadOp = convert(dsv_desc.depth_load);
            desc.storeOp = convert(dsv_desc.depth_store);
            desc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
            desc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
            desc.stencilLoadOp = convert(dsv_desc.stencil_load);
            desc.stencilStoreOp = convert(dsv_desc.stencil_store);

            ref.attachment = size - 1;
            ref.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

            image_md.allocate(
                    vk::ImageCreateFlags{}, vk::ImageUsageFlagBits::eDepthStencilAttachment,
                    frame_size.width, frame_size.height, 1u, 1u, &desc.format);

            sub_pass.pDepthStencilAttachment = &attachment_references[size - 1];
        }

        vk::AttachmentReference2 shading_rate_image_attachment_reference;
        vk::FragmentShadingRateAttachmentInfoKHR fragment_shading_rate_attachment_info;
        fragment_shading_rate_attachment_info.pFragmentShadingRateAttachment = &shading_rate_image_attachment_reference;

        // if (vrs_format != DataFormat::unknown && vrs_supported)
        // {
        // 	auto& desc = attachment_descriptions[size];
        // 	auto& ref = attachment_references[size++];

        // 	desc.format = vk_format(vrs_format);
        // 	desc.samples = static_cast<vk::SampleCountFlagBits>(samples);
        // 	desc.loadOp = vk::AttachmentLoadOp::eLoad;
        // 	desc.storeOp = vk::AttachmentStoreOp::eStore;
        // 	desc.initialLayout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
        // 	desc.finalLayout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;

        // 	ref.attachment = size - 1;
        // 	ref.layout = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;

        // 	sub_pass.pDepthStencilAttachment = &attachment_references[size - 1];

        // 	vk::PhysicalDeviceFragmentShadingRatePropertiesKHR shading_rate_image_properties;
        // 	vk::PhysicalDeviceProperties2 device_props2;
        // 	device_props2.pNext = &shading_rate_image_properties;
        // 	adapter.getProperties2(&device_props2);
        // 	auto vrs_size = shading_rate_image_properties.maxFragmentShadingRateAttachmentTexelSize.width;

        // 	fragment_shading_rate_attachment_info.shadingRateAttachmentTexelSize.width = vrs_size;
        // 	fragment_shading_rate_attachment_info.shadingRateAttachmentTexelSize.height = vrs_size;
        // 	sub_pass.pNext = &fragment_shading_rate_attachment_info;
        // }

        vk::RenderPassCreateInfo2 render_pass_info;
        render_pass_info.attachmentCount = size;
        render_pass_info.pAttachments = attachment_descriptions.data();
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &sub_pass;
        auto rp = shared_handle<vk::RenderPass>{ device->createRenderPass2KHR(render_pass_info, nullptr, DynamicLoader::loader), device };

        vk::FramebufferAttachmentsCreateInfo attachments_create_info;
        attachments_create_info.attachmentImageInfoCount = image_md.size();
        attachments_create_info.pAttachmentImageInfos = image_md.data();

        vk::FramebufferCreateInfo desc{
            vk::FramebufferCreateFlagBits::eImageless,
            rp.get(), uint32_t(image_md.size()), nullptr, frame_size.width, frame_size.height, 1, &attachments_create_info
        };

        return VKRenderPass{
            std::move(rp),
            shared_handle<vk::Framebuffer>{ device->createFramebuffer(desc), device },
            frame_size
        };
    }

    VKPipelineState VKDevice::CreateGraphicsPipeline(const VKGraphicsPipelineDesc& desc, std::span<const InputLayoutDesc> input_layout) const
    {
        static constexpr size_t attr_descriptions_per_binding = 16;
        std::array<vk::VertexInputBindingDescription, max_vertex_bindings> bindings;
        internals::uniform_allocator<vk::VertexInputAttributeDescription, max_vertex_bindings * attr_descriptions_per_binding> attributes;

        internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages> shader_stages;
        FillShaderStages(desc, shader_stages);

        std::bitset<max_vertex_bindings> binding_map;
        for (const auto& i : input_layout) {
            auto& b = bindings.at(i.input_slot);
            if (!binding_map[i.input_slot]) {
                b.inputRate = vk::VertexInputRate(i.input_slot_class);
                b.binding = i.input_slot;
                b.stride = 0; // we don't care abot stride, since we bind dynamic vertex buffers
                binding_map.set(i.input_slot);
            }
            auto& at = attributes.allocate();
            at.binding = i.input_slot;
            at.format = convert_vk(i.format);
            at.location = i.location;
            at.offset = i.aligned_byte_offset;
        }

        // remove empty bindings and compact the array
        size_t rsize = 0;
        for (size_t i = rsize; i < max_vertex_bindings; i++)
            if (binding_map[i])
                bindings[rsize++] = bindings[i];

        vk::PipelineVertexInputStateCreateInfo ia{
            vk::PipelineVertexInputStateCreateFlagBits{},
            uint32_t(rsize),
            bindings.data(),
            uint32_t(attributes.size()),
            attributes.data()
        };

        vk::PipelineViewportStateCreateInfo viewport_state;
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        vk::PipelineRasterizationStateCreateInfo rasterizer{
            vk::PipelineRasterizationStateCreateFlags{},
            false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise,
            false, 0.0f, 0.0f, 0.0f, 1.0f
        };

        vk::PipelineColorBlendAttachmentState color_blend_attachment[2]{
            // 1 for now, TODO: proper blending
            vk::PipelineColorBlendAttachmentState{ false, // disabled
                                                   vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
                                                   vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
                                                   vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA },
            vk::PipelineColorBlendAttachmentState{ false, // disabled
                                                   vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
                                                   vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
                                                   vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA },

        };
        vk::PipelineColorBlendStateCreateInfo color_blending{
            vk::PipelineColorBlendStateCreateFlags{},
            false,
            vk::LogicOp::eCopy,
            2,
            color_blend_attachment,
            { 0.0f, 0.0f, 0.0f, 0.0f }
        };

        vk::PipelineMultisampleStateCreateInfo multisampling{};
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multisampling.sampleShadingEnable = false;

        vk::PipelineInputAssemblyStateCreateInfo input_assembly{
            vk::PipelineInputAssemblyStateCreateFlags{},
            vk::PrimitiveTopology::eTriangleList, false
        };

        static constexpr size_t max_dynstates = 5; // only four if not using vrs
        internals::uniform_allocator<vk::DynamicState, max_dynstates> dynamic_state_enables;
        dynamic_state_enables.allocate(vk::DynamicState::eViewport);
        dynamic_state_enables.allocate(vk::DynamicState::eScissor);
        dynamic_state_enables.allocate(vk::DynamicState::ePrimitiveTopology);
        dynamic_state_enables.allocate(vk::DynamicState::eVertexInputBindingStride);
        // if (vrs_supported)
        // 	dynamic_state_enables.allocate(vk::DynamicState::eFragmentShadingRateKHR);

        vk::PipelineDynamicStateCreateInfo dss{
            {}, uint32_t(dynamic_state_enables.size()), dynamic_state_enables.data()
        };

        vk::PipelineDepthStencilStateCreateInfo depth_stencil_state{
            vk::PipelineDepthStencilStateCreateFlags{},
            true, true,
            vk::CompareOp::eLess,
            false, false,
            vk::StencilOpState{}, vk::StencilOpState{},
            0.0f, 1.0f
        };

        vk::GraphicsPipelineCreateInfo pipeline_desc{
            vk::PipelineCreateFlags{},
            uint32_t(shader_stages.size()),
            shader_stages.data(), // shader stages
            &ia, // vertex input
            &input_assembly, // input assembly
            nullptr, // tessellation
            &viewport_state, // viewport
            &rasterizer, // rasterizer
            &multisampling, // multisampling
            &depth_stencil_state, // depth stencil
            &color_blending, // color blending
            &dss, // dynamic state
            desc.sig, // pipeline layout
            desc.pass.GetInternal().GetRenderPass(), // render pass
        };

        return VKPipelineState{ shared_handle<vk::Pipeline>{ device->createGraphicsPipeline(nullptr, pipeline_desc).value, device } };
    }

    VKRenderTargetView VKDevice::CreateRenderTargetView(VKTextureView texture, RenderSelector range) const
    {
        vk::ImageViewCreateInfo desc{
            vk::ImageViewCreateFlags{},
            texture.image,
            vk::ImageViewType::e2DArray,
            texture.format,
            {},
            vk::ImageSubresourceRange{
                    aspect_flags(texture.format),
                    range.mip,
                    1u,
                    range.base_layer,
                    range.extent_layers,
            }
        };
        return VKRenderTargetView{ shared_handle<vk::ImageView>{ device->createImageView(desc), device } };
    }

    void VKDevice::GetQueueFamilies(VKAdapterView adapter) noexcept
    {
        using namespace river::flags;
        auto family_props = adapter.getQueueFamilyProperties();
        lib_info(format("The system supports {} queue families", family_props.size()));
        assert(family_props.size() < 256);

        // NOLINTNEXTLINE
        for (uint8_t i = 0; i < family_props.size(); i++) {
            using enum vk::QueueFlagBits;
            auto& family = family_props[i];
            if ((family.queueFlags & eGraphics) == eGraphics && queues.available_queues[+QueueTypes::graphics].Empty()) {
                queues.available_queues[+QueueTypes::graphics] = {
                    .queue_flags = uint16_t(uint32_t(family.queueFlags)),
                    .count = uint8_t(family.queueCount),
                    .family_index = i,
                };
                lib_info(format("\tDedicated Graphics queues: {}", family.queueCount));
                continue;
            }
            if ((family.queueFlags & eCompute) == eCompute && queues.available_queues[+QueueTypes::compute].Empty()) {
                queues.available_queues[+QueueTypes::compute] = {
                    .queue_flags = uint16_t(uint32_t(family.queueFlags)),
                    .count = uint8_t(family.queueCount),
                    .family_index = i,
                };
                lib_info(format("\tDedicated Compute queues: {}", family.queueCount));
                continue;
            }
            if ((family.queueFlags & eVideoDecodeKHR) == eVideoDecodeKHR && queues.available_queues[+QueueTypes::video_decode].Empty()) {
                queues.available_queues[+QueueTypes::video_decode] = {
                    .queue_flags = uint16_t(uint32_t(family.queueFlags)),
                    .count = uint8_t(family.queueCount),
                    .family_index = i,
                };
                lib_info(format("\tDedicated Video Decode queues: {}", family.queueCount));
                continue;
            }
            if ((family.queueFlags & eTransfer) == eTransfer && queues.available_queues[+QueueTypes::copy].Empty()) {
                queues.available_queues[+QueueTypes::copy] = {
                    .queue_flags = uint16_t(uint32_t(family.queueFlags)),
                    .count = uint8_t(family.queueCount),
                    .family_index = i,
                };
                lib_info(format("\tDedicated Data Transfer queues: {}", family.queueCount));
                continue;
            }
        }
    }

    internals::uniform_allocator<const char*, VKDevice::required_extensions.size()>
    VKDevice::RequestExtensions(VKAdapterView adapter) noexcept
    {
        auto extensions = adapter.enumerateDeviceExtensionProperties();
        std::unordered_set<std::string_view, string_hash> ext_set;
        ext_set.reserve(extensions.size());

        for (const auto& e : extensions)
            ext_set.emplace(e.extensionName.data());

        internals::uniform_allocator<const char*, required_extensions.size()> avail_exts{};

        for (const auto* i : required_extensions) {
            if (!ext_set.contains(i))
                continue;
            avail_exts.allocate(i);

            if (i == std::string_view(VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME))
                vrs_supported = true;
            else if (i == std::string_view(VK_NV_MESH_SHADER_EXTENSION_NAME))
                mesh_shader_supported = true;
            else if (i == std::string_view(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME))
                ray_tracing_supported = true;
            else if (i == std::string_view(VK_KHR_RAY_QUERY_EXTENSION_NAME))
                ray_query_supported = true;
            else if (i == std::string_view(VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME))
                draw_indirect_supported = true;
        }

        if constexpr (debug_mode) {
            lib_info("Active Device Extensions:");
            for (auto& i : avail_exts)
                lib_info(format("\t{}", i));
        }

        return avail_exts;
    }

    vk::PresentModeKHR VKDevice::GetPresentMode(vk::SurfaceKHR surface, bool vsync) const noexcept
    {
        using enum vk::PresentModeKHR;
        auto modes = adapter.getSurfacePresentModesKHR(surface);
        return vsync ? std::ranges::count(modes, eFifoRelaxed) ? eFifoRelaxed : eFifo
                     : eImmediate;
    }

    // NOLINTNEXTLINE
    void VKDevice::FillShaderStages(const VKGraphicsPipelineDesc& desc, internals::uniform_allocator<vk::PipelineShaderStageCreateInfo, max_shader_stages>& shader_stages) const noexcept
    {
        if (desc.vs) {
            auto& vs = shader_stages.allocate();
            vs.stage = vk::ShaderStageFlagBits::eVertex;
            vs.module = desc.vs.GetInternal().GetShaderModule();
            vs.pName = "main";
        }
        if (desc.ps) {
            auto& vs = shader_stages.allocate();
            vs.stage = vk::ShaderStageFlagBits::eFragment;
            vs.module = desc.ps.GetInternal().GetShaderModule();
            vs.pName = "main";
        }
        if (desc.gs) {
            auto& vs = shader_stages.allocate();
            vs.stage = vk::ShaderStageFlagBits::eGeometry;
            vs.module = desc.gs.GetInternal().GetShaderModule();
            vs.pName = "main";
        }
        if (desc.hs) {
            auto& vs = shader_stages.allocate();
            vs.stage = vk::ShaderStageFlagBits::eTessellationControl;
            vs.module = desc.hs.GetInternal().GetShaderModule();
            vs.pName = "main";
        }
        if (desc.ds) {
            auto& vs = shader_stages.allocate();
            vs.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
            vs.module = desc.ds.GetInternal().GetShaderModule();
            vs.pName = "main";
        }
    }


}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_device.inl"
#endif
