#ifndef WIS_VK_DEVICE_CPP
#define WIS_VK_DEVICE_CPP
#include <wisdom/vulkan/vk_device.h>

#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_external.h>

inline wis::detail::QueueResidency
GetQueueFamilies(VkPhysicalDevice adapter, const wis::VKMainInstance& itable) noexcept
{
    using namespace wis::detail;
    using wis::operator+;

    VkResult result = VK_SUCCESS;
    uint32_t count = 0;
    itable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, nullptr);
    auto family_props = wis::detail::make_fixed_allocation<VkQueueFamilyProperties>(count);
    itable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, family_props.get_data());

    wis::lib_info(wis::format("The system supports {} queue families", family_props.size));
    assert(family_props.size < 256);

    QueueResidency queues;

    // NOLINTNEXTLINE
    for (uint8_t i = 0; i < family_props.size; i++) {
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

inline static std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>
GetAvailableExtensions(wis::Result& result, VkPhysicalDevice adapter, const wis::VKMainInstance& itable)
{
    std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>> ext_map;

    uint32_t count = 0;
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, nullptr);
    wis::detail::fixed_allocation<VkExtensionProperties> ext_props = wis::detail::make_fixed_allocation<VkExtensionProperties>(count);
    if (!ext_props) {
        result = wis::make_result<FUNC, "Failed to allocate memory for extension properties">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return ext_map;
    }
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, ext_props.get());

    ext_map.reserve(count);
    for (auto& ext : ext_props) {
        ext_map.emplace(ext.extensionName, ext);
    }
    return ext_map;
}

