#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/vk_device.h>
#endif // !WISDOM_HEADER_ONLY

#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/vk_factory.h>

constexpr static inline std::array required_extensions{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, // for Swapchain
    VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, // for Fence
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME, // for barriers

    VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, // for Allocator
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, // for Allocator
    VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, // for Allocator
    VK_KHR_MAINTENANCE_4_EXTENSION_NAME, // for Allocator

    VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME, // for PushDescriptor
    VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME, // for Tessellation control point count
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, // for dynamic render pass

    VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME, // for Mutable Descriptor Type
    VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,

    VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME, // for Border Color

    // VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    // VK_KHR_RAY_QUERY_EXTENSION_NAME,
    // VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
};

inline auto RequestExtensions(VkPhysicalDevice adapter, const wis::VkInstanceTable& itable) noexcept
{
    std::vector<VkExtensionProperties> ext_props;
    uint32_t count = 0;
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, nullptr);
    ext_props.resize(count);
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, ext_props.data());

    std::unordered_set<std::string_view, wis::string_hash> ext_set;
    ext_set.reserve(count);

    for (const auto& e : ext_props)
        ext_set.emplace(e.extensionName);

    wis::detail::uniform_allocator<const char*, required_extensions.size()> avail_exts{};

    for (const auto* i : required_extensions) {
        if (!ext_set.contains(i))
            continue;
        avail_exts.allocate(i);
    }

    if constexpr (wis::debug_mode) {
        wis::lib_info("Active Device Extensions:");
        for (auto& i : avail_exts)
            wis::lib_info(wis::format("\t{}", i));
    }

    return avail_exts;
}

inline wis::detail::QueueResidency GetQueueFamilies(VkPhysicalDevice adapter, const wis::VkInstanceTable& itable) noexcept
{
    using namespace wis::detail;
    using wis::operator+;

    std::vector<VkQueueFamilyProperties> family_props;
    VkResult result = VK_SUCCESS;
    uint32_t count = 0;
    itable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, nullptr);
    family_props.resize(count);
    itable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, family_props.data());

    wis::lib_info(wis::format("The system supports {} queue families", family_props.size()));
    assert(family_props.size() < 256);

    QueueResidency queues;

    // NOLINTNEXTLINE
    for (uint8_t i = 0; i < family_props.size(); i++) {
        using enum VkQueueFlagBits;
        auto& family = family_props[i];
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) ==
                    VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT &&
            queues.available_queues[+QueueTypes::Graphics].Empty()) {
            queues.available_queues[+QueueTypes::Graphics] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Graphics queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) ==
                    VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT &&
            queues.available_queues[+QueueTypes::Compute].Empty()) {
            queues.available_queues[+QueueTypes::Compute] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Compute queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR) ==
                    VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR &&
            queues.available_queues[+QueueTypes::VideoDecode].Empty()) {
            queues.available_queues[+QueueTypes::VideoDecode] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Video Decode queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) ==
                    VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT &&
            queues.available_queues[+QueueTypes::Copy].Empty()) {
            queues.available_queues[+QueueTypes::Copy] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Data Transfer queues: {}", family.queueCount));
            continue;
        }
    }
    return queues;
}

wis::ResultValue<wis::VKDevice> wis::VKCreateDevice(wis::VKAdapter adapter) noexcept
{
    constexpr static auto max_queue_count = +wis::detail::QueueTypes::Count;
    wis::detail::uniform_allocator<VkDeviceQueueCreateInfo, max_queue_count> queue_infos{};

    auto& adapter_i = adapter.GetInternal();
    auto hadapter = adapter_i.adapter;
    auto& itable = adapter_i.instance.table();
    auto& gtable = wis::detail::VKFactoryGlobals::Instance().global_table;

    auto queues = GetQueueFamilies(hadapter, itable);

    void** next_chain = nullptr;

    auto set_next = [&next_chain](void* next) {
        struct R {
            VkStructureType a;
            void* pnext;
        };
        auto x = reinterpret_cast<R*>(next);
        *next_chain = next;
        next_chain = &x->pnext;
    };

    constexpr static auto priorities = []() {
        std::array<float, 64> priorities{};
        priorities.fill(1.0f);
        return priorities;
    }();

    for (size_t queue_info_size = 0; queue_info_size < max_queue_count; queue_info_size++) {
        auto& q = queues.available_queues[queue_info_size];
        if (q.count == 0u)
            continue;
        queue_infos.allocate() = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            q.family_index,
            q.count,
            priorities.data(),
        };
    }

    auto present_exts = RequestExtensions(hadapter, itable);
    if (!present_exts.contains(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME))
        return wis::make_result<FUNC, "The system does not support timeline semaphores.">(VkResult::VK_ERROR_UNKNOWN);

    if (!present_exts.contains(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME))
        return wis::make_result<FUNC, "The system does not support synchronization primitives.">(VkResult::VK_ERROR_UNKNOWN);

    // Loading features
    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = nullptr,
        .features = {},
    };
    next_chain = &features.pNext;

    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timeline_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
        .pNext = nullptr,
        .timelineSemaphore = true,
    };
    set_next(&timeline_features);

    VkPhysicalDeviceSynchronization2Features sync_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
        .pNext = nullptr,
        .synchronization2 = true,
    };
    set_next(&sync_features);

    VkPhysicalDeviceExtendedDynamicState2FeaturesEXT extdyn_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT,
        .pNext = nullptr,
        .extendedDynamicState2 = true,
    };
    if (present_exts.contains(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME)) // TODO: Check if there is such extension
        set_next(&extdyn_features);

    VkPhysicalDeviceDescriptorBufferFeaturesEXT descbuffer_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT,
        .pNext = nullptr,
    };
    if (present_exts.contains(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME))
        set_next(&descbuffer_features);

    VkPhysicalDeviceDynamicRenderingFeaturesKHR dyn_render{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
        .pNext = nullptr,
        .dynamicRendering = true,
    };
    if (present_exts.contains(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME))
        set_next(&dyn_render);

    VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE mutable_desc_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE,
        .pNext = nullptr,
        .mutableDescriptorType = true,
    };
    if (present_exts.contains(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME) ||
        present_exts.contains(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME))
        set_next(&mutable_desc_features);

    VkPhysicalDeviceCustomBorderColorFeaturesEXT border_color_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT,
        .pNext = nullptr,
        .customBorderColors = true,
    };
    if (present_exts.contains(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME))
        set_next(&border_color_features);

    VkPhysicalDeviceBufferDeviceAddressFeatures buffer_address_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
        .pNext = nullptr,
        .bufferDeviceAddress = true,
    };
    set_next(&buffer_address_features);

    itable.vkGetPhysicalDeviceFeatures2(hadapter, &features);

    VkDeviceCreateInfo device_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(present_exts.size()),
        .ppEnabledExtensionNames = present_exts.data(),
        .pEnabledFeatures = nullptr,
    };

    // Creating device
    InternalFeatures ifeatures{
        .has_descriptor_buffer = present_exts.contains(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME),
        .dynamic_rendering = bool(dyn_render.dynamicRendering),
    };
    DeviceFeatures dfeatures =
            DeviceFeatures(+DeviceFeatures::PushDescriptors & int(ifeatures.has_descriptor_buffer));

    VkDevice unsafe_device;
    VkResult result = VK_SUCCESS;
    if (!wis::succeeded(result = itable.vkCreateDevice(hadapter, &device_info, nullptr, &unsafe_device)))
        return wis::make_result<FUNC, "vkCreateDevice failed to create device">(result);

    wis::managed_handle<VkDevice> device{ unsafe_device, (PFN_vkDestroyDevice)gtable.vkGetDeviceProcAddr(unsafe_device, "vkDestroyDevice") };
    std::unique_ptr<VkDeviceTable> device_table = wis::detail::make_unique<VkDeviceTable>();
    if (!device_table)
        return wis::make_result<FUNC, "Failed to allocate device table">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    device_table->Init(device.get(), gtable);

    auto feature_details = wis::detail::make_unique<FeatureDetails>();
    if (!feature_details)
        return wis::make_result<FUNC, "Failed to allocate feature details">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    wis::VKDevice vkdevice{ wis::SharedDevice{ device.release(), std::move(device_table) },
                            std::move(adapter), std::move(feature_details),
                            dfeatures, ifeatures };

    return { wis::success, std::move(vkdevice) };
}

