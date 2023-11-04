#pragma once
#ifndef WISDOM_HEADER_ONLY
#include <wisdom/vulkan/xvk_device.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/util/flags.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <wisdom/util/misc.h>
#include <wisdom/vulkan/xvk_factory.h>
#include <cassert>
#include <unordered_set>

constexpr inline std::array required_extensions{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, // for Swapchain
    VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME, // for Fence

    VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME, // for Allocator
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, // for Allocator
    VK_KHR_BIND_MEMORY_2_EXTENSION_NAME, // for Allocator
    VK_KHR_MAINTENANCE_4_EXTENSION_NAME, // for Allocator

    VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME,

    // VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    // VK_KHR_RAY_QUERY_EXTENSION_NAME,
    // VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    // VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
    // VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    // VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
    // VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
    // VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
    // VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME,
    // VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
    // VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
    // VK_NV_MESH_SHADER_EXTENSION_NAME,
    // VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
    // VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
    // VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
};

auto RequestExtensions(wis::VKAdapterHandle adapter_hnd) noexcept
{
    auto* adapter = std::get<0>(adapter_hnd);
    auto* itbl = std::get<1>(adapter_hnd);

    std::vector<VkExtensionProperties> ext_props;
    uint32_t count = 0;
    itbl->vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, nullptr);
    ext_props.resize(count);
    itbl->vkEnumerateDeviceExtensionProperties(adapter, nullptr, &count, ext_props.data());

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