wis::VKDevice
wis::ImplVKCreateDevice(wis::Result& result, wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept
{
    VKDevice out_device;
    auto& internal = out_device.GetMutableInternal();

    auto& adapter_i = in_adapter.GetInternal();
    auto hadapter = adapter_i.adapter;
    auto& itable = adapter_i.instance.table();
    auto& gtable = adapter_i.instance.gtable();

    std::span<wis::VKDeviceExtension*> exts_span{ exts, exts + ext_size };
    std::unordered_map<VkStructureType, uintptr_t> struct_map;
    std::unordered_map<VkStructureType, uintptr_t> property_map;
    std::unordered_set<std::string_view> ext_name_set;
    auto available_exts = GetAvailableExtensions(result, hadapter, itable);
    if (result.status != wis::Status::Ok) {
        return out_device;
    }

    for (auto*& ext : exts_span) {
        bool supported = ext->GetExtensionInfo(available_exts, ext_name_set, struct_map, property_map);
        if (!supported) {
            ext = nullptr;
            continue;
        }
    }

    // Ext1
    if (!internal.ext1.GetExtensionInfo(available_exts, ext_name_set, struct_map, property_map)) {
        result = wis::make_result<FUNC, "Failed to get base extensions to run device">(VkResult::VK_ERROR_UNKNOWN);
        return out_device;
    }

    // Allocate memory for extension names
    auto ext_names = wis::detail::make_fixed_allocation<const char*>(ext_name_set.size());
    if (!ext_names) {
        result = wis::make_result<FUNC, "Failed to allocate memory for extension names">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_device;
    }

    size_t i = 0;
    for (auto& ext : ext_name_set)
        ext_names[i++] = ext.data();

    // Initialize features
    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = nullptr,
        .features = {},
    };
    VkPhysicalDeviceVulkan11Features vulkan11_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = nullptr,
    };
    features.pNext = &vulkan11_features;

    VkPhysicalDeviceVulkan12Features vulkan12_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = nullptr,
    };
    vulkan11_features.pNext = &vulkan12_features;

    // Allocate memory for all structures
    size_t allocation_size = 0;
    for (auto& [type, size] : struct_map)
        allocation_size += size;

    auto allocation = wis::detail::make_fixed_allocation<uint8_t>(allocation_size);
    if (!allocation) {
        result = wis::make_result<FUNC, "Failed to allocate memory for feature structures">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_device;
    }
    memset(allocation.get_data(), 0, allocation_size);

    VkBaseInStructure* linked_struct = reinterpret_cast<VkBaseInStructure*>(&vulkan12_features);
    uint8_t* current = allocation.get_data();
    for (auto& [type, size] : struct_map) {
        auto* ptr = reinterpret_cast<VkBaseInStructure*>(current);
        ptr->sType = type;
        ptr->pNext = nullptr;
        linked_struct->pNext = ptr;
        linked_struct = ptr;
        current += size;

        // override to the structure pointer
        size = reinterpret_cast<uintptr_t>(ptr);
    }

    // Add the structures to the map
    struct_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES] = reinterpret_cast<uintptr_t>(&vulkan12_features);
    struct_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES] = reinterpret_cast<uintptr_t>(&vulkan11_features);
    struct_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2] = reinterpret_cast<uintptr_t>(&features);

    // Initialize
    itable.vkGetPhysicalDeviceFeatures2(hadapter, &features);

    // Get the properties
    VkPhysicalDeviceProperties2 props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = nullptr,
        .properties = {},
    };

    VkPhysicalDeviceVulkan11Properties vulkan11_props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
        .pNext = nullptr,
    };
    props.pNext = &vulkan11_props;

    VkPhysicalDeviceVulkan12Properties vulkan12_props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
        .pNext = nullptr,
    };
    vulkan11_props.pNext = &vulkan12_props;

    // Allocate memory for all properties
    allocation_size = 0;
    for (auto& [type, size] : property_map)
        allocation_size += size;

    auto allocation_props = wis::detail::make_fixed_allocation<uint8_t>(allocation_size);
    if (!allocation_props) {
        result = wis::make_result<FUNC, "Failed to allocate memory for property structures">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_device;
    }
    memset(allocation_props.get_data(), 0, allocation_size);

    VkBaseInStructure* linked_prop = reinterpret_cast<VkBaseInStructure*>(&vulkan12_props);
    current = allocation_props.get_data();
    for (auto& [type, size] : property_map) {
        auto* ptr = reinterpret_cast<VkBaseInStructure*>(current);
        ptr->sType = type;
        ptr->pNext = nullptr;
        linked_prop->pNext = ptr;
        linked_prop = ptr;
        current += size;

        // override to the structure pointer
        size = reinterpret_cast<uintptr_t>(ptr);
    }

    // Add the structures to the map
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES] = reinterpret_cast<uintptr_t>(&vulkan12_props);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES] = reinterpret_cast<uintptr_t>(&vulkan11_props);
    property_map[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2] = reinterpret_cast<uintptr_t>(&props);

    // Get the properties
    itable.vkGetPhysicalDeviceProperties2(hadapter, &props);

    // Initialize queue families
    constexpr static auto max_queue_count = +wis::detail::QueueTypes::Count;
    wis::detail::uniform_allocator<VkDeviceQueueCreateInfo, max_queue_count> queue_infos{};

    internal.queues = GetQueueFamilies(hadapter, itable);

    constexpr static auto priorities = []() {
        std::array<float, 64> priorities{};
        priorities.fill(1.0f);
        return priorities;
    }
    ();

    for (size_t queue_info_size = 0; queue_info_size < max_queue_count; queue_info_size++) {
        auto& q = internal.queues.available_queues[queue_info_size];
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

    VkDeviceCreateInfo device_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size()),
        .pQueueCreateInfos = queue_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(ext_name_set.size()),
        .ppEnabledExtensionNames = ext_names.get(),
        .pEnabledFeatures = nullptr,
    };

    // Creating device
    VkDevice device;
    auto vres = itable.vkCreateDevice(hadapter, &device_info, nullptr, &device);
    if (!wis::succeeded(vres)) {
        result = wis::make_result<FUNC, "vkCreateDevice failed to create device">(vres);
        return out_device;
    }
    wis::managed_handle<VkDevice> managed_device{ device, (PFN_vkDestroyDevice)gtable.vkGetDeviceProcAddr(device, "vkDestroyDevice") };

    std::unique_ptr<VKMainDevice> device_table = wis::detail::make_unique<VKMainDevice>();
    if (!device_table) {
        result = wis::make_result<FUNC, "Failed to allocate device table">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_device;
    }

    if (!device_table->Init(device, gtable.vkGetDeviceProcAddr)) {
        result = wis::make_result<FUNC, "Failed to initialize device table">(VkResult::VK_ERROR_UNKNOWN);
        return out_device;
    }

    // Create the device
    internal.adapter = std::move(in_adapter);
    internal.device = wis::SharedDevice{ managed_device.release(), std::move(device_table), &gtable };
    internal.ext1.Init(out_device, struct_map, property_map);
    if (!internal.ext1.Supported() && !force) {
        result = wis::make_result<FUNC, "Failed to initialize embedded extensions">(VkResult::VK_ERROR_UNKNOWN);
        return out_device;
    }

    // Init Default Layout
    result = internal.InitDefaultLayout();
    if (result.status != wis::Status::Ok) {
        return out_device;
    }

    // Create Default Allocator
    internal.allocator = out_device.VKCreateAllocator(result);
    if (result.status != wis::Status::Ok) {
        return out_device;
    }

    // Init the rest of the extensions
    for (auto*& ext : exts_span) {
        if (ext == nullptr)
            continue;
        ext->Init(out_device, struct_map, property_map);
    }

    return out_device;
}

wis::Result wis::ImplVKDevice::WaitForMultipleFences(const VKFenceView* fences, const uint64_t* values,
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

wis::VKFence
wis::ImplVKDevice::CreateFence(wis::Result& result, uint64_t initial_value, wis::FenceFlags flags) const noexcept
{
    VKFence out_fence;
    auto& internal = out_fence.GetMutableInternal();

    constexpr static VkExportSemaphoreCreateInfo export_info{
        .sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO,
        .handleTypes = detail::semaphore_handle_type
    };

    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = flags & FenceFlags::Shared && ext1.GetFeatures().interop_device ? &export_info : nullptr,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
        .flags = 0,
    };

    VkResult vr = device.table().vkCreateSemaphore(device.get(), &desc, nullptr, internal.fence.put(device, device.table().vkDestroySemaphore));

    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "vkCreateSemaphore failed to create a timeline semaphore.">(vr);
        return out_fence;
    }
    return out_fence;
}