wis::VKDevice::VKDevice(wis::SharedDevice in_device,
                        wis::VKAdapter in_adapter,
                        std::unique_ptr<FeatureDetails> feature_details,
                        wis::DeviceFeatures in_features,
                        InternalFeatures in_ifeatures) noexcept
    : QueryInternal(std::move(in_adapter), std::move(in_device), in_ifeatures), features(in_features)
{
    auto& gtable = wis::detail::VKFactoryGlobals::Instance().global_table;
    auto& itable = GetInstanceTable();
    auto& dtable = device.table();

    queues = GetQueueFamilies(adapter.GetInternal().adapter, itable);

    VkPhysicalDeviceProperties2 props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = nullptr,
        .properties = {},
    };

    auto& descriptor_buffer_properties = feature_details->descriptor_buffer_properties;
    {
        descriptor_buffer_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT;
        descriptor_buffer_properties.pNext = nullptr;
    };

    if (ifeatures.has_descriptor_buffer) {
        props.pNext = &descriptor_buffer_properties;
    }
    itable.vkGetPhysicalDeviceProperties2(adapter.GetInternal().adapter, &props);

    if (ifeatures.has_descriptor_buffer) {
        feature_details->mutable_descriptor_size = std::max({
                descriptor_buffer_properties.uniformBufferDescriptorSize,
                descriptor_buffer_properties.storageBufferDescriptorSize,

                descriptor_buffer_properties.sampledImageDescriptorSize,
                descriptor_buffer_properties.storageImageDescriptorSize,

                descriptor_buffer_properties.storageTexelBufferDescriptorSize,
                descriptor_buffer_properties.uniformTexelBufferDescriptorSize,
        });
    }
    if (ifeatures.has_descriptor_buffer) {
        ifeatures.push_descriptor_bufferless = descriptor_buffer_properties.bufferlessPushDescriptors;
    }
    ifeatures.max_ia_attributes = props.properties.limits.maxVertexInputAttributes;

    this->feature_details = std::move(feature_details);
}

wis::Result wis::VKDevice::WaitForMultipleFences(const VKFenceView* fences, const uint64_t* values,
                                                 uint32_t count, MutiWaitFlags wait_all,
                                                 uint64_t timeout) const noexcept
{
    VkSemaphoreWaitInfo waitInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
                                  .pNext = nullptr,
                                  .flags = VkSemaphoreWaitFlags(wait_all),
                                  .semaphoreCount = static_cast<uint32_t>(count),
                                  .pSemaphores = reinterpret_cast<const VkSemaphore*>(fences),
                                  .pValues = values };
    VkResult result = device.table().vkWaitSemaphores(device.get(), &waitInfo, timeout);

    return succeeded(result)
            ? wis::success
            : wis::make_result<FUNC, "vkWaitSemaphores failed to wait for fences.">(result);
}

wis::ResultValue<wis::VKFence>
wis::VKDevice::CreateFence(uint64_t initial_value) const noexcept
{
    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };
    VkSemaphore sem;
    VkResult result = device.table().vkCreateSemaphore(device.get(), &desc, nullptr, &sem);

    if (!succeeded(result))
        return wis::make_result<FUNC, "vkCreateSemaphore failed to create a timeline semaphore.">(result);

    return VKFence{ { sem, device, device.table().vkDestroySemaphore } };
}

wis::ResultValue<wis::VKCommandQueue>
wis::VKDevice::CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority) const noexcept
{
    (void)priority; // TODO: use priority

    const auto* queue = queues.GetOfType(type);
    if (queue == nullptr)
        return { wis::make_result<FUNC, "The system does not support the requested queue type">(
                         VkResult::VK_ERROR_UNKNOWN),
                 wis::VKCommandQueue{} };

    VkDeviceQueueInfo2 info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = queue->family_index,
        .queueIndex = queue->GetNextInLine(),
    };
    VkQueue queue_handle;
    device.table().vkGetDeviceQueue2(device.get(), &info, &queue_handle);
    return wis::VKCommandQueue{ device, VkQueue{ queue_handle } };
}