wis::detail::QueueResidency GetQueueFamilies(wis::VKAdapterHandle adapter_hnd) noexcept
{
    using namespace river::flags;
    using namespace wis::detail;

    auto* adapter = std::get<0>(adapter_hnd);
    auto* itbl = std::get<1>(adapter_hnd);

    std::vector<VkQueueFamilyProperties> family_props;
    VkResult result = VK_SUCCESS;
    uint32_t count = 0;
    itbl->vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, nullptr);
    family_props.resize(count);
    itbl->vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, family_props.data());

    wis::lib_info(wis::format("The system supports {} queue families", family_props.size()));
    assert(family_props.size() < 256);

    QueueResidency queues;

    // NOLINTNEXTLINE
    for (uint8_t i = 0; i < family_props.size(); i++) {
        using enum VkQueueFlagBits;
        auto& family = family_props[i];
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT &&
            queues.available_queues[+QueueTypes::Graphics].Empty()) {
            queues.available_queues[+QueueTypes::Graphics] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Graphics queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) == VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT &&
            queues.available_queues[+QueueTypes::Compute].Empty()) {
            queues.available_queues[+QueueTypes::Compute] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Compute queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR) == VkQueueFlagBits::VK_QUEUE_VIDEO_DECODE_BIT_KHR &&
            queues.available_queues[+QueueTypes::VideoDecode].Empty()) {
            queues.available_queues[+QueueTypes::VideoDecode] = {
                uint16_t(uint32_t(family.queueFlags)),
                uint8_t(family.queueCount),
                i,
            };
            wis::lib_info(wis::format("\tDedicated Video Decode queues: {}", family.queueCount));
            continue;
        }
        if ((family.queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) == VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT &&
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

wis::VKDevice::VKDevice(wis::shared_handle<VkInstance> instance,
                        wis::SharedDevice device,
                        wis::VKAdapterHandle adapter,
                        wis::DeviceFeatures features,
                        InternalFeatures ifeatures) noexcept
    : QueryInternal(std::move(instance), std::move(device), adapter, ifeatures), features(features)
{
    queues = GetQueueFamilies(adapter);

    VkPhysicalDeviceProperties2 props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = nullptr,
        .properties = {},
    };
    VkPhysicalDeviceDescriptorBufferPropertiesEXT dbp{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT,
        .pNext = nullptr,
    };
    if (ifeatures.has_descriptor_buffer) {
        props.pNext = &dbp;
    }
    std::get<1>(adapter)->vkGetPhysicalDeviceProperties2(std::get<0>(adapter), &props);

    if (ifeatures.has_descriptor_buffer) {
        ifeatures.push_descriptor_bufferless = dbp.bufferlessPushDescriptors;
    }
    ifeatures.max_ia_attributes = props.properties.limits.maxVertexInputAttributes;

    auto& gt = wis::Internal<VKFactory>::global_table;
    auto& it = *GetInstanceTable();
    auto& dt = *device.table();

    allocator_functions = std::shared_ptr<VmaVulkanFunctions>{ new VmaVulkanFunctions{
            .vkGetInstanceProcAddr = gt.vkGetInstanceProcAddr,
            .vkGetDeviceProcAddr = gt.vkGetDeviceProcAddr,
            .vkGetPhysicalDeviceProperties = it.vkGetPhysicalDeviceProperties,
            .vkGetPhysicalDeviceMemoryProperties = it.vkGetPhysicalDeviceMemoryProperties,
            .vkAllocateMemory = dt.vkAllocateMemory,
            .vkFreeMemory = dt.vkFreeMemory,
            .vkMapMemory = dt.vkMapMemory,
            .vkUnmapMemory = dt.vkUnmapMemory,
            .vkFlushMappedMemoryRanges = dt.vkFlushMappedMemoryRanges,
            .vkInvalidateMappedMemoryRanges = dt.vkInvalidateMappedMemoryRanges,
            .vkBindBufferMemory = dt.vkBindBufferMemory,
            .vkBindImageMemory = dt.vkBindImageMemory,
            .vkGetBufferMemoryRequirements = dt.vkGetBufferMemoryRequirements,
            .vkGetImageMemoryRequirements = dt.vkGetImageMemoryRequirements,
            .vkCreateBuffer = dt.vkCreateBuffer,
            .vkDestroyBuffer = dt.vkDestroyBuffer,
            .vkCreateImage = dt.vkCreateImage,
            .vkDestroyImage = dt.vkDestroyImage,
            .vkCmdCopyBuffer = dt.vkCmdCopyBuffer,
            .vkGetBufferMemoryRequirements2KHR = dt.vkGetBufferMemoryRequirements2,
            .vkGetImageMemoryRequirements2KHR = dt.vkGetImageMemoryRequirements2,
            .vkBindBufferMemory2KHR = dt.vkBindBufferMemory2,
            .vkBindImageMemory2KHR = dt.vkBindImageMemory2,
            .vkGetPhysicalDeviceMemoryProperties2KHR = it.vkGetPhysicalDeviceMemoryProperties2,
            .vkGetDeviceBufferMemoryRequirements = dt.vkGetDeviceBufferMemoryRequirements,
            .vkGetDeviceImageMemoryRequirements = dt.vkGetDeviceImageMemoryRequirements,
    } };
}