wis::VKCommandQueue
wis::ImplVKDevice::CreateCommandQueue(wis::Result& result, wis::QueueType type) const noexcept
{
    VKCommandQueue out_queue;
    auto& internal = out_queue.GetMutableInternal();
    const auto* queue = queues.GetOfType(type);
    if (queue == nullptr) {
        result = wis::make_result<FUNC, "The system does not support the requested queue type">(VkResult::VK_ERROR_UNKNOWN);
        return out_queue;
    }
    VkDeviceQueueInfo2 info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = queue->family_index,
        .queueIndex = queue->GetNextInLine(),
    };
    device.table().vkGetDeviceQueue2(device.get(), &info, &internal.queue);
    internal.device = device;
    return out_queue;
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

wis::VKPipelineState
wis::ImplVKDevice::CreateGraphicsPipeline(wis::Result& result, const wis::VKGraphicsPipelineDesc& desc) const noexcept
{
    VKPipelineState out_pipeline;
    auto& internal = out_pipeline.GetMutableInternal();

    wis::detail::uniform_allocator<VkPipelineShaderStageCreateInfo, max_shader_stages> shader_stages;
    wis::detail::VKFillShaderStage(shader_stages, desc.shaders.vertex, VK_SHADER_STAGE_VERTEX_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc.shaders.pixel, VK_SHADER_STAGE_FRAGMENT_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc.shaders.geometry,
                                   VK_SHADER_STAGE_GEOMETRY_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc.shaders.hull,
                                   VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc.shaders.domain,
                                   VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);

    uint32_t ia_count = desc.input_layout.attribute_count;
    if (ia_count > ext1.GetInternal().base_properties.max_ia_attributes) {
        result = wis::make_result<FUNC,
               "The system does not support the requested number of vertex attributes">(
                   VkResult::VK_ERROR_UNKNOWN);
        return out_pipeline;
    }

    wis::detail::limited_allocator<VkVertexInputAttributeDescription, wis::max_vertex_bindings>
    attributes{ ia_count };

    uint32_t byte_offset = 0;
    auto* ia_data = attributes.data();
    for (uint32_t i = 0; i < ia_count; i++) {
        auto& ia = ia_data[i];
        auto& a = desc.input_layout.attributes[i];
        ia.binding = a.input_slot;
        ia.format = convert_vk(a.format);
        ia.location = a.location;
        ia.offset = a.offset_bytes;
    }

    VkPipelineVertexInputStateCreateInfo vertex_input{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = desc.input_layout.slot_count,
        .pVertexBindingDescriptions = desc.input_layout.slot_count ? reinterpret_cast<const VkVertexInputBindingDescription*>(desc.input_layout.slots) : nullptr,
        .vertexAttributeDescriptionCount = ia_count,
        .pVertexAttributeDescriptions = ia_count ? ia_data : nullptr,
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

    if (desc.rasterizer) {
        rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = desc.rasterizer->depth_clip_enable,
            .rasterizerDiscardEnable = false,
            .polygonMode = convert_vk(desc.rasterizer->fill_mode),
            .cullMode = convert_vk(desc.rasterizer->cull_mode),
            .frontFace = convert_vk(desc.rasterizer->front_face),
            .depthBiasEnable = desc.rasterizer->depth_bias_enable,
            .depthBiasConstantFactor = desc.rasterizer->depth_bias,
            .depthBiasClamp = desc.rasterizer->depth_bias_clamp,
            .depthBiasSlopeFactor = desc.rasterizer->depth_bias_slope_factor,
            .lineWidth = 1.0f,
        };
    }

    VkPipelineInputAssemblyStateCreateInfo input_assembly{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = convert_vk(desc.topology_type),
        .primitiveRestartEnable = false,
    };

    //--Render targets
    uint32_t rt_size = std::min(desc.attachments.attachments_count, wis::max_render_targets);
    VkFormat rt_formats[8] {};
    for (uint32_t i = 0; i < rt_size; i++) {
        rt_formats[i] = convert_vk(desc.attachments.attachment_formats[i]);
    }

    VkPipelineRenderingCreateInfo dynamic_rendering{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .pNext = nullptr,
        .viewMask = ext1.GetFeatures().multiview ? desc.view_mask : 0,
        .colorAttachmentCount = rt_size,
        .pColorAttachmentFormats = rt_formats,
        .depthAttachmentFormat = convert_vk(desc.attachments.depth_attachment),
        .stencilAttachmentFormat = VK_FORMAT_UNDEFINED // TODO: formats for pure stencils
    };

    //--Color blending
    constexpr static VkPipelineColorBlendAttachmentState default_color_blend_attachment{
        .blendEnable = false,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };
    VkPipelineColorBlendAttachmentState color_blend_attachment[max_render_targets] {};
    VkPipelineColorBlendStateCreateInfo color_blending;
    for (uint32_t i = 0; i < rt_size; i++)
        color_blend_attachment[i] = default_color_blend_attachment;

    if (desc.blend) {
        auto& blend = *desc.blend;
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
    } else {
        color_blending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_NO_OP,
            .attachmentCount = rt_size,
            .pAttachments = color_blend_attachment,
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
    if (desc.sample) {
        multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .rasterizationSamples = convert_vk(desc.sample->rate),
            .sampleShadingEnable = true,
            .minSampleShading = desc.sample->quality,
            .pSampleMask = &desc.sample->sample_mask,
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

    if (desc.depth_stencil) {
        auto& ds = *desc.depth_stencil;
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

    if (ia_count)
        dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE);

    if (std::get<0>(desc.shaders.hull))
        dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT);

    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_BLEND_CONSTANTS);

    VkPipelineDynamicStateCreateInfo dynamic_state{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = uint32_t(dynamic_state_enables.size()),
        .pDynamicStates = dynamic_state_enables.data()
    };

    VkPipelineCreateFlags flags = convert_vk(desc.flags);

    VkGraphicsPipelineCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &dynamic_rendering,
        .flags = flags,
        .stageCount = static_cast<uint32_t>(shader_stages.size()),
        .pStages = shader_stages.data(),
        .pVertexInputState = &vertex_input,
        .pInputAssemblyState = &input_assembly,
        .pViewportState = &viewport_state,
        .pRasterizationState = desc.rasterizer ? &rasterizer : &default_rasterizer,
        .pMultisampleState = desc.sample ? &multisampling : &default_multisampling,
        .pDepthStencilState = desc.depth_stencil ? &depth_stencil_state : &default_depth_stencil,
        .pColorBlendState = &color_blending,
        .pDynamicState = &dynamic_state,
        .layout = std::get<0>(desc.root_signature),
    };

    auto vr = device.table().vkCreateGraphicsPipelines(device.get(), nullptr, 1u, &info, nullptr,
              internal.pipeline.put_unsafe(device, device.table().vkDestroyPipeline));
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a graphics pipeline">(vr);
    }
    return out_pipeline;
}