wis::ResultValue<wis::VKRootSignature>
wis::VKDevice::CreateRootSignature(const RootConstant* constants,
                                   uint32_t constants_size,
                                   const wis::DescriptorTable* tables,
                                   uint32_t tables_count) const noexcept
{
    wis::detail::limited_allocator<VkPushConstantRange, 8> vk_constants{ constants_size, true };

    std::unique_ptr<VkDescriptorSetLayout[]> vk_dsl;
    if (tables_count > 0) {
        if (vk_dsl = wis::detail::make_unique_for_overwrite<VkDescriptorSetLayout[]>(tables_count); !vk_dsl)
            return wis::make_result<FUNC, "Failed to allocate descriptor set layout array">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    for (uint32_t i = 0; i < constants_size; i++) {
        auto& c = vk_constants.data()[i];
        auto& r = constants[i];
        c.stageFlags = convert_vk(r.stage);
        c.offset = 0;
        c.size = r.size_bytes;
    }

    for (size_t i = 0; i < tables_count; i++) {
        auto [res, h] = CreateDescriptorSetLayout(&tables[i]);
        if (res.status != wis::Status::Ok) {
            for (size_t j = 0; j < i; j++)
                device.table().vkDestroyDescriptorSetLayout(device.get(), vk_dsl[j], nullptr);

            return res;
        }
        vk_dsl[i] = h;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = tables_count,
        .pSetLayouts = vk_dsl.get(),
        .pushConstantRangeCount = constants_size,
        .pPushConstantRanges = vk_constants.data(),
    };
    VkPipelineLayout layout;
    auto vr = device.table().vkCreatePipelineLayout(device.get(), &pipeline_layout_info, nullptr, &layout);

    if (!succeeded(vr))
        return wis::make_result<FUNC, "Failed to create a pipeline layout">(vr);

    return VKRootSignature{ wis::managed_handle_ex<VkPipelineLayout>{ layout, device, device.table().vkDestroyPipelineLayout }, std::move(vk_dsl), tables_count };
}

namespace wis::detail {
inline void VKFillShaderStage(wis::detail::uniform_allocator<VkPipelineShaderStageCreateInfo,
                                                             wis::max_shader_stages>& shader_stages,
                              wis::VKShaderView shader, VkShaderStageFlagBits stage) noexcept
{
    auto sh = std::get<0>(shader);
    if (sh == nullptr)
        return;

    shader_stages.allocate() = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = stage,
        .module = sh,
        .pName = "main",
        .pSpecializationInfo = nullptr,
    };
}
} // namespace wis::detail