std::pair<wis::Result, wis::VKDevice>
wis::VKCreateDevice(wis::VKFactoryHandle factory, wis::VKAdapterHandle adapter) noexcept
{
    constexpr static auto max_queue_count = +wis::detail::QueueTypes::Count;
    wis::detail::uniform_allocator<VkDeviceQueueCreateInfo, max_queue_count> queue_infos{};
    auto queues = GetQueueFamilies(adapter);
    auto* xadapter = std::get<0>(adapter);
    auto* itbl = std::get<1>(factory);

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

    auto present_exts = RequestExtensions(adapter);
    if (!present_exts.contains(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME))
        return { wis::make_result<FUNC, "The system does not support timeline semaphores.">(VkResult::VK_ERROR_UNKNOWN), wis::VKDevice{} };

    // Loading features
    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = nullptr,
        .features = {},
    };

    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timeline_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
        .pNext = nullptr,
        .timelineSemaphore = true,
    };
    features.pNext = &timeline_features;

    VkPhysicalDeviceDescriptorBufferFeaturesEXT descbuffer_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT,
        .pNext = nullptr,
    };
    if (present_exts.contains(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME))
        timeline_features.pNext = &descbuffer_features;

    itbl->vkGetPhysicalDeviceFeatures2(xadapter, &features);

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
    };
    DeviceFeatures dfeatures = DeviceFeatures(+DeviceFeatures::PushDescriptors & int(ifeatures.has_descriptor_buffer));

    VkDevice device{};
    VkResult result = VK_SUCCESS;
    if (!wis::succeeded(result = itbl->vkCreateDevice(xadapter, &device_info, nullptr, &device)))
        return { wis::make_result<FUNC, "vkCreateDevice failed to create device">(result), wis::VKDevice{} };

    std::unique_ptr<VkDeviceTable> device_table = std::make_unique<VkDeviceTable>();
    device_table->Init(device, Internal<VKFactory>::global_table);
    wis::VKDevice vkdevice{
        std::move(std::get<0>(factory)), wis::SharedDevice{ device, std::move(device_table) }, std::move(adapter), dfeatures, ifeatures
    };

    return { wis::success, std::move(vkdevice) };
}

wis::Result
wis::VKDevice::WaitForMultipleFences(const VKFenceView* fences,
                                     const uint64_t* values,
                                     uint32_t count,
                                     MutiWaitFlags wait_all,
                                     uint64_t timeout) const noexcept
{
    VkSemaphoreWaitInfo waitInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .pNext = nullptr,
        .flags = VkSemaphoreWaitFlags(wait_all),
        .semaphoreCount = static_cast<uint32_t>(count),
        .pSemaphores = reinterpret_cast<const VkSemaphore*>(fences),
        .pValues = values
    };
    VkResult result = device.table()->vkWaitSemaphores(device.get(), &waitInfo, timeout);

    return succeeded(result)
            ? wis::success
            : wis::make_result<FUNC, "vkWaitSemaphores failed to wait for fences.">(result);
}

std::pair<wis::Result, wis::VKFence>
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
    VkResult result = VK_SUCCESS;

    return wis::succeeded(result = device.table()->vkCreateSemaphore(device.get(), &desc, nullptr, &sem))
            ? std::pair{ wis::success, VKFence{ { sem, device, device.table()->vkDestroySemaphore } } }
            : std::pair{ wis::make_result<FUNC, "vkCreateSemaphore failed to create semaphore">(result), VKFence{} };
}

std::pair<wis::Result, wis::VKResourceAllocator>
wis::VKDevice::CreateAllocator() const noexcept
{
    auto [result, allocator] = CreateAllocatorI();
    return result.status == wis::Status::Ok
            ? std::pair{ wis::success, VKResourceAllocator{ wis::managed_handle_ex<VmaAllocator>{ device, allocator }, allocator_functions } }
            : std::pair{ result, VKResourceAllocator{} };
}

std::pair<wis::Result, wis::VKRootSignature>
wis::VKDevice::CreateRootSignature(RootConstant* constants, uint32_t constants_size) const noexcept
{
    wis::detail::limited_allocator<VkPushConstantRange, 8> vk_constants{ constants_size };

    for (uint32_t i = 0; i < constants_size; i++) {
        auto& c = vk_constants.data()[i];
        auto& r = constants[i];
        c.stageFlags = convert_vk(r.stage);
        c.offset = 0;
        c.size = r.size_bytes;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = constants_size,
        .pPushConstantRanges = vk_constants.data(),
    };
    VkPipelineLayout layout;
    auto vr = device.table()->vkCreatePipelineLayout(device.get(), &pipeline_layout_info, nullptr, &layout);

    return !succeeded(vr)
            ? std::pair{ wis::make_result<FUNC, "Failed to create a pipeline layout">(vr), VKRootSignature{} }
            : std::pair{ wis::success, VKRootSignature{ wis::managed_handle_ex<VkPipelineLayout>{ layout, device, device.table()->vkDestroyPipelineLayout } } };
}