wis::VKCommandList
wis::ImplVKDevice::CreateCommandList(wis::Result& result, wis::QueueType type) const noexcept
{
    VKCommandList out_list;
    auto& internal = out_list.GetMutableInternal();

    auto& dtable = device.table();
    VkCommandPoolCreateInfo cmd_pool_create_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queues.GetOfType(type)->family_index,
    };
    wis::scoped_handle<VkCommandPool> cmd_pool;
    auto vr =
        dtable.vkCreateCommandPool(device.get(), &cmd_pool_create_info, nullptr, cmd_pool.put(device.get(), dtable.vkDestroyCommandPool));
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a command pool">(vr);
        return out_list;
    }

    VkCommandBufferAllocateInfo cmd_buf_alloc_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = cmd_pool.get(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    vr = dtable.vkAllocateCommandBuffers(device.get(), &cmd_buf_alloc_info, &internal.command_list);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to allocate a command buffer">(vr);
        return out_list;
    }

    internal.device = device;
    internal.allocator = cmd_pool.release();

    VkCommandBufferBeginInfo desc{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = {},
        .pInheritanceInfo = nullptr,
    };
    vr = dtable.vkBeginCommandBuffer(internal.command_list, &desc);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "vkBeginCommandBuffer failed">(vr);
    }
    return out_list;
}

wis::VKShader
wis::ImplVKDevice::CreateShader(wis::Result& result, void* bytecode,
                                uint32_t size) const noexcept
{
    VKShader out_shader;
    auto& internal = out_shader.GetMutableInternal();

    VkShaderModuleCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = size,
        .pCode = reinterpret_cast<const uint32_t*>(bytecode),
    };
    auto vr = device.table().vkCreateShaderModule(device.get(), &desc, nullptr, internal.shader.put(device, device.table().vkDestroyShaderModule));

    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a shader module">(vr);
    }
    return out_shader;
}

wis::VKResourceAllocator
wis::ImplVKDevice::CreateAllocator(wis::Result& result) const noexcept
{
    VKResourceAllocator allocator;
    auto& internal = allocator.GetMutableInternal();

    wis::shared_handle<VmaAllocator> interop;
    if (ext1.GetFeatures().interop_device) {
        internal.export_memory_allocator = VKCreateAllocator(result, true);
        if (result.status != wis::Status::Ok)
            return allocator;
    }
    internal.allocator = this->allocator;
    return allocator;
}

