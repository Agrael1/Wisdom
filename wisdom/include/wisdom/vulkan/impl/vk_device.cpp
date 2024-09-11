#ifndef VK_DEVICE_CPP
#define VK_DEVICE_CPP
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

inline static wis::ResultValue<std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>>>
GetAvailableExtensions(VkPhysicalDevice adapter, const wis::VKMainInstance& itable)
{
    std::unordered_map<std::string, VkExtensionProperties, wis::string_hash, std::equal_to<>> ext_map;

    uint32_t count = 0;
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, nullptr);
    wis::detail::fixed_allocation<VkExtensionProperties> ext_props = wis::detail::make_fixed_allocation<VkExtensionProperties>(count);
    if (!ext_props)
        return wis::make_result<FUNC, "Failed to allocate memory for extension properties">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);
    itable.vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, ext_props.get());

    ext_map.reserve(count);
    for (auto& ext : ext_props)
        ext_map.emplace(ext.extensionName, ext);

    return ext_map;
}

wis::ResultValue<wis::VKDevice> wis::VKCreateDevice(wis::VKAdapter adapter) noexcept
{
    return VKCreateDeviceWithExtensions(std::move(adapter), nullptr, 0);
}

wis::ResultValue<wis::VKDevice>
wis::VKCreateDeviceWithExtensions(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size) noexcept
{
    return VKCreateDeviceWithExtensionsForce(std::move(in_adapter), exts, ext_size, false);
}

wis::ResultValue<wis::VKDevice>
wis::VKCreateDeviceWithExtensionsForce(wis::VKAdapter in_adapter, wis::VKDeviceExtension** exts, uint32_t ext_size, bool force) noexcept
{
    auto& adapter_i = in_adapter.GetInternal();
    auto hadapter = adapter_i.adapter;
    auto& itable = adapter_i.instance.table();
    auto& gtable = adapter_i.instance.gtable();

    std::span<wis::VKDeviceExtension*> exts_span{ exts, exts + ext_size };
    std::unordered_map<VkStructureType, uintptr_t> struct_map;
    std::unordered_map<VkStructureType, uintptr_t> property_map;
    std::unordered_set<std::string_view> ext_name_set;
    auto [res, available_exts] = GetAvailableExtensions(hadapter, itable);
    if (res.status != wis::Status::Ok)
        return res;

    for (auto*& ext : exts_span) {
        bool supported = ext->GetExtensionInfo(available_exts, ext_name_set, struct_map, property_map);
        if (!supported) {
            ext = nullptr;
            continue;
        }
    }

    // Ext1
    wis::VKDeviceExtensionEmbedded1 ext1;
    if (!ext1.GetExtensionInfo(available_exts, ext_name_set, struct_map, property_map))
        return wis::make_result<FUNC, "Failed to get base extensions to run device">(VkResult::VK_ERROR_UNKNOWN);

    // Allocate memory for extension names
    auto ext_names = wis::detail::make_fixed_allocation<const char*>(ext_name_set.size());
    if (!ext_names)
        return wis::make_result<FUNC, "Failed to allocate memory for extension names">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

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
    if (!allocation)
        return wis::make_result<FUNC, "Failed to allocate memory for feature structures">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

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
    if (!allocation_props)
        return wis::make_result<FUNC, "Failed to allocate memory for property structures">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

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

    auto queues = GetQueueFamilies(hadapter, itable);

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
    if (!wis::succeeded(vres))
        return wis::make_result<FUNC, "vkCreateDevice failed to create device">(vres);

    wis::managed_handle<VkDevice> managed_device{ device, (PFN_vkDestroyDevice)gtable.vkGetDeviceProcAddr(device, "vkDestroyDevice") };
    std::unique_ptr<VKMainDevice> device_table = wis::detail::make_unique<VKMainDevice>();
    if (!device_table)
        return wis::make_result<FUNC, "Failed to allocate device table">(VkResult::VK_ERROR_OUT_OF_HOST_MEMORY);

    if (!device_table->Init(device, gtable.vkGetDeviceProcAddr))
        return wis::make_result<FUNC, "Failed to initialize device table">(VkResult::VK_ERROR_UNKNOWN);

    wis::VKDevice vkdevice{ wis::SharedDevice{ managed_device.release(), std::move(device_table), &gtable },
                            std::move(in_adapter), std::move(ext1) };

    // Init embedded extensions
    vkdevice.ext1.Init(vkdevice, struct_map, property_map);
    if (!vkdevice.ext1.Supported() && !force)
        return wis::make_result<FUNC, "The system does not support the required extensions">(VkResult::VK_ERROR_UNKNOWN);

    // Init the rest of the extensions
    for (auto*& ext : exts_span) {
        if (ext == nullptr)
            continue;
        ext->Init(vkdevice, struct_map, property_map);
    }

    // Create Default Allocator
    auto [res1, allocator] = vkdevice.VKCreateAllocator();
    if (res1.status != wis::Status::Ok)
        return res1;

    vkdevice.allocator = std::move(allocator);

    return { wis::success, std::move(vkdevice) };
}