wis::ResultValue<wis::VKPipelineState>
wis::VKDevice::CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept
{
    wis::detail::uniform_allocator<VkPipelineShaderStageCreateInfo, max_shader_stages> shader_stages;
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.vertex, VK_SHADER_STAGE_VERTEX_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.pixel, VK_SHADER_STAGE_FRAGMENT_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.geometry,
                                   VK_SHADER_STAGE_GEOMETRY_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.hull,
                                   VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.domain,
                                   VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);

    uint32_t ia_count = desc->input_layout.attribute_count;
    if (ia_count > ifeatures.max_ia_attributes)
        return wis::make_result<FUNC,
                                "The system does not support the requested number of vertex attributes">(
                VkResult::VK_ERROR_UNKNOWN);

    wis::detail::limited_allocator<VkVertexInputAttributeDescription, wis::max_vertex_bindings>
            attributes{ ia_count };

    uint32_t byte_offset = 0;
    auto* ia_data = attributes.data();
    for (uint32_t i = 0; i < ia_count; i++) {
        auto& ia = ia_data[i];
        auto& a = desc->input_layout.attributes[i];
        ia.binding = a.input_slot;
        ia.format = convert_vk(a.format);
        ia.location = a.location;
        ia.offset = a.offset_bytes;
    }

    VkPipelineVertexInputStateCreateInfo vertex_input{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = desc->input_layout.slot_count,
        .pVertexBindingDescriptions =
                reinterpret_cast<const VkVertexInputBindingDescription*>(desc->input_layout.slots),
        .vertexAttributeDescriptionCount = ia_count,
        .pVertexAttributeDescriptions = ia_data,
    };

    VkPipelineViewportStateCreateInfo viewport_state{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr,
    };

    constexpr static VkPipelineRasterizationStateCreateInfo default_rasterizer{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = true,
        .rasterizerDiscardEnable = false,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = false,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    VkPipelineRasterizationStateCreateInfo rasterizer;

    if (desc->rasterizer) {
        rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = desc->rasterizer->depth_clip_enable,
            .rasterizerDiscardEnable = true,
            .polygonMode = convert_vk(desc->rasterizer->fill_mode),
            .cullMode = convert_vk(desc->rasterizer->cull_mode),
            .frontFace = convert_vk(desc->rasterizer->front_face),
            .depthBiasEnable = desc->rasterizer->depth_bias_enable,
            .depthBiasConstantFactor = desc->rasterizer->depth_bias,
            .depthBiasClamp = desc->rasterizer->depth_bias_clamp,
            .depthBiasSlopeFactor = desc->rasterizer->depth_bias_slope_factor,
            .lineWidth = 1.0f,
        };
    }

    VkPipelineInputAssemblyStateCreateInfo input_assembly{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = convert_vk(desc->topology_type),
        .primitiveRestartEnable = false,
    };

    constexpr static VkPipelineColorBlendStateCreateInfo default_color_blending{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = false,
        .logicOp = VK_LOGIC_OP_NO_OP,
        .attachmentCount = 0,
        .pAttachments = nullptr,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment[max_render_targets]{};
    VkPipelineColorBlendStateCreateInfo color_blending;
    if (desc->blend) {
        auto& blend = *desc->blend;
        if (!blend.logic_op_enable) {
            for (uint32_t i = 0; i < blend.attachment_count; i++) {
                auto& a = blend.attachments[i];
                auto& b = color_blend_attachment[i];
                b.blendEnable = a.blend_enable;
                b.srcColorBlendFactor = convert_vk(a.src_color_blend);
                b.dstColorBlendFactor = convert_vk(a.dst_color_blend);
                b.colorBlendOp = convert_vk(a.color_blend_op);
                b.srcAlphaBlendFactor = convert_vk(a.src_alpha_blend);
                b.dstAlphaBlendFactor = convert_vk(a.dst_alpha_blend);
                b.alphaBlendOp = convert_vk(a.alpha_blend_op);
                b.colorWriteMask = VkColorComponentFlags(a.color_write_mask);
            }
        }

        color_blending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = blend.logic_op_enable,
            .logicOp = convert_vk(blend.logic_op),
            .attachmentCount = blend.logic_op_enable ? 0u : blend.attachment_count,
            .pAttachments = blend.logic_op_enable ? nullptr : color_blend_attachment,
            .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
        };
    }

    constexpr static VkPipelineMultisampleStateCreateInfo default_multisampling{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = false,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = false,
        .alphaToOneEnable = false,
    };

    VkPipelineMultisampleStateCreateInfo multisampling;
    if (desc->sample) {
        multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .rasterizationSamples = convert_vk(desc->sample->rate),
            .sampleShadingEnable = true,
            .minSampleShading = desc->sample->quality,
            .pSampleMask = &desc->sample->sample_mask,
            .alphaToCoverageEnable = false,
            .alphaToOneEnable = false,
        };
    }

    constexpr static VkPipelineDepthStencilStateCreateInfo default_depth_stencil{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable = false,
        .depthBoundsTestEnable = false,
        .stencilTestEnable = false,
    };
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state;

    if (desc->depth_stencil) {
        auto& ds = *desc->depth_stencil;
        depth_stencil_state = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthTestEnable = ds.depth_enable,
            .depthWriteEnable = ds.depth_write_enable,
            .depthCompareOp = convert_vk(ds.depth_comp),
            .depthBoundsTestEnable = ds.depth_bound_test,
            .stencilTestEnable = ds.stencil_enable,
            .front =
                    VkStencilOpState{
                            .failOp = convert_vk(ds.stencil_front.fail_op),
                            .passOp = convert_vk(ds.stencil_front.pass_op),
                            .depthFailOp = convert_vk(ds.stencil_front.depth_fail_op),
                            .compareOp = convert_vk(ds.stencil_front.comparison),
                            .compareMask = ds.stencil_front.read_mask,
                            .writeMask = ds.stencil_front.write_mask,
                            .reference = 0,
                    },
            .back =
                    VkStencilOpState{
                            .failOp = convert_vk(ds.stencil_back.fail_op),
                            .passOp = convert_vk(ds.stencil_back.pass_op),
                            .depthFailOp = convert_vk(ds.stencil_back.depth_fail_op),
                            .compareOp = convert_vk(ds.stencil_back.comparison),
                            .compareMask = ds.stencil_back.read_mask,
                            .writeMask = ds.stencil_back.write_mask,
                            .reference = 0,
                    },
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
        };
    }

    static constexpr size_t max_dynstates = 6;
    wis::detail::uniform_allocator<VkDynamicState, max_dynstates> dynamic_state_enables;
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_SCISSOR);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE);

    if (std::get<0>(desc->shaders.hull))
        dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT);

    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_BLEND_CONSTANTS);

    VkPipelineDynamicStateCreateInfo dynamic_state{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = uint32_t(dynamic_state_enables.size()),
        .pDynamicStates = dynamic_state_enables.data()
    };

    uint32_t rt_size = std::min(desc->attachments.attachments_count, wis::max_render_targets);
    VkFormat rt_formats[8];
    for (uint32_t i = 0; i < rt_size; i++) {
        rt_formats[i] = convert_vk(desc->attachments.attachment_formats[i]);
    }

    VkPipelineRenderingCreateInfo dynamic_rendering{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .pNext = nullptr,
        .viewMask = 0xff,
        .colorAttachmentCount = desc->attachments.attachments_count,
        .pColorAttachmentFormats = rt_formats,
        .depthAttachmentFormat = convert_vk(desc->attachments.depth_attachment),
        .stencilAttachmentFormat = VK_FORMAT_UNDEFINED // TODO: formats for pure stencils
    };

    VkPipelineCreateFlags flags =
            VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT * int(ifeatures.has_descriptor_buffer);

    VkGraphicsPipelineCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = flags,
        .stageCount = static_cast<uint32_t>(shader_stages.size()),
        .pStages = shader_stages.data(),
        .pVertexInputState = &vertex_input,
        .pInputAssemblyState = &input_assembly,
        .pViewportState = &viewport_state,
        .pRasterizationState = desc->rasterizer ? &rasterizer : &default_rasterizer,
        .pMultisampleState = desc->sample ? &multisampling : &default_multisampling,
        .pDepthStencilState = desc->depth_stencil ? &depth_stencil_state : &default_depth_stencil,
        .pColorBlendState = desc->blend ? &color_blending : &default_color_blending,
        .pDynamicState = &dynamic_state,
        .layout = std::get<0>(desc->root_signature),
    };

    VkPipeline pipeline;
    auto result = device.table().vkCreateGraphicsPipelines(device.get(), nullptr, 1u, &info, nullptr,
                                                           &pipeline);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a graphics pipeline">(result);

    return wis::VKPipelineState{
        wis::SharedPipeline{ pipeline, device, device.table().vkDestroyPipeline }
    };
}