wis::shared_handle<VmaAllocator>
wis::ImplVKDevice::VKCreateAllocator(wis::Result& result, bool interop) const noexcept
{
    uint32_t version = 0;
    auto& itable = GetInstanceTable();
    auto& dtable = device.table();
    auto& gtable = device.gtable();
    auto& adapter_i = adapter.GetInternal();
    gtable.vkEnumerateInstanceVersion(&version);

    VmaVulkanFunctions allocator_functions{
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
    };

    VkPhysicalDeviceMemoryProperties2 mem_props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
    };
    itable.vkGetPhysicalDeviceMemoryProperties2(adapter_i.adapter, &mem_props);

    constexpr static std::array<VkExternalMemoryHandleTypeFlagsKHR, VK_MAX_MEMORY_TYPES> handle_types = [] {
        std::array<VkExternalMemoryHandleTypeFlagsKHR, VK_MAX_MEMORY_TYPES> handle_types{};
        handle_types.fill(detail::memory_handle_type);
        return handle_types;
    }();

    VmaAllocatorCreateInfo allocatorInfo{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT | VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE4_BIT,
        .physicalDevice = adapter_i.adapter,
        .device = device.get(),
        .pVulkanFunctions = &allocator_functions,
        .instance = adapter_i.instance.get(),
        .vulkanApiVersion = version,
        .pTypeExternalMemoryHandleTypes = ext1.GetFeatures().interop_device && interop ? handle_types.data() : nullptr
    };

    if (ext1.GetFeatures().index_buffer_range)
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE5_BIT;

    // Only if there is an interop extension
    if (ext1.GetFeatures().interop_device && interop) {
#ifdef _WIN32
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT;
        (void*&)allocator_functions.vkGetMemoryWin32HandleKHR = GetDeviceProcAddr<void*>("vkGetMemoryWin32HandleKHR");
#endif // WIN32
    }

    wis::shared_handle<VmaAllocator> out_allocator;
    VkResult vr = vmaCreateAllocator(&allocatorInfo, out_allocator.put_unsafe(device));

    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create an Allocator">(vr);
    }
    return out_allocator;
}