std::pair<wis::Result, VkDescriptorSetLayout>
wis::VKDevice::CreatePushDescriptorLayout(wis::PushDescriptor desc) const noexcept
{
    VkDescriptorSetLayoutBinding binding{
        .binding = desc.bind_register,
        .descriptorType = convert_vk(desc.type),
        .descriptorCount = 1,
        .stageFlags = VkShaderStageFlags(convert_vk(desc.stage)),
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo dsl{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = VkDescriptorSetLayoutCreateFlags(VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT * int(ifeatures.has_descriptor_buffer) |
                                                  VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR),
        .bindingCount = 1,
        .pBindings = &binding,
    };

    VkDescriptorSetLayout layout;
    auto vr = device.table()->vkCreateDescriptorSetLayout(device.get(), &dsl, nullptr, &layout);

    return succeeded(vr)
            ? std::pair{ wis::success, layout }
            : std::pair{ wis::make_result<FUNC, "Failed to create a descriptor set layout">(vr), VkDescriptorSetLayout{} };
}

std::pair<wis::Result, VmaAllocator>
wis::VKDevice::CreateAllocatorI() const noexcept
{
    uint32_t version = 0;
    auto& gt = wis::Internal<VKFactory>::global_table;
    auto& it = *GetInstanceTable();
    auto& dt = *device.table();
    gt.vkEnumerateInstanceVersion(&version);

    VmaAllocatorCreateInfo allocatorInfo{
        .flags = 0,
        .physicalDevice = std::get<0>(adapter),
        .device = device.get(),
        .pVulkanFunctions = allocator_functions.get(),
        .instance = instance.get(),
        .vulkanApiVersion = version,
    };

    VmaAllocator al;
    VkResult vr;
    return wis::succeeded(vr = vmaCreateAllocator(&allocatorInfo, &al))
            ? std::make_pair(wis::success, al)
            : std::make_pair(wis::make_result<FUNC, "Failed to create an Allocator">(vr), VmaAllocator{});
}

std::pair<wis::Result, wis::VKCommandQueue>
wis::VKDevice::CreateCommandQueue(wis::QueueType type, wis::QueuePriority priority) const noexcept
{
    (void)priority; // TODO: use priority

    const auto* queue = queues.GetOfType(type);
    if (queue == nullptr)
        return { wis::make_result<FUNC, "The system does not support the requested queue type">(VkResult::VK_ERROR_UNKNOWN), wis::VKCommandQueue{} };

    VkDeviceQueueInfo2 info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = queue->family_index,
        .queueIndex = queue->GetNextInLine(),
    };
    VkQueue queue_handle;
    device.table()->vkGetDeviceQueue2(device.get(), &info, &queue_handle);
    return { wis::success, wis::VKCommandQueue{ device, VkQueue{ queue_handle } } };
}

std::pair<wis::Result, wis::VKShader>
wis::VKDevice::CreateShader(void* bytecode, uint32_t size) const noexcept
{
    VkShaderModuleCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = size / sizeof(uint32_t),
        .pCode = reinterpret_cast<const uint32_t*>(bytecode),
    };
    VkShaderModule shader;
    auto vr = device.table()->vkCreateShaderModule(device.get(), &desc, nullptr, &shader);

    return succeeded(vr)
            ? std::pair{ wis::success, wis::VKShader{ wis::managed_handle_ex<VkShaderModule>{ shader, device, device.table()->vkDestroyShaderModule } } }
            : std::pair{ wis::make_result<FUNC, "Failed to create a shader module">(vr), wis::VKShader{} };
}