wis::ResultValue<wis::VKCommandList>
wis::VKDevice::CreateCommandList(wis::QueueType type) const noexcept
{
    auto& dtable = device.table();
    VkCommandPoolCreateInfo cmd_pool_create_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queues.GetOfType(type)->family_index,
    };
    wis::scoped_handle<VkCommandPool> cmd_pool;
    auto result =
            dtable.vkCreateCommandPool(device.get(), &cmd_pool_create_info, nullptr, cmd_pool.put(device.get(), dtable.vkDestroyCommandPool));
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a command pool">(result);

    VkCommandBufferAllocateInfo cmd_buf_alloc_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = cmd_pool.get(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer cmd_buf;
    result = dtable.vkAllocateCommandBuffers(device.get(), &cmd_buf_alloc_info, &cmd_buf);

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to allocate a command buffer">(result);

    VkCommandBufferBeginInfo desc{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = {},
        .pInheritanceInfo = nullptr,
    };
    result = dtable.vkBeginCommandBuffer(cmd_buf, &desc);
    if (!succeeded(result)) {
        return make_result<FUNC, "vkBeginCommandBuffer failed">(result);
    }

    return wis::VKCommandList{ device, cmd_pool.release(), cmd_buf };
}

wis::ResultValue<wis::VKShader> wis::VKDevice::CreateShader(void* bytecode,
                                                            uint32_t size) const noexcept
{
    VkShaderModuleCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = size,
        .pCode = reinterpret_cast<const uint32_t*>(bytecode),
    };
    VkShaderModule shader;
    auto vr = device.table().vkCreateShaderModule(device.get(), &desc, nullptr, &shader);

    if (!succeeded(vr))
        return wis::make_result<FUNC, "Failed to create a shader module">(vr);

    return wis::VKShader{ wis::managed_handle_ex<VkShaderModule>{ shader, device, device.table().vkDestroyShaderModule } };
}

wis::ResultValue<wis::VKResourceAllocator> wis::VKDevice::CreateAllocator() const noexcept
{
    auto [result, hallocator] = CreateAllocatorI();
    if (result.status != wis::Status::Ok)
        return result;

    return VKResourceAllocator{ wis::shared_handle<VmaAllocator>{ device, hallocator },
                                allocator_functions };
}

wis::ResultValue<VmaAllocator> wis::VKDevice::CreateAllocatorI() const noexcept
{
    uint32_t version = 0;
    auto& gtable = wis::detail::VKFactoryGlobals::Instance().global_table;
    auto& itable = GetInstanceTable();
    auto& dtable = device.table();
    auto& adapter_i = adapter.GetInternal();
    gtable.vkEnumerateInstanceVersion(&version);

    if (!allocator_functions)
        allocator_functions = std::shared_ptr<VmaVulkanFunctions>{ new (std::nothrow) VmaVulkanFunctions{
                .vkGetInstanceProcAddr = gtable.vkGetInstanceProcAddr,
                .vkGetDeviceProcAddr = gtable.vkGetDeviceProcAddr,
                .vkGetPhysicalDeviceProperties = itable.vkGetPhysicalDeviceProperties,
                .vkGetPhysicalDeviceMemoryProperties = itable.vkGetPhysicalDeviceMemoryProperties,
                .vkAllocateMemory = dtable.vkAllocateMemory,
                .vkFreeMemory = dtable.vkFreeMemory,
                .vkMapMemory = dtable.vkMapMemory,
                .vkUnmapMemory = dtable.vkUnmapMemory,
                .vkFlushMappedMemoryRanges = dtable.vkFlushMappedMemoryRanges,
                .vkInvalidateMappedMemoryRanges = dtable.vkInvalidateMappedMemoryRanges,
                .vkBindBufferMemory = dtable.vkBindBufferMemory,
                .vkBindImageMemory = dtable.vkBindImageMemory,
                .vkGetBufferMemoryRequirements = dtable.vkGetBufferMemoryRequirements,
                .vkGetImageMemoryRequirements = dtable.vkGetImageMemoryRequirements,
                .vkCreateBuffer = dtable.vkCreateBuffer,
                .vkDestroyBuffer = dtable.vkDestroyBuffer,
                .vkCreateImage = dtable.vkCreateImage,
                .vkDestroyImage = dtable.vkDestroyImage,
                .vkCmdCopyBuffer = dtable.vkCmdCopyBuffer,
                .vkGetBufferMemoryRequirements2KHR = dtable.vkGetBufferMemoryRequirements2,
                .vkGetImageMemoryRequirements2KHR = dtable.vkGetImageMemoryRequirements2,
                .vkBindBufferMemory2KHR = dtable.vkBindBufferMemory2,
                .vkBindImageMemory2KHR = dtable.vkBindImageMemory2,
                .vkGetPhysicalDeviceMemoryProperties2KHR = itable.vkGetPhysicalDeviceMemoryProperties2,
                .vkGetDeviceBufferMemoryRequirements = dtable.vkGetDeviceBufferMemoryRequirements,
                .vkGetDeviceImageMemoryRequirements = dtable.vkGetDeviceImageMemoryRequirements,
        } };

    if (!allocator_functions)
        return wis::make_result<FUNC, "Failed to allocate allocator functions.">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    VmaAllocatorCreateInfo allocatorInfo{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = adapter_i.adapter,
        .device = device.get(),
        .pVulkanFunctions = allocator_functions.get(),
        .instance = adapter_i.instance.get(),
        .vulkanApiVersion = version,
    };

    VmaAllocator al;
    VkResult vr = vmaCreateAllocator(&allocatorInfo, &al);

    if (!succeeded(vr))
        return wis::make_result<FUNC, "Failed to create an Allocator">(vr);

    return al;
}