wis::VKSwapChain
wis::ImplVKDevice::VKCreateSwapChain(wis::Result& result, wis::SharedSurface surface,
                                     const SwapchainDesc* desc,
                                     VkQueue graphics_queue,
                                     void* pNext) const noexcept
{
    VKSwapChain out_swapchain;
    auto& internal = out_swapchain.GetMutableInternal();

    auto& itable = GetInstanceTable();
    auto& dtable = device.table();
    auto hadapter = adapter.GetInternal().adapter;

    // Get the surface capabilities and present queue
    {
        int32_t present_queue = -1;
        auto hadapter = adapter.GetInternal().adapter;
        for (uint16_t i = 0; i < size_t(wis::detail::QueueTypes::Count); i++) {
            const auto& x = queues.available_queues[i];
            if (x.Empty())
                continue;

            VkBool32 supported = false;
            auto vr = itable.vkGetPhysicalDeviceSurfaceSupportKHR(hadapter, x.family_index,
                      surface.get(), &supported);
            if (!succeeded(vr)) {
                result = wis::make_result<FUNC, "Failed to check if the queue supports presentation to the surface">(vr);
                return out_swapchain;
            }

            if (supported) {
                present_queue = i;
                break;
            }
        }
        if (present_queue == -1) {
            result = wis::make_result<FUNC, "None of the queues support presenting to the surface">(VkResult::VK_ERROR_UNKNOWN);
            return out_swapchain;
        }
        const auto& queue = queues.available_queues[present_queue];
        VkDeviceQueueInfo2 info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = queue.family_index,
            .queueIndex = queue.GetNextInLine(),
        };
        dtable.vkGetDeviceQueue2(device.get(), &info, &internal.present_queue);
        internal.graphics_queue = graphics_queue;
    }

    uint32_t format_count = 0;
    itable.vkGetPhysicalDeviceSurfaceFormatsKHR(hadapter, surface.get(), &format_count,
            nullptr);
    wis::detail::fixed_allocation<VkSurfaceFormatKHR> surface_formats = wis::detail::make_fixed_allocation<VkSurfaceFormatKHR>(format_count);
    if (!surface_formats) {
        result = wis::make_result<FUNC, "Failed to allocate memory for surface formats">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_swapchain;
    }
    auto vr = itable.vkGetPhysicalDeviceSurfaceFormatsKHR(hadapter, surface.get(),
              &format_count, surface_formats.get());

    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to get surface formats">(vr);
        return out_swapchain;
    }

    auto format = std::ranges::find_if(surface_formats, [=](VkSurfaceFormatKHR fmt) {
        return fmt.format == convert_vk(desc->format);
    });

    if (format == surface_formats.end() || format->format == VkFormat::VK_FORMAT_UNDEFINED) {
        result = wis::make_result<FUNC, "Supplied format is not supported by surface">(VkResult::VK_ERROR_UNKNOWN); // TODO: Make more meaningful error
        return out_swapchain;
    }

    VkSurfaceCapabilitiesKHR cap{};
    vr = itable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(hadapter, surface.get(), &cap);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to get surface capabilities">(vr);
        return out_swapchain;
    }
    bool stereo = cap.maxImageArrayLayers > 1 && desc->stereo;
    if (stereo)
        lib_info("Stereo mode is ativated");

    uint32_t layers = stereo ? 2u : 1u;

    // Get present modes
    uint32_t presentation_count = 0;
    itable.vkGetPhysicalDeviceSurfacePresentModesKHR(hadapter, surface.get(), &presentation_count,
            nullptr);
    assert(presentation_count <= 16);
    std::array<VkPresentModeKHR, 16> modes{};
    itable.vkGetPhysicalDeviceSurfacePresentModesKHR(hadapter, surface.get(), &presentation_count,
            modes.data());

    auto present_mode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
    bool tearing = desc->tearing;
    if (!desc->vsync) {
        if (desc->tearing) {
            if (tearing = std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR) > 0)
                present_mode = VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
            else if (tearing = std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR) > 0)
                present_mode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        } else if (std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR) > 0 && !stereo) {
            present_mode = VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    constexpr static uint32_t present_modes_count = 8;
    std::array<VkPresentModeKHR, present_modes_count> compatible_modes{};
    uint8_t supported_presentation = 0;
    uint32_t compatible_modes_count = 0;

    VkSwapchainCreateInfoKHR swap_info{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = pNext,
        .flags = 0,
        .surface = surface.get(),
        .minImageCount = desc->buffer_count,
        .imageFormat = convert_vk(desc->format),
        .imageColorSpace = format->colorSpace,
        .imageExtent = {
            std::clamp(desc->size.width, cap.minImageExtent.width, cap.maxImageExtent.width),
            std::clamp(desc->size.height, cap.minImageExtent.height, cap.maxImageExtent.height)
        },
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
    vr = dtable.vkCreateSwapchainKHR(device.get(), &swap_info, nullptr, swapchain.put(device.get(), dtable.vkDestroySwapchainKHR));

    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a swapchain">(vr);
        return out_swapchain;
    }

    // Create Command list
    VkCommandPoolCreateInfo cmd_pool_create_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queues.GetOfType(wis::QueueType::Graphics)->family_index,
    };
    wis::scoped_handle<VkCommandPool> cmd_pool;
    vr = dtable.vkCreateCommandPool(device.get(), &cmd_pool_create_info, nullptr, cmd_pool.put(device.get(), dtable.vkDestroyCommandPool));
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a command pool">(vr);
        return out_swapchain;
    }

    VkCommandBufferAllocateInfo cmd_buf_alloc_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = cmd_pool.get(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer cmd_buf;
    vr = dtable.vkAllocateCommandBuffers(device.get(), &cmd_buf_alloc_info, &cmd_buf);
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to allocate a command buffer">(vr);
        return out_swapchain;
    }

    // Create semaphores
    internal.render_completed_semaphore = wis::detail::make_unique_for_overwrite<VkSemaphore[]>(desc->buffer_count);
    if (!internal.render_completed_semaphore) {
        result = wis::make_result<FUNC, "failed to allocate render_completed_semaphore array">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_swapchain;
    }
    internal.image_ready_semaphores = wis::detail::make_unique_for_overwrite<VkSemaphore[]>(desc->buffer_count);
    if (!internal.image_ready_semaphores) {
        result = wis::make_result<FUNC, "failed to allocate image_ready_semaphores array">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_swapchain;
    }
    constexpr VkSemaphoreCreateInfo semaphore_info{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    for (uint32_t n = 0; n < desc->buffer_count; n++) {
        vr = dtable.vkCreateSemaphore(device.get(), &semaphore_info, nullptr, &internal.render_completed_semaphore[n]);
        if (!wis::succeeded(vr)) {
            for (uint32_t i = 0; i < n; i++) { // Cleanup
                dtable.vkDestroySemaphore(device.get(), internal.render_completed_semaphore[i], nullptr);
            }
            result = wis::make_result<FUNC, "vkCreateSemaphore failed for render_completed_semaphore">(vr);
            return out_swapchain;
        }

        vr = dtable.vkCreateSemaphore(device.get(), &semaphore_info, nullptr, &internal.image_ready_semaphores[n]);
        if (!wis::succeeded(vr)) {
            for (uint32_t i = 0; i < n; i++) { // Cleanup
                dtable.vkDestroySemaphore(device.get(), internal.render_completed_semaphore[i], nullptr);
                dtable.vkDestroySemaphore(device.get(), internal.image_ready_semaphores[i], nullptr);
            }
            result = wis::make_result<FUNC, "vkCreateSemaphore failed for image_ready_semaphore">(vr);
            return out_swapchain;
        }
    }

    // Create fences
    constexpr VkFenceCreateInfo fence_info{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    for (uint32_t i = 0; i < internal.fences.size(); i++) {
        vr = dtable.vkCreateFence(device.get(), &fence_info, nullptr, &internal.fences[i]);
        if (!wis::succeeded(vr)) {
            for (uint32_t i = 0; i < desc->buffer_count; i++) { // Cleanup (unless swapchain is set, the cleanup will not begin on internal)
                dtable.vkDestroySemaphore(device.get(), internal.render_completed_semaphore[i], nullptr);
                dtable.vkDestroySemaphore(device.get(), internal.image_ready_semaphores[i], nullptr);
            }
            for (uint32_t j = 0; j < i; j++) {
                dtable.vkDestroyFence(device.get(), internal.fences[j], nullptr);
            }
            result = wis::make_result<FUNC, "vkCreateFence failed for fences">(vr);
            return out_swapchain;
        }
    }

    internal.surface = std::move(surface);
    internal.device = device;
    internal.adapter = adapter.GetInternal().adapter;
    internal.swapchain = swapchain.release();
    internal.initialization = cmd_buf;
    internal.command_pool = cmd_pool.release();
    internal.format = *format;
    internal.present_mode = present_mode;
    internal.tearing = tearing;
    internal.stereo = stereo;
    internal.stereo_requested = desc->stereo;

    auto rres = internal.InitBackBuffers(swap_info.imageExtent);
    if (rres.status != wis::Status::Ok) {
        result = rres;
        out_swapchain = {};
        return out_swapchain;
    }

    rres = internal.AcquireNextIndex();
    if (rres.status != wis::Status::Ok) {
        result = rres;
        out_swapchain = {};
    }
    return out_swapchain;
}

wis::VKRenderTarget
wis::ImplVKDevice::CreateRenderTarget(wis::Result& result, wis::VKTextureView texture, wis::RenderTargetDesc desc) const noexcept
{
    VKRenderTarget out_render_target;
    auto& internal = out_render_target.GetMutableInternal();

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

    auto vr = device.table().vkCreateImageView(device.get(), &info, nullptr, internal.view.put(device, device.table().vkDestroyImageView));
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create an image view">(vr);
        return out_render_target;
    }

    internal.size = std::get<2>(texture);
    return out_render_target;
}