namespace wis::detail {
inline void VKFillShaderStage(wis::detail::uniform_allocator<VkPipelineShaderStageCreateInfo, wis::max_shader_stages>& shader_stages,
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

std::pair<wis::Result, wis::VKPipelineState>
wis::VKDevice::CreateGraphicsPipeline(const wis::VKGraphicsPipelineDesc* desc) const noexcept
{
    wis::detail::uniform_allocator<VkPipelineShaderStageCreateInfo, max_shader_stages> shader_stages;
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.vertex, VK_SHADER_STAGE_VERTEX_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.pixel, VK_SHADER_STAGE_FRAGMENT_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.geometry, VK_SHADER_STAGE_GEOMETRY_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.hull, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
    wis::detail::VKFillShaderStage(shader_stages, desc->shaders.domain, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);

    uint32_t ia_count = desc->input_layout.attribute_count;
    if (ia_count > ifeatures.max_ia_attributes)
        return { wis::make_result<FUNC, "The system does not support the requested number of vertex attributes">(VkResult::VK_ERROR_UNKNOWN), wis::VKPipelineState{} };

    wis::detail::limited_allocator<VkVertexInputAttributeDescription, wis::max_vertex_bindings> attributes{ ia_count };

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
        .pVertexBindingDescriptions = reinterpret_cast<const VkVertexInputBindingDescription*>(desc->input_layout.slots),
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
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = true,
    };

    // vk::PipelineColorBlendAttachmentState color_blend_attachment[2]{
    //     // 1 for now, TODO: proper blending
    //     vk::PipelineColorBlendAttachmentState{ false, // disabled
    //                                            vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
    //                                            vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
    //                                            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA },
    //     vk::PipelineColorBlendAttachmentState{ false, // disabled
    //                                            vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
    //                                            vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
    //                                            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA },

    //};
    // VkPipelineColorBlendStateCreateInfo color_blending{
    //    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    //    .pNext = nullptr,
    //    .flags = 0,
    //
    //    vk::PipelineColorBlendStateCreateFlags{},
    //    false,
    //    vk::LogicOp::eCopy,
    //    2,
    //    color_blend_attachment,
    //    { 0.0f, 0.0f, 0.0f, 0.0f }
    //};

    constexpr VkPipelineMultisampleStateCreateInfo default_multisampling{
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

    constexpr VkPipelineDepthStencilStateCreateInfo default_depth_stencil{
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
            .front = VkStencilOpState{
                    .failOp = convert_vk(ds.stencil_front.fail_op),
                    .passOp = convert_vk(ds.stencil_front.pass_op),
                    .depthFailOp = convert_vk(ds.stencil_front.depth_fail_op),
                    .compareOp = convert_vk(ds.stencil_front.comparison),
                    .compareMask = ds.stencil_front.read_mask,
                    .writeMask = ds.stencil_front.write_mask,
                    .reference = 0,
            },
            .back = VkStencilOpState{
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

    static constexpr size_t max_dynstates = 4;
    wis::detail::uniform_allocator<VkDynamicState, max_dynstates> dynamic_state_enables;
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_SCISSOR);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);
    dynamic_state_enables.allocate(VkDynamicState::VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE);

    VkPipelineDynamicStateCreateInfo dynamic_state{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = uint32_t(dynamic_state_enables.size()),
        .pDynamicStates = dynamic_state_enables.data()
    };

    VkPipelineCreateFlags flags = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT * int(ifeatures.has_descriptor_buffer);

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
        .pDynamicState = &dynamic_state,
        .layout = std::get<0>(desc->root_signature),
        .renderPass = std::get<0>(desc->render_pass),
    };

    VkPipeline pipeline;
    auto result = device.table()->vkCreateGraphicsPipelines(device.get(), nullptr, 1u, &info, nullptr, &pipeline);
    return wis::succeeded(result)
            ? std::pair{ wis::success, wis::VKPipelineState{ wis::managed_handle_ex<VkPipeline>{ pipeline, device, device.table()->vkDestroyPipeline } } }
            : std::pair{ wis::make_result<FUNC, "Failed to create a graphics pipeline">(result), wis::VKPipelineState{} };
}