wis::ResultValue<wis::VKSwapChain>
wis::VKDevice::VKCreateSwapChain(wis::SharedSurface surface,
                                 const SwapchainDesc* desc,
                                 VkQueue graphics_queue) const noexcept
{
    auto& itable = GetInstanceTable();
    auto& dtable = device.table();
    auto hadapter = adapter.GetInternal().adapter;

    int32_t present_queue = -1;
    for (uint16_t i = 0; i < size_t(wis::detail::QueueTypes::Count); i++) {
        const auto& x = queues.available_queues[i];
        if (x.Empty())
            continue;

        VkBool32 supported = false;
        auto result = itable.vkGetPhysicalDeviceSurfaceSupportKHR(hadapter, x.family_index,
                                                                  surface.get(), &supported);
        if (!succeeded(result))
            return wis::make_result<FUNC,
                                    "Failed to check if the queue supports presentation to the surface">(
                    result);

        if (supported) {
            present_queue = i;
            lib_info(format("Present queue {} selected", i));
            break;
        }
    }
    if (present_queue == -1) {
        lib_error("None of the queues support presenting to the surface");
        return wis::make_result<FUNC, "None of the queues support presenting to the surface">(
                VkResult::VK_ERROR_UNKNOWN);
    }

    const auto& queue = queues.available_queues[present_queue];
    VkDeviceQueueInfo2 info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = queue.family_index,
        .queueIndex = queue.GetNextInLine(),
    };
    VkQueue qpresent_queue;
    dtable.vkGetDeviceQueue2(device.get(), &info, &qpresent_queue);

    uint32_t format_count = 0;
    itable.vkGetPhysicalDeviceSurfaceFormatsKHR(hadapter, surface.get(), &format_count,
                                                nullptr);
    std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
    auto result = itable.vkGetPhysicalDeviceSurfaceFormatsKHR(hadapter, surface.get(),
                                                              &format_count, surface_formats.data());

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to get surface formats">(result);

    auto format = std::ranges::find_if(surface_formats, [=](VkSurfaceFormatKHR fmt) {
        return fmt.format == convert_vk(desc->format);
    });

    if (format == surface_formats.end() || format->format == VkFormat::VK_FORMAT_UNDEFINED) {
        lib_error(wis::format("Supplied format {} is not supported by surface", +desc->format));
        return wis::make_result<FUNC, "Supplied format is not supported by surface">(VkResult::VK_ERROR_UNKNOWN);
    }

    VkSurfaceCapabilitiesKHR cap{};

    result =
            itable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(hadapter, surface.get(), &cap);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to get surface capabilities">(result);

    bool stereo = cap.maxImageArrayLayers > 1 && desc->stereo;
    if (stereo)
        lib_info("Stereo mode is ativated");

    uint32_t layers = stereo ? 2u : 1u;

    auto GetPresentMode = [&](VkSurfaceKHR surface, bool vsync) noexcept {
        constexpr VkPresentModeKHR eFifoRelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        constexpr VkPresentModeKHR eFifo = VK_PRESENT_MODE_FIFO_KHR;
        constexpr VkPresentModeKHR eImmediate = VK_PRESENT_MODE_IMMEDIATE_KHR;

        uint32_t format_count = 0;
        itable.vkGetPhysicalDeviceSurfacePresentModesKHR(hadapter, surface, &format_count,
                                                         nullptr);
        std::vector<VkPresentModeKHR> modes(format_count);
        itable.vkGetPhysicalDeviceSurfacePresentModesKHR(hadapter, surface, &format_count,
                                                         modes.data());

        return vsync ? std::ranges::count(modes, eFifoRelaxed) ? eFifoRelaxed : eFifo : eImmediate;
    };

    auto present_mode = GetPresentMode(surface.get(), desc->vsync);

    VkSwapchainCreateInfoKHR swap_info{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = surface.get(),
        .minImageCount = desc->buffer_count,
        .imageFormat = convert_vk(desc->format),
        .imageColorSpace = format->colorSpace,
        .imageExtent = { desc->size.width, desc->size.height },
        .imageArrayLayers = layers,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = cap.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = true,
        .oldSwapchain = nullptr,
    };

    wis::scoped_handle<VkSwapchainKHR> swapchain;
    result = dtable.vkCreateSwapchainKHR(device.get(), &swap_info, nullptr, swapchain.put(device.get(), dtable.vkDestroySwapchainKHR));

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a swapchain">(result);

    VkCommandPoolCreateInfo cmd_pool_create_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queues.GetOfType(wis::QueueType::Graphics)->family_index,
    };
    wis::scoped_handle<VkCommandPool> cmd_pool;
    result = dtable.vkCreateCommandPool(device.get(), &cmd_pool_create_info, nullptr, cmd_pool.put(device.get(), dtable.vkDestroyCommandPool));
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a command pool">(result);

    VkCommandBufferAllocateInfo cmd_buf_alloc_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = cmd_pool.get(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer cmd_buf;
    result = dtable.vkAllocateCommandBuffers(device.get(), &cmd_buf_alloc_info, &cmd_buf);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to allocate a command buffer">(result);

    wis::detail::VKSwapChainCreateInfo sci{ std::move(surface), device, adapter.GetInternal().adapter, itable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR, swapchain.release(),
                                            cmd_buf, cmd_pool.release(), qpresent_queue, graphics_queue, *format, present_mode, stereo, desc->stereo };

    auto rres = sci.InitBackBuffers(swap_info.imageExtent);
    if (rres.status != wis::Status::Ok)
        return rres;

    rres = sci.InitSemaphores();
    if (rres.status != wis::Status::Ok)
        return rres;

    rres = sci.AquireNextIndex();
    if (rres.status != wis::Status::Ok)
        return rres;

    return wis::VKSwapChain{ std::move(sci) };
}

wis::ResultValue<wis::VKRenderTarget>
wis::VKDevice::CreateRenderTarget(VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
{
    auto vk_format = convert_vk(desc.format);
    VkImageViewCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .image = std::get<0>(texture),
        .format = vk_format,
    };
    info.subresourceRange.aspectMask = aspect_flags(vk_format);

    switch (desc.layout) {
    case wis::TextureLayout::Texture1D:
        info.viewType = VK_IMAGE_VIEW_TYPE_1D;
        {
            info.subresourceRange.baseMipLevel = desc.mip,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = 0,
            info.subresourceRange.layerCount = 1;
        };
        break;
    case wis::TextureLayout::Texture2D:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        {
            info.subresourceRange.baseMipLevel = desc.mip,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = 0,
            info.subresourceRange.layerCount = 1;
        };
        break;
    case wis::TextureLayout::Texture3D:
        info.viewType = VK_IMAGE_VIEW_TYPE_3D;
        {
            info.subresourceRange.baseMipLevel = desc.mip,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = desc.base_array_layer,
            info.subresourceRange.layerCount = desc.layer_count;
        };
        break;
    case wis::TextureLayout::Texture1DArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        {
            info.subresourceRange.baseMipLevel = desc.mip,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = desc.base_array_layer,
            info.subresourceRange.layerCount = desc.layer_count;
        };
        break;
    case wis::TextureLayout::Texture2DArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        {
            info.subresourceRange.baseMipLevel = desc.mip,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = desc.base_array_layer,
            info.subresourceRange.layerCount = desc.layer_count;
        };
        break;
    case wis::TextureLayout::Texture2DMS:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        {
            info.subresourceRange.baseMipLevel = 0,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = 0,
            info.subresourceRange.layerCount = 1;
        };
        break;
    case wis::TextureLayout::Texture2DMSArray:
        info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        {
            info.subresourceRange.baseMipLevel = 0,
            info.subresourceRange.levelCount = 1,
            info.subresourceRange.baseArrayLayer = desc.base_array_layer,
            info.subresourceRange.layerCount = desc.layer_count;
        };
        break;
    default:
        break;
    }

    VkImageView view;
    auto result = device.table().vkCreateImageView(device.get(), &info, nullptr, &view);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create an image view">(result);

    return VKRenderTarget{ wis::managed_handle_ex<VkImageView>{ view, device, device.table().vkDestroyImageView }, std::get<2>(texture) };
}