bool wis::ImplVKDevice::QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
{
    auto& features = ext1.GetInternal().features;

    switch (feature) {
    case wis::DeviceFeature::WaitForPresent:
        return features.present_wait;
    case wis::DeviceFeature::AdvancedIndexBuffer:
        return features.index_buffer_range;
    case wis::DeviceFeature::EnchancedBarriers:
        return features.synchronization_2;
    case wis::DeviceFeature::DynamicVSync:
        return features.dynamic_vsync;
    case wis::DeviceFeature::UnusedRenderTargets:
        return features.dynamic_render_unused_attachments;
    case wis::DeviceFeature::PushDescriptors:
        return features.push_descriptor;
    default:
        return false;
    }
}

//--------------------------------------------------------------------------------------------------

wis::VKSampler
wis::ImplVKDevice::CreateSampler(wis::Result& result, const wis::SamplerDesc& desc) const noexcept
{
    VKSampler out_sampler;
    auto& internal = out_sampler.GetMutableInternal();

    VkSamplerCustomBorderColorCreateInfoEXT custom_border_color{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT,
        .pNext = nullptr,
        .customBorderColor = {
            desc.border_color[0],
            desc.border_color[1],
            desc.border_color[2],
            desc.border_color[3],
        },
    };
    VkSamplerCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = &custom_border_color,
        .flags = 0,
        .magFilter = convert_vk(desc.mag_filter),
        .minFilter = convert_vk(desc.min_filter),
        .mipmapMode = VkSamplerMipmapMode(desc.mip_filter),
        .addressModeU = convert_vk(desc.address_u),
        .addressModeV = convert_vk(desc.address_v),
        .addressModeW = convert_vk(desc.address_w),
        .mipLodBias = desc.mip_lod_bias,
        .anisotropyEnable = desc.anisotropic,
        .maxAnisotropy = float(desc.max_anisotropy),
        .compareEnable = desc.comparison_op != wis::Compare::Never,
        .compareOp = convert_vk(desc.comparison_op),
        .minLod = desc.min_lod,
        .maxLod = desc.max_lod,
        .borderColor = VkBorderColor::VK_BORDER_COLOR_FLOAT_CUSTOM_EXT
    };

    auto vr = device.table().vkCreateSampler(device.get(), &info, nullptr, internal.sampler.put(device, device.table().vkDestroySampler));
    if (!succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create a sampler">(vr);
    }
    return out_sampler;
}

wis::VKShaderResource
wis::ImplVKDevice::CreateShaderResource(wis::Result& result, wis::VKTextureView texture, wis::ShaderResourceDesc desc) const noexcept
{
    VKShaderResource out_resource;
    auto& internal = out_resource.GetMutableInternal();

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

    auto res = device.table().vkCreateImageView(device.get(), &info, nullptr, internal.view.put(device, device.table().vkDestroyImageView));
    if (!succeeded(res)) {
        result = wis::make_result<FUNC, "Failed to create an image view">(res);
    }
    return out_resource;
}

wis::VKDescriptorStorage
wis::ImplVKDevice::CreateDescriptorStorage(wis::Result& result, const wis::DescriptorStorageDesc& desc) const noexcept
{
    VKDescriptorStorage out_storage;
    auto& internal = out_storage.GetMutableInternal();

    constexpr static uint32_t num_sets = Internal<VKDescriptorStorage>::max_sets;
    // Get max descriptor counts
    std::array<uint32_t, num_sets> desc_counts{
        desc.sampler_count,
        desc.cbuffer_count,
        desc.texture_count,
        desc.stexture_count,
        desc.sbuffer_count,
        desc.rbuffer_count
    };

    uint32_t iterator = 0;
    VkDescriptorPoolSize pool_sizes[num_sets] {};
    for (size_t i = 0; i < num_sets; i++) {
        if (desc_counts[i] != 0) {
            pool_sizes[iterator].type = DefaultLayout::desc_types[i];
            pool_sizes[iterator].descriptorCount = desc_counts[i];
            iterator++;
        }
    }

    VkDescriptorPoolCreateInfo pool_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .maxSets = num_sets,
        .poolSizeCount = iterator,
        .pPoolSizes = pool_sizes,
    };
    wis::scoped_handle<VkDescriptorPool> pool;
    auto res = device.table().vkCreateDescriptorPool(device.get(), &pool_info, nullptr, pool.put(device.get(), device.table().vkDestroyDescriptorPool));
    if (!succeeded(res)) {
        result = wis::make_result<FUNC, "Failed to create a descriptor pool">(res);
        return out_storage;
    }

    VkDescriptorSetVariableDescriptorCountAllocateInfo variable_desc_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorSetCount = num_sets,
        .pDescriptorCounts = desc_counts.data(),
    };

    VkDescriptorSetAllocateInfo desc_alloc_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &variable_desc_info,
        .descriptorPool = pool.get(),
        .descriptorSetCount = uint32_t(std::size(internal.set)),
        .pSetLayouts = default_layout.desc_sets.data(),
    };
    res = device.table().vkAllocateDescriptorSets(device.get(), &desc_alloc_info, internal.set.data());
    if (!succeeded(res)) {
        result = wis::make_result<FUNC, "Failed to allocate descriptor sets">(res);
        return out_storage;
    }
    internal.pool = pool.release();
    internal.device = device;
    return out_storage;
}