wis::VKDevice::VKDevice(wis::SharedDevice in_device, wis::VKAdapter in_adapter,
                        wis::VKDeviceExtensionEmbedded1 ext1) noexcept
    : QueryInternal(std::move(in_adapter), std::move(in_device), std::move(ext1))
{
    queues = GetQueueFamilies(adapter.GetInternal().adapter, GetInstanceTable());
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
    constexpr static VkExportSemaphoreCreateInfo export_info{
        .sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO,
        .handleTypes = detail::semaphore_handle_type
    };

    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext = ext1.GetFeatures().interop_device ? &export_info : nullptr,
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
wis::VKDevice::CreateCommandQueue(wis::QueueType type) const noexcept
{
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
    if (ia_count > ext1.GetInternal().base_properties.max_ia_attributes)
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
        .pVertexBindingDescriptions = desc->input_layout.slot_count ? reinterpret_cast<const VkVertexInputBindingDescription*>(desc->input_layout.slots) : nullptr,
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

    if (ia_count)
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
            VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;

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
    wis::shared_handle<VmaAllocator> interop;
    if (ext1.GetFeatures().interop_device) {
        auto [result, hallocator] = VKCreateAllocator(true);
        if (result.status != wis::Status::Ok)
            return result;

        interop = std::move(hallocator);
    }

    return VKResourceAllocator{ allocator, std::move(interop) };
}

wis::ResultValue<wis::shared_handle<VmaAllocator>>
wis::VKDevice::VKCreateAllocator(bool interop) const noexcept
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
        allocator_functions.vkGetMemoryWin32HandleKHR = GetDeviceProcAddr<void*>("vkGetMemoryWin32HandleKHR");
#endif // WIN32
    }

    VmaAllocator al;
    VkResult vr = vmaCreateAllocator(&allocatorInfo, &al);

    if (!succeeded(vr))
        return wis::make_result<FUNC, "Failed to create an Allocator">(vr);

    return wis::shared_handle<VmaAllocator>{ device, al };
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
    wis::detail::fixed_allocation<VkSurfaceFormatKHR> surface_formats = wis::detail::make_fixed_allocation<VkSurfaceFormatKHR>(format_count);
    auto result = itable.vkGetPhysicalDeviceSurfaceFormatsKHR(hadapter, surface.get(),
                                                              &format_count, surface_formats.get());

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

    result = itable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(hadapter, surface.get(), &cap);
    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to get surface capabilities">(result);

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
    if (!desc->vsync) {
        if (desc->tearing) {
            if (std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR) > 0)
                present_mode = VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;
            else if (std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR) > 0)
                present_mode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        } else if (std::ranges::count(modes, VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR) > 0 && !stereo) {
            present_mode = VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    constexpr static uint32_t present_modes_count = 8;
    std::array<VkPresentModeKHR, present_modes_count> compatible_modes{};
    uint8_t supported_presentation = 0;
    uint32_t compatible_modes_count = 0;

    // Check if the extension is supported
    if (ext1.GetFeatures().dynamic_vsync) {
        compatible_modes_count = [&]() {
            VkSurfacePresentModeCompatibilityEXT present_mode_compat{
                .sType = VK_STRUCTURE_TYPE_SURFACE_PRESENT_MODE_COMPATIBILITY_EXT,
                .pNext = nullptr,
                .pPresentModes = nullptr,
            };
            VkSurfaceCapabilities2KHR cap2{
                .sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR,
                .pNext = &present_mode_compat,
            };
            VkSurfacePresentModeEXT xpresent_mode{
                .sType = VK_STRUCTURE_TYPE_SURFACE_PRESENT_MODE_EXT,
                .presentMode = present_mode,
            };
            VkPhysicalDeviceSurfaceInfo2KHR surface_info{
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                .pNext = &xpresent_mode,
                .surface = surface.get(),
            };
            itable.vkGetPhysicalDeviceSurfaceCapabilities2KHR(hadapter, &surface_info, &cap2);
            present_mode_compat.pPresentModes = compatible_modes.data();
            itable.vkGetPhysicalDeviceSurfaceCapabilities2KHR(hadapter, &surface_info, &cap2);
            return present_mode_compat.presentModeCount;
        }();

        for (size_t i = 0; i < compatible_modes_count; i++)
            if (modes[i] < 8)
                supported_presentation |= 1 << compatible_modes[i];

        // Transfer back to the original present mode
        supported_presentation |= 1 << present_mode;
    }

    VkSwapchainPresentModesCreateInfoEXT present_modes{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_MODES_CREATE_INFO_EXT,
        .pNext = nullptr,
        .presentModeCount = compatible_modes_count,
        .pPresentModes = compatible_modes.data(),
    };

    VkSwapchainCreateInfoKHR swap_info{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = ext1.GetFeatures().dynamic_vsync ? &present_modes : nullptr,
        .flags = 0,
        .surface = surface.get(),
        .minImageCount = desc->buffer_count,
        .imageFormat = convert_vk(desc->format),
        .imageColorSpace = format->colorSpace,
        .imageExtent = {
                std::clamp(desc->size.width, cap.minImageExtent.width, cap.maxImageExtent.width),
                std::clamp(desc->size.height, cap.minImageExtent.height, cap.maxImageExtent.height) },
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

    wis::detail::VKSwapChainCreateInfo sci{ std::move(surface),
                                            device,
                                            adapter.GetInternal().adapter,
                                            itable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
                                            swapchain.release(),
                                            cmd_buf,
                                            cmd_pool.release(),
                                            qpresent_queue,
                                            graphics_queue,
                                            *format,
                                            present_mode,
                                            ext1.GetFeatures().dynamic_vsync ? supported_presentation : uint8_t(0),
                                            desc->tearing,
                                            stereo,
                                            desc->stereo };

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
wis::VKDevice::CreateDescriptorBuffer(wis::DescriptorHeapType heap_type, wis::DescriptorMemory memory_type, uint64_t memory_bytes) const noexcept
{
    auto& ext1_i = ext1.GetInternal();
    uint32_t descriptor_size = heap_type == wis::DescriptorHeapType::Descriptor
            ? ext1_i.features.mutable_descriptor ? ext1_i.descriptor_buffer_features.mutable_descriptor_size : 0u
            : ext1_i.descriptor_buffer_features.sampler_size;

    VkBufferUsageFlags usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
            (heap_type == wis::DescriptorHeapType::Descriptor
                     ? VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT
                     : VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT);

    VkBufferCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = memory_bytes,
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

    return VKDescriptorBuffer{ allocator, buffer, allocation, heap_type, ext1_i.descriptor_buffer_features, uint32_t(descriptor_size) };
}

bool wis::VKDevice::QueryFeatureSupport(wis::DeviceFeature feature) const noexcept
{
    auto& features = ext1.GetInternal().features;

    switch (feature) {
    case wis::DeviceFeature::DescriptorBuffer:
        return features.descriptor_buffer;
    case wis::DeviceFeature::WaitForPresent:
        return features.present_wait;
    case wis::DeviceFeature::DescriptorEqualSize:
        return features.mutable_descriptor;
    case wis::DeviceFeature::AdvancedIndexBuffer:
        return features.index_buffer_range;
    case wis::DeviceFeature::EnchancedBarriers:
        return features.synchronization_2;
    case wis::DeviceFeature::DynamicVSync:
        return features.dynamic_vsync;
    default:
        return false;
    }
}

//--------------------------------------------------------------------------------------------------

wis::ResultValue<VkDescriptorSetLayout>
wis::VKDevice::CreateDummyDescriptorSetLayout(const VkDescriptorSetLayoutBinding& binding) const noexcept
{
    bool has_mutable = ext1.GetInternal().features.mutable_descriptor;
    constexpr static VkDescriptorType cbvSrvUavTypes[] = {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR /* Need to check support if this is desired. */
    };

    constexpr static VkMutableDescriptorTypeListVALVE bindings_mutable{
        .descriptorTypeCount = 1,
        .pDescriptorTypes = cbvSrvUavTypes
    };

    constexpr static VkMutableDescriptorTypeCreateInfoEXT mutable_type_info{
        .sType = VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_EXT,
        .pNext = nullptr,
        .mutableDescriptorTypeListCount = 1,
        .pMutableDescriptorTypeLists = &bindings_mutable,
    };

    VkDescriptorSetLayoutCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = has_mutable && binding.descriptorType != VK_DESCRIPTOR_TYPE_SAMPLER ? &mutable_type_info : nullptr,
        .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
        .bindingCount = 1,
        .pBindings = &binding,
    };

    VkDescriptorSetLayout layout;
    auto result = device.table().vkCreateDescriptorSetLayout(device.get(), &desc, nullptr, &layout);

    if (!succeeded(result))
        return wis::make_result<FUNC, "Failed to create a descriptor set layout">(result);

    return layout;
}

wis::ResultValue<VkDescriptorSetLayout>
wis::VKDevice::CreateDescriptorSetDescriptorLayout(const wis::DescriptorTable* table) const noexcept
{
    bool has_mutable = ext1.GetInternal().features.mutable_descriptor;
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
    wis::detail::limited_allocator<VkMutableDescriptorTypeListVALVE, 32> bindings_mutable{ has_mutable ? table->entry_count : 0, true };

    if (has_mutable)
        for (size_t i = 0; i < table->entry_count; i++)
            bindings_mutable.data()[i] = a;

    for (size_t i = 0; i < table->entry_count; i++) {
        auto& entry = table->entries[i];
        bindings.data()[i] = {
            .binding = entry.binding,
            .descriptorType = has_mutable ? VK_DESCRIPTOR_TYPE_MUTABLE_EXT : convert_vk(entry.type),
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
        .pNext = has_mutable ? &mutableTypeInfo : nullptr,
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
#endif // WISIMPL_VULKAN