wis::ResultValue<wis::VKDescriptorBuffer>
wis::VKDevice::CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint32_t descriptor_count) const noexcept
{
    if (!allocator) {
        auto [res, hallocator] = CreateAllocatorI();
        if (res.status != wis::Status::Ok)
            return res;
        allocator = wis::shared_handle<VmaAllocator>{ device, hallocator };
    }
    if (!allocator)
        return wis::make_result<FUNC, "Failed to create an allocator">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    uint32_t descriptor_size = heap_type == wis::DescriptorHeapType::Descriptor
            ? feature_details->mutable_descriptor_size
            : feature_details->descriptor_buffer_properties.samplerDescriptorSize;

    VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
            (heap_type == wis::DescriptorHeapType::Descriptor
                     ? VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT
                     : VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT);

    wis::DescriptorType desc_type = heap_type == wis::DescriptorHeapType::Descriptor
            ? wis::DescriptorType::ShaderResource
            : wis::DescriptorType::Sampler;

    wis::DescriptorTableEntry entry{
        .type = desc_type,
        .bind_register = 0,
        .count = descriptor_count,
    };

    wis::DescriptorTable table{
        .type = heap_type,
        .entries = &entry,
        .entry_count = 1,
        .stage = wis::ShaderStages::All,
    };
    auto [res, dsl] = CreateDescriptorSetLayout(&table);
    if (res.status != wis::Status::Ok)
        return res;

    VkDeviceSize descriptor_offset = 0;
    VkDeviceSize descriptor_set_size = 0;

    device.table().vkGetDescriptorSetLayoutBindingOffsetEXT(device.get(), dsl, 0, &descriptor_offset);
    device.table().vkGetDescriptorSetLayoutSizeEXT(device.get(), dsl, &descriptor_set_size);
    device.table().vkDestroyDescriptorSetLayout(device.get(), dsl, nullptr);

    VkBufferCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = wis::detail::aligned_size(descriptor_set_size, feature_details->descriptor_buffer_properties.descriptorBufferOffsetAlignment),
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };

    VmaAllocationCreateInfo alloc_info{
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .preferredFlags = 0,
    };

    VkBuffer buffer;
    VmaAllocation allocation;

    auto result = vmaCreateBuffer(allocator.get(), &info, &alloc_info, &buffer, &allocation, nullptr);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a descriptor heap buffer">(result);

    return VKDescriptorBuffer{ allocator, buffer, allocation, heap_type, feature_details->descriptor_buffer_properties, uint32_t(descriptor_size), uint32_t(descriptor_offset) };
}

wis::ResultValue<VkDescriptorSetLayout>
wis::VKDevice::CreateDescriptorSetDescriptorLayout(const wis::DescriptorTable* table) const noexcept
{
    constexpr static VkDescriptorType cbvSrvUavTypes[] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR /* Need to check support if this is desired. */
    };

    constexpr static VkMutableDescriptorTypeListVALVE a{
        .descriptorTypeCount = sizeof(cbvSrvUavTypes) / sizeof(VkDescriptorType),
        .pDescriptorTypes = cbvSrvUavTypes
    };

    wis::detail::limited_allocator<VkDescriptorSetLayoutBinding, 32> bindings{ table->entry_count, true };
    wis::detail::limited_allocator<VkMutableDescriptorTypeListVALVE, 32> bindings_mutable{ table->entry_count, true };

    for (size_t i = 0; i < table->entry_count; i++) {
        bindings_mutable.data()[i] = a;

        auto& entry = table->entries[i];
        bindings.data()[i] = {
            .binding = entry.binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_MUTABLE_EXT,
            .descriptorCount = entry.count,
            .stageFlags = uint32_t(convert_vk(table->stage)),
            .pImmutableSamplers = nullptr,
        };
    }
    VkMutableDescriptorTypeCreateInfoEXT mutableTypeInfo{
        .sType = VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_EXT,
        .pNext = nullptr,
        .mutableDescriptorTypeListCount = table->entry_count,
        .pMutableDescriptorTypeLists = bindings_mutable.data(),
    };

    VkDescriptorSetLayoutCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &mutableTypeInfo,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
        .bindingCount = table->entry_count,
        .pBindings = bindings.data(),
    };

    VkDescriptorSetLayout layout;
    auto result = device.table().vkCreateDescriptorSetLayout(device.get(), &desc, nullptr, &layout);

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a descriptor set layout">(result);

    return layout;
}

wis::ResultValue<VkDescriptorSetLayout>
wis::VKDevice::CreateDescriptorSetSamplerLayout(const wis::DescriptorTable* table) const noexcept
{
    wis::detail::limited_allocator<VkDescriptorSetLayoutBinding, 32> bindings{ table->entry_count, true };

    for (size_t i = 0; i < table->entry_count; i++) {
        auto& entry = table->entries[i];
        bindings.data()[i] = {
            .binding = entry.binding,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .descriptorCount = entry.count,
            .stageFlags = uint32_t(convert_vk(table->stage)),
            .pImmutableSamplers = nullptr,
        };
    }

    VkDescriptorSetLayoutCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
        .bindingCount = table->entry_count,
        .pBindings = bindings.data(),
    };

    VkDescriptorSetLayout layout;
    auto result = device.table().vkCreateDescriptorSetLayout(device.get(), &desc, nullptr, &layout);

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a descriptor set layout">(result);

    return layout;
}