wis::VKRootSignature
wis::ImplVKDevice::CreateRootSignature(wis::Result& result, const wis::PushConstant* constants,
                                       uint32_t constants_size,
                                       const PushDescriptor* push_descriptors,
                                       uint32_t push_descriptors_size,
                                       uint32_t space_overlap_count) const noexcept
{
    VKRootSignature out_signature;
    auto& internal = out_signature.GetMutableInternal();

    constexpr static uint32_t num_sets = uint32_t(wis::BindingIndex::Count);
    if (constants_size > wis::max_push_constants) {
        result = wis::make_result<FUNC, "constants_size exceeds max_push_constants">(VkResult::VK_ERROR_UNKNOWN);
        return out_signature;
    }
    if (push_descriptors_size > wis::max_push_descriptors) {
        result = wis::make_result<FUNC, "push_descriptors_size exceeds max_push_descriptors">(VkResult::VK_ERROR_UNKNOWN);
        return out_signature;
    }
    if (space_overlap_count > wis::max_descriptor_space_overlap) {
        result = wis::make_result<FUNC, "space_overlap_count exceeds max_descriptor_space_overlap">(VkResult::VK_ERROR_UNKNOWN);
        return out_signature;
    }

    if (internal.vk_dsls = wis::detail::make_unique_for_overwrite<VkDescriptorSetLayout[]>(num_sets + 1); !internal.vk_dsls) {
        result = wis::make_result<FUNC, "Failed to allocate descriptor set layout array">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_signature;
    }

    // Create push descriptor set layout
    {
        VkDescriptorSetLayoutBinding push_bindings[wis::max_push_descriptors] {};
        for (uint32_t i = 0; i < push_descriptors_size; i++) {
            auto& r = push_descriptors[i];
            auto& b = push_bindings[i];
            b.binding = i;
            b.descriptorType = convert_vk(r.type);
            b.descriptorCount = 1; // Push descriptors are always single
            b.stageFlags = convert_vk(r.stage);
        }
        VkDescriptorSetLayoutCreateInfo push_desc_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR,
            .bindingCount = push_descriptors_size,
            .pBindings = push_bindings,
        };
        auto res = device.table().vkCreateDescriptorSetLayout(device.get(), &push_desc_info, nullptr, &internal.vk_dsls[0]);
        if (!succeeded(res)) {
            result = wis::make_result<FUNC, "Failed to create a push descriptor set layout">(res);
            return out_signature;
        }
        std::copy_n(default_layout.desc_sets.begin(), num_sets, internal.vk_dsls.get() + 1);
    }

    VkPushConstantRange push_constants[wis::max_push_constants] {};
    for (uint32_t i = 0; i < constants_size; i++) {
        auto& c = push_constants[i];
        auto& r = constants[i];
        c.stageFlags = convert_vk(r.stage);
        c.offset = 0;
        c.size = r.size_bytes;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = num_sets + 1,
        .pSetLayouts = internal.vk_dsls.get(),
        .pushConstantRangeCount = constants_size,
        .pPushConstantRanges = push_constants,
    };
    auto vr = device.table().vkCreatePipelineLayout(device.get(), &pipeline_layout_info, nullptr, internal.root.put(device, device.table().vkDestroyPipelineLayout));
    if (!succeeded(vr)) {
        device.table().vkDestroyDescriptorSetLayout(device.get(), internal.vk_dsls[0], nullptr);
        result = wis::make_result<FUNC, "Failed to create a pipeline layout">(vr);
    }
    internal.dsl_count = 1; // number of descriptor set layouts to destroy
    return out_signature;
}

wis::VKRootSignature
wis::ImplVKDevice::CreateRootSignature2(wis::Result& result, const wis::PushConstant* push_constants,
                                        uint32_t constants_count,
                                        const wis::PushDescriptor* push_descriptors,
                                        uint32_t push_descriptors_count,
                                        const wis::DescriptorSpacing* descriptor_spacing) const noexcept
{
    return CreateRootSignature(result, push_constants, constants_count, push_descriptors, push_descriptors_count, 1);
}
#endif