wis::ResultValue<wis::VKSampler>
wis::VKDevice::CreateSampler(const wis::SamplerDesc* desc) const noexcept
{
    VkSamplerCustomBorderColorCreateInfoEXT custom_border_color{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT,
        .pNext = nullptr,
        .customBorderColor = {
                desc->border_color[0],
                desc->border_color[1],
                desc->border_color[2],
                desc->border_color[3],
        },
    };
    VkSamplerCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = &custom_border_color,
        .flags = 0,
        .magFilter = convert_vk(desc->mag_filter),
        .minFilter = convert_vk(desc->min_filter),
        .mipmapMode = VkSamplerMipmapMode(desc->mip_filter),
        .addressModeU = convert_vk(desc->address_u),
        .addressModeV = convert_vk(desc->address_v),
        .addressModeW = convert_vk(desc->address_w),
        .mipLodBias = desc->mip_lod_bias,
        .anisotropyEnable = desc->anisotropic,
        .maxAnisotropy = float(desc->max_anisotropy),
        .compareEnable = desc->comparison_op != wis::Compare::Never,
        .compareOp = convert_vk(desc->comparison_op),
        .minLod = desc->min_lod,
        .maxLod = desc->max_lod,
        .borderColor = VkBorderColor::VK_BORDER_COLOR_FLOAT_CUSTOM_EXT
    };

    VkSampler sampler;
    auto result = device.table().vkCreateSampler(device.get(), &info, nullptr, &sampler);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a sampler">(result);
    return wis::VKSampler{ wis::managed_handle_ex<VkSampler>{ sampler, device, device.table().vkDestroySampler } };
}

wis::ResultValue<wis::VKShaderResource>
wis::VKDevice::CreateShaderResource(wis::VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept
{
    VkImageViewCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .image = std::get<0>(texture),
        .viewType = convert_vk(desc.view_type),
        .format = convert_vk(desc.format),
        .components = {
                .r = convert_vk(desc.component_mapping.r),
                .g = convert_vk(desc.component_mapping.g),
                .b = convert_vk(desc.component_mapping.b),
                .a = convert_vk(desc.component_mapping.a),
        },
        .subresourceRange = {
                .aspectMask = aspect_flags(convert_vk(desc.format)),
        },
    };

    switch (desc.view_type) {
    default:
    case wis::TextureViewType::Texture1D:
    case wis::TextureViewType::Texture2D:
        info.subresourceRange.baseMipLevel = desc.subresource_range.base_mip_level;
        info.subresourceRange.levelCount = desc.subresource_range.level_count;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        break;
    case wis::TextureViewType::Texture1DArray:
    case wis::TextureViewType::Texture2DArray:
        info.subresourceRange.baseMipLevel = desc.subresource_range.base_mip_level;
        info.subresourceRange.levelCount = desc.subresource_range.level_count;
        info.subresourceRange.baseArrayLayer = desc.subresource_range.base_array_layer;
        info.subresourceRange.layerCount = desc.subresource_range.layer_count;
        break;
    case wis::TextureViewType::Texture2DMS:
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        break;
    case wis::TextureViewType::Texture2DMSArray:
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = desc.subresource_range.base_array_layer;
        info.subresourceRange.layerCount = desc.subresource_range.layer_count;
        break;
    case wis::TextureViewType::Texture3D:
        info.subresourceRange.baseMipLevel = desc.subresource_range.base_mip_level;
        info.subresourceRange.levelCount = desc.subresource_range.level_count;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        break;
    case wis::TextureViewType::TextureCube:
        info.subresourceRange.baseMipLevel = desc.subresource_range.base_mip_level;
        info.subresourceRange.levelCount = desc.subresource_range.level_count;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 6;
        break;
    case wis::TextureViewType::TextureCubeArray:
        info.subresourceRange.baseMipLevel = desc.subresource_range.base_mip_level;
        info.subresourceRange.levelCount = desc.subresource_range.level_count;
        info.subresourceRange.baseArrayLayer = desc.subresource_range.base_array_layer;
        info.subresourceRange.layerCount = desc.subresource_range.layer_count;
        break;
    }

    VkImageView view;
    auto res = device.table().vkCreateImageView(device.get(), &info, nullptr, &view);
    if (!succeeded(res))
        return wis::make_result<FUNC, "Failed to create an image view">(res);

    return wis::VKShaderResource{ wis::managed_handle_ex<VkImageView>{ view, device, device.table().vkDestroyImageView } };
}

// wis::ResultValue< VkDescriptorSetLayout>
// wis::VKDevice::CreatePushDescriptorLayout(wis::PushDescriptor desc) const noexcept {
//   VkDescriptorSetLayoutBinding binding{
//       .binding = desc.bind_register,
//       .descriptorType = convert_vk(desc.type),
//       .descriptorCount = 1,
//       .stageFlags = VkShaderStageFlags(convert_vk(desc.stage)),
//       .pImmutableSamplers = nullptr,
//   };
//   VkDescriptorSetLayoutCreateInfo dsl{
//       .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//       .pNext = nullptr,
//       .flags = VkDescriptorSetLayoutCreateFlags(
//           VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT *
//               int(ifeatures.has_descriptor_buffer) |
//           VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR),
//       .bindingCount = 1,
//       .pBindings = &binding,
//   };
//
//   VkDescriptorSetLayout layout;
//   auto vr = device.table()->vkCreateDescriptorSetLayout(device.get(), &dsl, nullptr, &layout);
//
//   return succeeded(vr)
//              ? std::pair{wis::success, layout}
//              : std::pair{wis::make_result<FUNC, "Failed to create a descriptor set layout">(vr),
//                          VkDescriptorSetLayout{}};
// }
//