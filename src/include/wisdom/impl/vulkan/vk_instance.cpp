#ifndef WIS_VK_INSTANCE_CPP
#define WIS_VK_INSTANCE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/impl/vulkan/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>
#include <memory>
#include <algorithm>
#include <semaphore>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

namespace wis::detail {
//-----------------------------------------------------------------------------
inline constexpr uint32_t order_performance(VkPhysicalDeviceType t)
{
    switch (t) {
    default:
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return 3;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return 4;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return 2;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
        return 1;
    }
}

//-----------------------------------------------------------------------------
inline constexpr uint32_t order_power(VkPhysicalDeviceType t)
{
    switch (t) {
    default:
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return 4;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return 3;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return 2;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
        return 1;
    }
}

inline void release_vk_instance(VkInstance instance, detail::control_block<VKInstanceHeader>* header) noexcept
{
    if (header && header->release() == 1) {
        // Destroy debug messenger if exists
        if (header->header.debug_messenger != VK_NULL_HANDLE &&
            header->header.instance_table.vkDestroyDebugUtilsMessengerEXT) {
            header->header.instance_table.vkDestroyDebugUtilsMessengerEXT(
                    instance,
                    header->header.debug_messenger,
                    nullptr);
        }

        // Last reference, destroy instance
        std::atomic_thread_fence(std::memory_order_acquire);
        header->header.instance_table.vkDestroyInstance(instance, nullptr);
        delete header;
    }
}

//-----------------------------------------------------------------------------
inline void release_vk_device(VkDevice device, detail::control_block<VKDeviceHeader>* header) noexcept
{
    if (header && header->release() == 1) {
        // Last reference, destroy device
        std::atomic_thread_fence(std::memory_order_acquire);

        // Destroy queue semaphores
        auto* sems = reinterpret_cast<std::binary_semaphore*>(header->header.queue_semaphores);
        std::destroy_n(sems, header->header.queue_family_count);

        // Deallocate semaphores memory manually
        ::operator delete[](sems, std::align_val_t(alignof(std::binary_semaphore)), std::nothrow);

        // Destroy static sampler pool allocator (unchecked, because we are the last holder of device)
        header->header.static_sampler_pool_allocator.DestroyPoolsUnchecked(device,
                                                                           header->header.device_table);

        header->header.device_table.vkDestroyDevice(device, nullptr);

        // Destroy instance
        release_vk_instance(header->header.instance,
                            header->header.shared_header);

        delete header;
    }
}

inline int32_t get_best_queue_family_index(WisCommandQueueType type, wis::span<const VkQueueFamilyProperties> queue_family_properties)
{
    int32_t best_index = -1;
    for (uint32_t i = 0; i < queue_family_properties.size(); ++i) {
        auto& props = queue_family_properties[i];
        switch (type) {
        case WisCommandQueueType::WisCommandQueueTypeGraphics:
            if (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return i; // prefer graphics
            }
            break;
        case WisCommandQueueType::WisCommandQueueTypeCompute:
            if (props.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                if (best_index == -1) {
                    best_index = i; // first compute queue found
                } else if (!(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                           (queue_family_properties[best_index].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                    best_index = i; // prefer compute-only over graphics+compute
                }
            }
            break;
        case WisCommandQueueType::WisCommandQueueTypeTransfer:
            if (props.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                if (best_index == -1) {
                    best_index = i; // first transfer queue found
                } else {
                    bool current_is_dedicated = !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                            !(props.queueFlags & VK_QUEUE_COMPUTE_BIT);
                    bool best_is_dedicated = !(queue_family_properties[best_index].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                            !(queue_family_properties[best_index].queueFlags & VK_QUEUE_COMPUTE_BIT);
                    if (current_is_dedicated && !best_is_dedicated) {
                        best_index = i; // prefer transfer-only
                    }
                }
            }
            break;
        case WisCommandQueueType::WisCommandQueueTypeVideoDecode:
            if (props.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) {
                return i; // prefer video decode
            }
            break;
        case WisCommandQueueType::WisCommandQueueTypeVideoEncode:
            if (props.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) {
                return i; // prefer video encode
            }
            break;
        default:
            return -1;
        }
    }
    return best_index;
};

struct DeviceExtension1 : VKDeviceExtensionImpl<DeviceExtension1> {
    VKDeviceFeatures features;

public:
    WisResult CollectInfo(VKDeviceExtensionCollector& collector) noexcept
    {
        // Optional extensions
        if (collector.IsExtensionPresent(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME)) {
            features.has_custom_border_color = true;

            collector.EnableExtension({
                    .name                 = VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT,
                    .property_struct_size = sizeof(VkPhysicalDeviceCustomBorderColorPropertiesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)) {
            collector.EnableExtension({
                    .name                 = VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceMaintenance4Features),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES,
                    .property_struct_size = sizeof(VkPhysicalDeviceMaintenance4Properties),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)) {
            features.index_buffer_range = true;
            collector.EnableExtension({
                    .name                 = VK_KHR_MAINTENANCE_5_EXTENSION_NAME,
                    .feature_struct       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_FEATURES_KHR,
                    .feature_struct_size  = sizeof(VkPhysicalDeviceMaintenance5FeaturesKHR),
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES_KHR,
                    .property_struct_size = sizeof(VkPhysicalDeviceMaintenance5PropertiesKHR),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)) {
            features.dynamic_rendering = true;
            collector.EnableExtension({
                    .name                = VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
                    .feature_struct_size = sizeof(VkPhysicalDeviceDynamicRenderingFeatures),
            });
        }

        if (collector.IsExtensionPresent(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME)) {
            features.extended_dynamic_state = true;
            collector.EnableExtension({
                    .name                = VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceExtendedDynamicState2FeaturesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)) {
            features.synchronization_2 = true;
            collector.EnableExtension({
                    .name                = VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
                    .feature_struct_size = sizeof(VkPhysicalDeviceSynchronization2FeaturesKHR),
            });
        }

        if (collector.IsExtensionPresent(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME)) {
            features.dynamic_render_unused_attachments = true;
            collector.EnableExtension({
                    .name                = VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME,
                    .feature_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT,
                    .feature_struct_size = sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT),
            });
        }

        if (collector.IsExtensionPresent(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME)) {
            features.push_descriptor = true;
            collector.EnableExtension({
                    .name                 = VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
                    .property_struct      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES,
                    .property_struct_size = sizeof(VkPhysicalDevicePushDescriptorPropertiesKHR),
            });
        }
        return vk_success;
    }
    WisResult Init(const impl::VKDeviceImpl&         device_impl,
                   const VKDeviceExtensionCollector& collector) noexcept
    {
        {
            auto& push_desc_properties = *collector.GetEnabledPropertyStruct<VkPhysicalDevicePushDescriptorProperties>(
                    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES);
            features.max_push_descriptors = push_desc_properties.maxPushDescriptors;
        }
        {
            auto& base_properties = *collector.GetEnabledPropertyStruct<VkPhysicalDeviceProperties2>(
                    VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2);
            features.max_push_constant_size = base_properties.properties.limits.maxPushConstantsSize;
        }

        // Nothing to initialize for now
        return vk_success;
    }
};
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKCreateInstance(const WisDebugDesc*            debug_layer,
                    WisVKInstanceExtensionHeader** extensions,
                    size_t                         extension_count,
                    WisVKInstance*                 instance)
{
    WisResult res = vk_success;
    // Instance can come as partially constructed from C side
    auto& impl = *reinterpret_cast<VKInstanceImpl*>(instance);

    auto header = make_unique<control_block<VKInstanceHeader>>();
    if (!header) {
        return make_result<Func(), "Failed to allocate memory for Vulkan instance header">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    header->header.library = unique_library{ wis::detail::InitializeVulkanLibrary() };
    if (!header->header.library) {
        return make_result<Func(), "Failed to load Vulkan library">(VK_ERROR_UNKNOWN);
    }

    if (!header->header.global_table.Init(header->header.library.get())) {
        return make_result<Func(), "Failed to initialize Vulkan global function table">(VK_ERROR_UNKNOWN);
    }

    auto& gt = header->header.global_table;

    VkResult vr      = VK_SUCCESS;
    uint32_t version = 0;
    if (!gt.vkEnumerateInstanceVersion) {
        return make_result<Func(), "The Vulkan version is too old!">(VK_ERROR_UNKNOWN);
    }

    // Get Vulkan instance version
    vr = gt.vkEnumerateInstanceVersion(&version);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to enumerate Vulkan instance version">(vr);
    }

    // Gather extensions
    VKInstanceExtensionCollector collector{ gt, res };
    if (res.status != WisStatusOk) {
        return res;
    }

    // Let extensions collect their info
    for (size_t i = 0; i < extension_count; ++i) {
        wis::VKInstanceExtensionHeader* ext_header = reinterpret_cast<wis::VKInstanceExtensionHeader*>(extensions[i]);
        auto                            res2       = ext_header->init_fptr(ext_header, nullptr, &collector);
        // Non-fatal, allow to silently fail
        (void)res2;
    }

    // Setup debug layer if requested
    bool debug_layer_enabled = debug_layer && debug_layer->enable_debug_layer;
    if (debug_layer_enabled) {
        collector.EnableLayer("VK_LAYER_KHRONOS_validation");
        collector.EnableExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // Prepared enabled extensions and layers arrays
    auto&& [ext_layer_array, ext_count, layer_count] = collector.GetExtensionsAndLayers(res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Prepare debug callback thunk
    std::unique_ptr<VKDebugCallbackThunk> debug_layer_thunk;
    if (debug_layer && debug_layer->callback) {
        debug_layer_thunk = make_unique<VKDebugCallbackThunk>();
        if (debug_layer_thunk) {
            debug_layer_thunk->callback  = debug_layer->callback;
            debug_layer_thunk->user_data = debug_layer->user_data;
        }
        // Non-fatal, allow to silently fail
    }

    // Create Vulkan instance
    VkApplicationInfo info{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion         = version,
    };
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = VKDebugCallbackThunk::wisDebugUtilsMessengerCallbackThunk,
        .pUserData       = debug_layer_thunk.get(),
    };
    VkInstanceCreateInfo create_info{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = debug_layer_thunk ? &debug_create_info : nullptr,
        .pApplicationInfo        = &info,
        .enabledLayerCount       = static_cast<uint32_t>(layer_count),
        .ppEnabledLayerNames     = layer_count ? ext_layer_array.get() + ext_count : nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(ext_count),
        .ppEnabledExtensionNames = ext_layer_array.get(),
    };

    VkInstance instance_handle = VK_NULL_HANDLE;
    vr                         = gt.vkCreateInstance(&create_info, nullptr, &instance_handle);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan instance">(vr);
    }

    // Initialize instance table
    auto& instance_table = header->header.instance_table;
    if (!instance_table.Init(instance_handle, gt.vkGetInstanceProcAddr)) {
        instance_table.vkDestroyInstance(instance_handle, nullptr); // cleanup
        return make_result<Func(), "Failed to initialize Vulkan instance function table">(VK_ERROR_UNKNOWN);
    }

    // Initialize adapter table
    if (!header->header.adapter_table.Init(instance_handle, gt.vkGetInstanceProcAddr)) {
        instance_table.vkDestroyInstance(instance_handle, nullptr); // cleanup
        return make_result<Func(), "Failed to initialize Vulkan adapter function table">(VK_ERROR_UNKNOWN);
    }

    // Setup debug messenger if requested
    if (debug_layer_thunk && instance_table.vkCreateDebugUtilsMessengerEXT) {
        auto vr2 = instance_table.vkCreateDebugUtilsMessengerEXT(
                instance_handle,
                &debug_create_info,
                nullptr,
                &header->header.debug_messenger);
        // Non-fatal, allow to silently fail
        (void)vr2;
    }

    // Fill instance impl
    impl.instance      = instance_handle;
    impl.shared_header = header.release();

    // Store debug thunk
    impl.shared_header->header.debug_callback_thunk = std::move(debug_layer_thunk);
    impl.shared_header->header.api_version          = version;

    // Initialize instance extensions
    for (auto* ext : wis::span<WisVKInstanceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<VKInstanceExtensionHeader*>(ext);
        if (table) {
            auto xres = table->init_fptr(table, &impl, &collector);
            if (xres.status != WisStatusOk) {
                res.status = WisStatusPartial; // mark as partial success if any extension fails
            }
        }
    }
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyInstance(WisVKInstance* self)
{
    auto& impl = *reinterpret_cast<VKInstanceImpl*>(self);
    if (!impl.instance) {
        return;
    }
    detail::release_vk_instance(impl.instance, impl.shared_header);
    impl.shared_header = nullptr;
    impl.instance      = VK_NULL_HANDLE;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKInstanceQueryAdapters(const WisVKInstance* self,
                                                             WisAdapterPreference preference,
                                                             WisVKAdapterQuery*   query)
{
    // Query can come as partially constructed from C side
    auto& impl          = *reinterpret_cast<VKAdapterQueryImpl*>(query);
    auto& instance_impl = *reinterpret_cast<const VKInstanceImpl*>(self);

    auto&       header = *instance_impl.shared_header;
    const auto& table  = header.header.instance_table;
    const auto& atable = header.header.adapter_table;

    std::unique_ptr<VkPhysicalDevice[]> devices_ref;

    std::unique_ptr<std::byte[]> aux_pool;

    // Get number of physical devices
    uint32_t device_count = 0;
    VkResult vr           = table.vkEnumeratePhysicalDevices(instance_impl.instance, &device_count, nullptr);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to enumerate Vulkan physical devices">(vr);
    }
    if (device_count == 0) {
        return make_result<Func(), "No Vulkan physical devices found">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get physical devices
    devices_ref = make_unique<VkPhysicalDevice[]>(device_count);
    if (!devices_ref) {
        return make_result<Func(), "Not enough memory for physical devices array">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    vr = table.vkEnumeratePhysicalDevices(instance_impl.instance, &device_count, devices_ref.get());
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to enumerate Vulkan physical devices">(vr);
    }

    if (preference == WisAdapterPreference::WisAdapterPreferenceNone) {
        // No sorting needed
        impl.adapter_count    = device_count;
        impl.physical_devices = devices_ref.release();
        impl.instance         = instance_impl.instance;
        impl.shared_header    = instance_impl.shared_header;
        impl.shared_header->add_ref(); // hold reference to instance header
        return vk_success;
    }

    // Sort devices based on preference
    constexpr static std::size_t max_align      = std::max(alignof(VkPhysicalDeviceProperties), alignof(std::uintptr_t));
    std::size_t                  total_aux_size = sizeof(VkPhysicalDeviceProperties) * device_count + device_count * sizeof(std::uintptr_t);

    aux_pool = make_unique<std::byte[]>(total_aux_size + max_align - 1);
    if (!aux_pool) {
        return make_result<Func(), "Not enough memory for auxiliary sorting buffer">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    // Aligned pointers
    auto*                                 aux_ptr = aligned_address(aux_pool.get(), max_align);
    wis::span<VkPhysicalDeviceProperties> properties_span{
        reinterpret_cast<VkPhysicalDeviceProperties*>(aux_ptr),
        device_count,
    };
    wis::span<std::uintptr_t> index_span{
        reinterpret_cast<std::uintptr_t*>(aux_ptr + sizeof(VkPhysicalDeviceProperties) * device_count),
        device_count,
    };

    // Gather properties
    for (std::size_t i = 0; i < device_count; ++i) {
        atable.vkGetPhysicalDeviceProperties(devices_ref[i], &properties_span[i]);
        index_span[i] = static_cast<std::uintptr_t>(i);
    }

    // Sort functions (heuristics)
    auto less_consumption = [&](std::uintptr_t a, std::uintptr_t b) {
        VkPhysicalDeviceProperties& a_properties = properties_span[a];
        VkPhysicalDeviceProperties& b_properties = properties_span[b];
        auto                        a_order      = wis::detail::order_power(a_properties.deviceType);
        auto                        b_order      = wis::detail::order_power(b_properties.deviceType);
        if (a_order != b_order) {
            return a_order > b_order;
        }
        return a_properties.limits.maxMemoryAllocationCount > b_properties.limits.maxMemoryAllocationCount;
    };
    auto less_performance = [&](std::uintptr_t a, std::uintptr_t b) {
        VkPhysicalDeviceProperties& a_properties = properties_span[a];
        VkPhysicalDeviceProperties& b_properties = properties_span[b];
        auto                        a_order      = wis::detail::order_performance(a_properties.deviceType);
        auto                        b_order      = wis::detail::order_performance(b_properties.deviceType);
        if (a_order != b_order) {
            return a_order > b_order;
        }
        return a_properties.limits.maxMemoryAllocationCount > b_properties.limits.maxMemoryAllocationCount;
    };

    // Sort indices based on preference
    switch (preference) {
    case WisAdapterPreference::WisAdapterPreferenceMinConsumption:
        std::sort(index_span.begin(), index_span.end(), [&](std::uintptr_t a, std::uintptr_t b) {
            return less_consumption(a, b);
        });
        break;
    case WisAdapterPreference::WisAdapterPreferencePerformance:
        std::sort(index_span.begin(), index_span.end(), [&](std::uintptr_t a, std::uintptr_t b) {
            return less_performance(a, b);
        });
        break;
    default:
        // No sorting
        break;
    }

    // Reorder devices_ref based on sorted indices O(n) algorithm with reused space
    wis::span<VkPhysicalDevice> ptr_span{
        reinterpret_cast<VkPhysicalDevice*>(index_span.data()),
        device_count,
    };

    // Copy original pointers to aux buffer
    for (std::size_t i = 0; i < device_count; ++i) {
        ptr_span[i] = devices_ref[index_span[i]];
    }
    std::memcpy(devices_ref.get(), ptr_span.data(), sizeof(VkPhysicalDevice) * device_count);

    // Fill query impl
    impl.adapter_count    = device_count;
    impl.physical_devices = devices_ref.release();
    impl.instance         = instance_impl.instance;
    impl.shared_header    = instance_impl.shared_header;
    impl.shared_header->add_ref(); // hold reference to instance header
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyAdapterQuery(WisVKAdapterQuery* self)
{
    auto& impl = *reinterpret_cast<VKAdapterQueryImpl*>(self);
    if (impl.physical_devices) {
        delete[] impl.physical_devices;
    }

    detail::release_vk_instance(impl.instance, impl.shared_header);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self)
{
    return reinterpret_cast<const VKAdapterQueryImpl*>(self)->adapter_count;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKAdapterQueryGetAdapterDesc(const WisVKAdapterQuery* self,
                                                                  size_t                   index,
                                                                  WisAdapterDesc*          desc)
{
    auto& impl = *reinterpret_cast<const VKAdapterQueryImpl*>(self);
    if (index >= impl.adapter_count) {
        return make_result<Func(), "Adapter index out of bounds">(VK_ERROR_INITIALIZATION_FAILED);
    }
    auto& atable  = impl.shared_header->header.adapter_table;
    auto  adapter = impl.physical_devices[index];

    VkPhysicalDeviceIDProperties id_props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES,
    };
    VkPhysicalDeviceProperties2 properties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = &id_props
    };
    atable.vkGetPhysicalDeviceProperties2(adapter, &properties);

    auto& got_desc = properties.properties;

    VkPhysicalDeviceMemoryProperties memory_props{};
    atable.vkGetPhysicalDeviceMemoryProperties(adapter, &memory_props);

    // Get flags
    WisAdapterFlags flag{};
    if ((got_desc.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
        flag = WisAdapterFlags(flag | WisAdapterFlags::WisAdapterFlagsRemote);
    }
    if ((got_desc.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU) == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU) {
        flag = WisAdapterFlags(flag | WisAdapterFlags::WisAdapterFlagsSoftware);
    }

    uint64_t dedicated_video_memory = 0;
    uint64_t shared_system_memory   = 0;

    wis::span types{ memory_props.memoryTypes };
    for (auto& i : types) {
        if (i.propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
            memory_props.memoryHeaps[i.heapIndex].flags &
                    VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            dedicated_video_memory = memory_props.memoryHeaps[i.heapIndex].size;
        }

        if (i.propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            shared_system_memory = memory_props.memoryHeaps[i.heapIndex].size;
        }
        if ((dedicated_video_memory != 0u) && (shared_system_memory != 0u)) {
            break;
        }
    }

    *desc = WisAdapterDesc{
        .vendor_id = got_desc.vendorID,
        .device_id = got_desc.deviceID,

        .dedicated_video_memory = dedicated_video_memory,
        .shared_system_memory   = shared_system_memory, // Vulkan does not expose shared system memory directly

        .adapter_id = id_props.deviceLUIDValid ? *reinterpret_cast<const uint64_t*>(id_props.deviceUUID) : 0,
        .flags      = flag,
    };

    std::memcpy(desc->description, got_desc.deviceName, sizeof(desc->description) - 1);
    std::memcpy(desc->adapter_uuid, id_props.deviceUUID, sizeof(desc->adapter_uuid));
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery*     self,
                                                                size_t                       index,
                                                                WisVKDeviceExtensionHeader** extensions,
                                                                size_t                       extension_count,
                                                                WisVKDevice*                 device)
{
    WisResult res  = vk_success;
    auto&     impl = *reinterpret_cast<const VKAdapterQueryImpl*>(self);
    if (index >= impl.adapter_count) {
        return make_result<Func(), "Adapter index out of bounds">(VK_ERROR_INITIALIZATION_FAILED);
    }

    auto header = make_unique<control_block<VKDeviceHeader>>();
    if (!header) {
        return make_result<Func(), "Failed to allocate memory for Vulkan device header">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    auto& atable  = impl.shared_header->header.adapter_table;
    auto& adapter = impl.physical_devices[index];

    VKDeviceExtensionCollector collector{ atable, adapter, res };
    if (res.status != WisStatusOk) {
        return res;
    }

    // Let extensions collect their info
    for (size_t i = 0; i < extension_count; ++i) {
        auto* header = reinterpret_cast<wis::VKDeviceExtensionHeader*>(extensions[i]);
        if (header) {
            auto res2 = header->init_fptr(header, nullptr, &collector);
            // Non-fatal, allow to silently fail
            (void)res2;
        }
    }

    detail::DeviceExtension1 device_ext1;
    auto                     xres = device_ext1.CollectInfo(collector);

    // Prepared enabled extensions array
    auto&& [ext_buffer, ext_strings, ext_count, feature_structs, property_structs] = collector.GetInitBuffer(res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Create default enabled features
    VkPhysicalDeviceVulkan12Features vulkan12_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = feature_structs, // link to extension features
    };
    VkPhysicalDeviceVulkan11Features vulkan11_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = &vulkan12_features,
    };
    VkPhysicalDeviceFeatures2 features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &vulkan11_features,
    };
    atable.vkGetPhysicalDeviceFeatures2(adapter, &features);

    // Add default features to collector
    collector.ForceBindFeatureStruct(&features);
    collector.ForceBindFeatureStruct(&vulkan11_features);
    collector.ForceBindFeatureStruct(&vulkan12_features);

    // Create properties structures
    VkPhysicalDeviceVulkan12Properties vulkan12_properties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
        .pNext = property_structs, // link to extension properties
    };
    VkPhysicalDeviceVulkan11Properties vulkan11_properties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
        .pNext = &vulkan12_properties,
    };
    VkPhysicalDeviceProperties2 properties{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = &vulkan11_properties,
    };
    atable.vkGetPhysicalDeviceProperties2(adapter, &properties);

    // Add default properties to collector
    collector.ForceBindPropertyStruct(&properties);
    collector.ForceBindPropertyStruct(&vulkan11_properties);
    collector.ForceBindPropertyStruct(&vulkan12_properties);

    // Initialize queues
    uint32_t count = 0;
    atable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, nullptr);

    if (count == 0) {
        return make_result<Func(), "No queue families found on the selected Vulkan adapter">(VK_ERROR_INITIALIZATION_FAILED);
    }

    std::unique_ptr<VkQueueFamilyProperties[]> family_props = make_unique<VkQueueFamilyProperties[]>(count);
    if (!family_props) {
        return make_result<Func(), "Not enough memory for device queue family properties array">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }
    atable.vkGetPhysicalDeviceQueueFamilyProperties(adapter, &count, family_props.get());

    float                                      priority           = 1.0f;
    std::unique_ptr<VkDeviceQueueCreateInfo[]> queue_create_infos = make_unique<VkDeviceQueueCreateInfo[]>(count);
    if (!queue_create_infos) {
        return make_result<Func(), "Not enough memory for device queue create infos array">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }
    for (uint32_t i = 0; i < count; ++i) {
        queue_create_infos[i] = VkDeviceQueueCreateInfo{
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = i,
            .queueCount       = 1,
            .pQueuePriorities = &priority,
        };
    }

    // Get optimal queue index mapping
    auto prop_span = wis::span<const VkQueueFamilyProperties>{ family_props.get(), count };
    for (uint32_t i = 0; i < WisCommandQueueTypeCount; ++i) {

        int32_t queue_family_index = wis::detail::get_best_queue_family_index(static_cast<WisCommandQueueType>(i),
                                                                              prop_span);

        header->header.common_queue_family_indices[i] = static_cast<uint16_t>(queue_family_index >= 0 ? queue_family_index : 0xFFFF);
    }

    // Create device
    VkDeviceCreateInfo device_create_info{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &features, // link to extension features
        .queueCreateInfoCount    = count,
        .pQueueCreateInfos       = queue_create_infos.get(),
        .enabledExtensionCount   = static_cast<uint32_t>(ext_count),
        .ppEnabledExtensionNames = ext_strings,
    };
    VkDevice device_handle = VK_NULL_HANDLE;
    VkResult vr            = atable.vkCreateDevice(adapter, &device_create_info, nullptr, &device_handle);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan device">(vr);
    }

    // Initialize device table
    auto& device_table = header->header.device_table;
    auto& gtable       = impl.shared_header->header.global_table;
    if (!device_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return make_result<Func(), "Failed to initialize Vulkan device function table">(VK_ERROR_UNKNOWN);
    }

    // Initialize command queue table
    if (!header->header.command_queue_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return make_result<Func(), "Failed to initialize Vulkan command queue function table">(VK_ERROR_UNKNOWN);
    }

    // Initialize command list table
    if (!header->header.command_list_table.Init(device_handle, gtable.vkGetDeviceProcAddr)) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return make_result<Func(), "Failed to initialize Vulkan command list function table">(VK_ERROR_UNKNOWN);
    }

    // Allocate queue semaphores
    uint8_t* queue_sems = new (std::align_val_t(alignof(std::binary_semaphore)), std::nothrow) uint8_t[count * sizeof(std::binary_semaphore)];
    if (!queue_sems) {
        device_table.vkDestroyDevice(device_handle, nullptr); // cleanup
        return make_result<Func(), "Not enough memory for device queue binding semaphores">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    // Initialize semaphores
    for (uint32_t i = 0; i < count; ++i) {
        new (queue_sems + sizeof(std::binary_semaphore) * i) std::binary_semaphore(1);
    }

    // Fill device impl
    auto& device_impl           = *reinterpret_cast<VKDeviceImpl*>(device);
    device_impl.device_header   = header.release();
    device_impl.device          = device_handle;
    device_impl.physical_device = adapter;

    auto& device_header         = device_impl.device_header->header;
    device_header.shared_header = impl.shared_header;
    device_header.shared_header->add_ref(); // hold reference to instance header
    device_header.instance = impl.instance;

    // Store queue family properties
    device_header.queue_family_count      = count;
    device_header.queue_family_properties = std::move(family_props);
    device_header.queue_semaphores        = queue_sems;

    auto res2 = device_ext1.Init(device_impl, collector);
    // Non-fatal, allow to silently fail
    (void)res2;

    // Store device extensions info
    device_header.features = device_ext1.features;

    // Initialize device extensions
    for (auto* ext : wis::span<WisVKDeviceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<wis::VKDeviceExtensionHeader*>(ext);
        if (table) {
            auto xres = table->init_fptr(table, &device_impl, &collector);
            if (xres.status != WisStatusOk) {
                res.status = WisStatusPartial; // mark as partial success if any extension fails
            }
        }
    }

    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyDevice(WisVKDevice* self)
{
    auto& impl = *reinterpret_cast<VKDeviceImpl*>(self);
    if (!impl.device) {
        return;
    }
    detail::release_vk_device(impl.device, impl.device_header);
    impl.device_header = nullptr;
    impl.device        = VK_NULL_HANDLE;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice*  self,
                                                                WisCommandQueueType type,
                                                                WisVKCommandQueue*  queue)
{
    WisResult res        = vk_success;
    auto&     device     = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     queue_impl = *reinterpret_cast<VKCommandQueueImpl*>(queue);
    VkQueue   vk_queue   = VK_NULL_HANDLE;

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return make_result<Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint16_t queue_family_index = device.device_header->header.common_queue_family_indices[static_cast<size_t>(type)];
    if (queue_family_index == 0xFFFF) {
        return make_result<Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    VkDeviceQueueInfo2 queue_info{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family_index),
        .queueIndex       = 0,
    };
    device.device_header->header.device_table.vkGetDeviceQueue2(device.device, &queue_info, &vk_queue);

    if (vk_queue == VK_NULL_HANDLE) {
        return make_result<Func(), "Failed to get Vulkan device queue">(VK_ERROR_INITIALIZATION_FAILED);
    }
    // Fill command queue impl
    queue_impl.queue         = vk_queue;
    queue_impl.device_header = device.device_header;
    device.device_header->add_ref(); // hold reference to device header
    queue_impl.device = device.device;

    // Setup semaphore pointer
    queue_impl.semaphore_ptr = device.device_header->header.queue_semaphores +
            sizeof(std::binary_semaphore) * queue_family_index;
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self)
{
    auto& impl = *reinterpret_cast<VKCommandQueueImpl*>(self);
    if (impl.queue) {
        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
        impl.queue         = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateCommandList(const WisVKDevice*  self,
                                                               WisCommandQueueType type,
                                                               WisVKCommandList*   list)
{
    WisResult res       = vk_success;
    auto&     device    = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     list_impl = *reinterpret_cast<VKCommandListImpl*>(list);

    // Sanity check: lower and upper bound
    using QueueTypeUnderlying = std::underlying_type_t<WisCommandQueueType>;
    if (static_cast<QueueTypeUnderlying>(type) < 0 ||
        static_cast<size_t>(type) >= WisCommandQueueTypeCount) {
        return make_result<Func(), "Invalid command queue type specified">(VK_ERROR_INITIALIZATION_FAILED);
    }

    // Get queue family index based on type
    uint16_t queue_family_index = device.device_header->header.common_queue_family_indices[static_cast<size_t>(type)];
    if (queue_family_index == 0xFFFF) {
        return make_result<Func(), "No suitable queue family found for the requested queue type">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    // Create command pool
    VkCommandPoolCreateInfo pool_info{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = static_cast<uint32_t>(queue_family_index),
    };
    VkCommandPool command_pool = VK_NULL_HANDLE;
    auto&         table        = device.device_header->header.device_table;
    VkResult      vr           = table.vkCreateCommandPool(device.device, &pool_info, nullptr, &command_pool);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan command pool">(vr);
    }
    // Create command buffer
    VkCommandBufferAllocateInfo alloc_info{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = command_pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer = VK_NULL_HANDLE;

    vr = table.vkAllocateCommandBuffers(device.device, &alloc_info, &command_buffer);
    if (!succeeded(vr)) {
        table.vkDestroyCommandPool(device.device, command_pool, nullptr); // cleanup
        return make_result<Func(), "Failed to allocate Vulkan command buffer">(vr);
    }

    // Fill command list impl
    list_impl.command_pool   = command_pool;
    list_impl.command_buffer = command_buffer;
    list_impl.device_header  = device.device_header;
    device.device_header->add_ref(); // hold reference to device header
    list_impl.device = device.device;
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self)
{
    auto& impl = *reinterpret_cast<VKCommandListImpl*>(self);
    if (impl.command_buffer != VK_NULL_HANDLE) {
        // Free command buffer
        auto& table = impl.device_header->header.device_table;
        table.vkDestroyCommandPool(impl.device, impl.command_pool, nullptr); // this also frees command buffers
        impl.command_buffer = VK_NULL_HANDLE;

        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
    }
}

WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
                                                         uint64_t           initial_value,
                                                         WisVKFence*        fence)
{
    WisResult res       = vk_success;
    auto&     device    = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     out_fence = *reinterpret_cast<VKFenceImpl*>(fence);

    VkSemaphoreTypeCreateInfo timeline_desc{
        .sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue  = initial_value,
    };

    VkSemaphoreCreateInfo desc{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = &timeline_desc,
    };
    VkSemaphore semaphore = VK_NULL_HANDLE;
    auto&       table     = device.device_header->header.device_table;
    VkResult    vr        = table.vkCreateSemaphore(device.device, &desc, nullptr, &semaphore);
    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan timeline semaphore">(vr);
    }
    // Fill fence impl
    out_fence.fence         = semaphore;
    out_fence.device        = device.device;
    out_fence.device_header = device.device_header;
    device.device_header->add_ref(); // hold reference to device header
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyFence(WisVKFence* self)
{
    auto& impl = *reinterpret_cast<VKFenceImpl*>(self);
    if (impl.fence != VK_NULL_HANDLE) {
        auto& table = impl.device_header->header.device_table;
        table.vkDestroySemaphore(impl.device, impl.fence, nullptr);
        impl.fence = VK_NULL_HANDLE;
        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreateResourceAllocator(const WisVKDevice*      self,
                                                                     WisVKResourceAllocator* allocator)
{
    WisResult res             = vk_success;
    auto&     device          = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     device_header   = device.device_header->header;
    auto&     instance_header = device_header.shared_header->header;
    auto&     adapter         = device.physical_device;

    uint32_t version = instance_header.api_version;
    auto&    itable  = instance_header.instance_table;
    auto&    gtable  = instance_header.global_table;
    auto&    dtable  = device_header.device_table;
    auto&    atable  = instance_header.adapter_table;
    auto&    ctable  = device_header.command_list_table;

    VmaVulkanFunctions allocator_functions{
        .vkGetInstanceProcAddr                   = gtable.vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr                     = gtable.vkGetDeviceProcAddr,
        .vkGetPhysicalDeviceProperties           = atable.vkGetPhysicalDeviceProperties,
        .vkGetPhysicalDeviceMemoryProperties     = atable.vkGetPhysicalDeviceMemoryProperties,
        .vkAllocateMemory                        = dtable.vkAllocateMemory,
        .vkFreeMemory                            = dtable.vkFreeMemory,
        .vkMapMemory                             = dtable.vkMapMemory,
        .vkUnmapMemory                           = dtable.vkUnmapMemory,
        .vkFlushMappedMemoryRanges               = dtable.vkFlushMappedMemoryRanges,
        .vkInvalidateMappedMemoryRanges          = dtable.vkInvalidateMappedMemoryRanges,
        .vkBindBufferMemory                      = dtable.vkBindBufferMemory,
        .vkBindImageMemory                       = dtable.vkBindImageMemory,
        .vkGetBufferMemoryRequirements           = dtable.vkGetBufferMemoryRequirements,
        .vkGetImageMemoryRequirements            = dtable.vkGetImageMemoryRequirements,
        .vkCreateBuffer                          = dtable.vkCreateBuffer,
        .vkDestroyBuffer                         = dtable.vkDestroyBuffer,
        .vkCreateImage                           = dtable.vkCreateImage,
        .vkDestroyImage                          = dtable.vkDestroyImage,
        .vkCmdCopyBuffer                         = ctable.vkCmdCopyBuffer,
        .vkGetBufferMemoryRequirements2KHR       = dtable.vkGetBufferMemoryRequirements2,
        .vkGetImageMemoryRequirements2KHR        = dtable.vkGetImageMemoryRequirements2,
        .vkBindBufferMemory2KHR                  = dtable.vkBindBufferMemory2,
        .vkBindImageMemory2KHR                   = dtable.vkBindImageMemory2,
        .vkGetPhysicalDeviceMemoryProperties2KHR = atable.vkGetPhysicalDeviceMemoryProperties2,
        .vkGetDeviceBufferMemoryRequirements     = dtable.vkGetDeviceBufferMemoryRequirements,
        .vkGetDeviceImageMemoryRequirements      = dtable.vkGetDeviceImageMemoryRequirements,
    };

    VkPhysicalDeviceMemoryProperties2 mem_props{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
    };
    atable.vkGetPhysicalDeviceMemoryProperties2(adapter, &mem_props);

    VmaAllocatorCreateInfo allocatorInfo{
        .flags            = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice   = adapter,
        .device           = device.device,
        .pVulkanFunctions = &allocator_functions,
        .instance         = device_header.instance,
        .vulkanApiVersion = version
    };

    // Enable maintenance5 if available and maintenance4
    if (dtable.vkGetDeviceBufferMemoryRequirements) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE4_BIT;
    }
    if (device_header.features.index_buffer_range) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE5_BIT;
    }

#ifdef _WIN32
    // Only if there is an interop extension
    if (dtable.vkGetMemoryWin32HandleKHR) {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT;
        allocator_functions.vkGetMemoryWin32HandleKHR = dtable.vkGetMemoryWin32HandleKHR;
    }
#endif // _WIN32

    VmaAllocator out_allocator;
    VkResult     vr = vmaCreateAllocator(&allocatorInfo, &out_allocator);

    if (!succeeded(vr)) {
        return make_result<Func(), "Failed to create Vulkan memory allocator">(vr);
    }

    // Fill allocator impl
    auto& allocator_impl         = *reinterpret_cast<VKResourceAllocatorImpl*>(allocator);
    allocator_impl.allocator     = out_allocator;
    allocator_impl.device        = device.device;
    allocator_impl.device_header = device.device_header;
    device.device_header->add_ref(); // hold reference to device header

    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self)
{
    auto& impl = *reinterpret_cast<VKResourceAllocatorImpl*>(self);
    if (impl.allocator) {
        vmaDestroyAllocator(impl.allocator);
        impl.allocator = nullptr;

        detail::release_vk_device(impl.device, impl.device_header);
        impl.device        = VK_NULL_HANDLE;
        impl.device_header = nullptr;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKDeviceCreatePipelineLayout(const WisVKDevice*           self,
                                                                  const WisPipelineLayoutDesc* desc,
                                                                  WisVKPipelineLayout*         layout)
{
    WisResult res         = vk_success;
    auto&     device      = *reinterpret_cast<const VKDeviceImpl*>(self);
    auto&     layout_impl = *reinterpret_cast<VKPipelineLayoutImpl*>(layout);
    auto&     table       = device.device_header->header.device_table;
    auto      features    = device.device_header->header.features;

    // Pre checks
    uint32_t layout_count  = 0;
    uint32_t binding_count = 0;
    if (desc->push_descriptor_count > 0) {
        layout_count += 1;
        binding_count += static_cast<uint32_t>(desc->push_descriptor_count);
        if (!features.push_descriptor) {
            return make_result<Func(), "Push descriptors are not supported on this Vulkan device">(VK_ERROR_FEATURE_NOT_PRESENT);
        }

        // Check upper limit
        if (desc->push_descriptor_count > features.max_push_descriptors) {
            return make_result<Func(), "Exceeded maximum number of Vulkan push descriptors">(VK_ERROR_INITIALIZATION_FAILED);
        }
    }

    // Static samplers
    if (desc->static_sampler_count > 0) {
        layout_count += 1;
        binding_count += static_cast<uint32_t>(desc->static_sampler_count);
    }

    // How many DSLs are needed
    std::unique_ptr<impl::VKDescriptorSetLayoutContainer> dsl_layouts;
    if (layout_count > 0) {
        dsl_layouts = std::unique_ptr<impl::VKDescriptorSetLayoutContainer>(static_cast<VKDescriptorSetLayoutContainer*>(
                ::operator new(sizeof(VKDescriptorSetLayoutContainer) + (layout_count - 1) * sizeof(VkDescriptorSetLayout) + desc->static_sampler_count * (sizeof(VkSampler)))));
        if (!dsl_layouts) {
            return make_result<Func(), "Not enough memory for Vulkan descriptor set layouts container">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }
        dsl_layouts->dsl_count = layout_count;
    }

    auto destroy_allocated_sets = [&]() {
        if (dsl_layouts) {
            for (uint32_t i = 0; i < dsl_layouts->dsl_count; ++i) {
                if (dsl_layouts->vk_dsls[i] != VK_NULL_HANDLE) {
                    table.vkDestroyDescriptorSetLayout(device.device, dsl_layouts->vk_dsls[i], nullptr);
                    dsl_layouts->vk_dsls[i] = VK_NULL_HANDLE;
                }
            }
        }
    };
    auto destroy_allocated_samplers = [&]() {
        if (dsl_layouts) {
            wis::span<VkSampler> static_samplers_span{
                reinterpret_cast<VkSampler*>(dsl_layouts->vk_dsls + layout_count),
                desc->static_sampler_count
            };
            for (uint32_t i = 0; i < desc->static_sampler_count; i++) {
                if (static_samplers_span[i] != VK_NULL_HANDLE) {
                    table.vkDestroySampler(device.device, static_samplers_span[i], nullptr);
                    static_samplers_span[i] = VK_NULL_HANDLE;
                }
            }
        }
    };

    // Prepare push constant ranges
    VkPushConstantRange push_constant_ranges[std::size_t(ShaderStages::Count)];
    if (desc->push_constant_count > 0) {
        // Check upper limit
        if (desc->push_constant_count > static_cast<size_t>(ShaderStages::Count)) {
            return make_result<Func(), "Exceeded maximum number of Vulkan push constant ranges">(VK_ERROR_INITIALIZATION_FAILED);
        }

        for (size_t i = 0; i < desc->push_constant_count; ++i) {
            auto& src = desc->push_constants[i];
            auto& dst = push_constant_ranges[i];

            // Check size limit
            if (src.size_bytes > device.device_header->header.features.max_push_constant_size) {
                return make_result<Func(), "Exceeded maximum size of Vulkan push constant range">(VK_ERROR_INITIALIZATION_FAILED);
            }

            dst.stageFlags = detail::convert_vk(src.stage);
            dst.offset     = 0; // offset is always 0 for each push constant
            dst.size       = static_cast<uint32_t>(src.size_bytes);
        }
    }

    // Bindings
    uint32_t                         current_set = 0;
    wis::span<VkDescriptorSetLayout> out_dsls{
        dsl_layouts ? dsl_layouts->vk_dsls : nullptr,
        layout_count,
    };

    std::unique_ptr<VkDescriptorSetLayoutBinding[]> bindings;
    if (binding_count > 0) {
        bindings = make_unique<VkDescriptorSetLayoutBinding[]>(binding_count);
        if (!bindings) {
            return make_result<Func(), "Not enough memory for Vulkan descriptor set layout bindings">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }
    }

    // Push descriptors
    if (desc->push_descriptor_count > 0) {
        wis::span<const WisPushDescriptor> push_descriptors{
            desc->push_descriptors,
            desc->push_descriptor_count,
        };
        wis::span push_bindings{ bindings.get(), desc->push_descriptor_count };
        for (uint32_t i = 0; i < push_descriptors.size(); i++) {
            auto& r          = push_descriptors[i];
            push_bindings[i] = {
                .binding         = r.bind_register,
                .descriptorType  = convert_vk(r.type),
                .descriptorCount = 1, // Push descriptors are always single
                .stageFlags      = convert_vk(r.stage),
            };
        }
        VkDescriptorSetLayoutCreateInfo push_desc_info{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = nullptr,
            .flags        = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR,
            .bindingCount = static_cast<uint32_t>(desc->push_descriptor_count),
            .pBindings    = push_bindings.data(),
        };
        auto vr = table.vkCreateDescriptorSetLayout(device.device, &push_desc_info, nullptr, &out_dsls[current_set]);
        if (!succeeded(vr)) {
            return make_result<Func(), "Failed to create Vulkan push descriptor set layout">(vr);
        }
        current_set++;
    }

    // Static samplers
    VkDescriptorSet static_sampler_set = VK_NULL_HANDLE;
    if (desc->static_sampler_count > 0) {
        wis::span<const WisStaticSamplerDesc> static_samplers{
            desc->static_samplers,
            desc->static_sampler_count,
        };
        wis::span sampler_bindings{
            bindings.get() + desc->push_descriptor_count,
            desc->static_sampler_count
        };
        wis::span<VkSampler> static_samplers_span{
            reinterpret_cast<VkSampler*>(out_dsls.end()),
            desc->static_sampler_count
        };

        // Allocate sampler bindings
        for (uint32_t i = 0; i < static_samplers.size(); i++) {
            auto& r = static_samplers[i].sampler;
            // Create Vulkan sampler
            VkSamplerCreateInfo sampler_info{
                .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter               = convert_vk(r.mag_filter),
                .minFilter               = convert_vk(r.min_filter),
                .mipmapMode              = VkSamplerMipmapMode(r.mip_filter),
                .addressModeU            = convert_vk(r.address_u),
                .addressModeV            = convert_vk(r.address_v),
                .addressModeW            = convert_vk(r.address_w),
                .mipLodBias              = r.mip_lod_bias,
                .anisotropyEnable        = r.is_anisotropic,
                .maxAnisotropy           = std::max(float(r.max_anisotropy), 1.0f),
                .compareEnable           = r.comparison_op != WisCompareOperationNever,
                .compareOp               = convert_vk(r.comparison_op),
                .minLod                  = r.min_lod,
                .maxLod                  = r.max_lod,
                .borderColor             = convert_vk(r.static_border_color),
                .unnormalizedCoordinates = r.flags & WisSamplerFlagsNonNormalizedCoordinates ? VK_TRUE : VK_FALSE,
            };
            VkSampler sampler = VK_NULL_HANDLE;
            VkResult  vr      = table.vkCreateSampler(device.device, &sampler_info, nullptr, &sampler);
            if (!succeeded(vr)) {
                destroy_allocated_samplers();
                destroy_allocated_sets();
                return make_result<Func(), "Failed to create Vulkan static sampler">(vr);
            }
            static_samplers_span[i] = sampler;
        }

        for (uint32_t i = 0; i < sampler_bindings.size(); i++) {
            auto& r             = static_samplers[i];
            sampler_bindings[i] = {
                .binding            = r.bind_register,
                .descriptorType     = VK_DESCRIPTOR_TYPE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = convert_vk(r.stage),
                .pImmutableSamplers = &static_samplers_span[i]
            };
        }
        VkDescriptorSetLayoutCreateInfo static_desc_info{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(sampler_bindings.size()),
            .pBindings    = sampler_bindings.data(),
        };
        auto vr = table.vkCreateDescriptorSetLayout(device.device, &static_desc_info, nullptr, &out_dsls[current_set]);
        if (!succeeded(vr)) {
            destroy_allocated_samplers();
            destroy_allocated_sets();
            return make_result<Func(), "Failed to create Vulkan push descriptor set layout">(vr);
        }

        // Allocate static sampler set
        auto [set, pool] = device.device_header->header.static_sampler_pool_allocator.AllocateSet(
                device.device,
                table,
                out_dsls[current_set]);

        if (set == VK_NULL_HANDLE) {
            destroy_allocated_samplers();
            destroy_allocated_sets();
            return make_result<Func(), "Failed to allocate static sampler descriptor set">(VK_ERROR_INITIALIZATION_FAILED);
        }
        dsl_layouts->static_sampler_pool  = pool;
        dsl_layouts->static_sampler_count = desc->static_sampler_count;
        static_sampler_set                = set;
        current_set++;
    }

    auto destroy_static_sampler_set = [&]() {
        if (static_sampler_set != VK_NULL_HANDLE) {
            table.vkFreeDescriptorSets(device.device,
                                       dsl_layouts->static_sampler_pool,
                                       1,
                                       &static_sampler_set);
        }
    };

    // TODO: Tables

    // Create pipeline layout
    VkPipelineLayoutCreateInfo pipeline_layout_info{
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = layout_count,
        .pSetLayouts            = dsl_layouts ? dsl_layouts->vk_dsls : nullptr,
        .pushConstantRangeCount = static_cast<uint32_t>(desc->push_constant_count),
        .pPushConstantRanges    = push_constant_ranges,
    };
    VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
    auto             vr              = table.vkCreatePipelineLayout(device.device,
                                           &pipeline_layout_info,
                                           nullptr,
                                           &pipeline_layout);
    if (!succeeded(vr)) {
        destroy_static_sampler_set();
        destroy_allocated_samplers();
        destroy_allocated_sets();
        return make_result<Func(), "Failed to create Vulkan pipeline layout">(vr);
    }
    // Fill pipeline layout impl
    layout_impl.layout        = pipeline_layout;
    layout_impl.device        = device.device;
    layout_impl.device_header = device.device_header;
    device.device_header->add_ref(); // hold reference to device header
    layout_impl.dsl_container   = dsl_layouts.release();
    layout_impl.static_samplers = static_sampler_set;

    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyPipelineLayout(WisVKPipelineLayout* self)
{
    auto& impl = *reinterpret_cast<VKPipelineLayoutImpl*>(self);
    if (impl.layout != VK_NULL_HANDLE) {
        auto& table = impl.device_header->header.device_table;
        table.vkDestroyPipelineLayout(impl.device, impl.layout, nullptr);
        impl.layout = VK_NULL_HANDLE;

        // Release descriptor set layouts
        if (impl.dsl_container) {
            for (std::size_t i = 0; i < impl.dsl_container->dsl_count; ++i) {
                table.vkDestroyDescriptorSetLayout(impl.device, impl.dsl_container->vk_dsls[i], nullptr);
            }

            // Destroy static sampler set
            if (impl.static_samplers != VK_NULL_HANDLE) {
                table.vkFreeDescriptorSets(impl.device,
                                           impl.dsl_container->static_sampler_pool,
                                           1,
                                           &impl.static_samplers);
                impl.static_samplers = VK_NULL_HANDLE;

                // Destroy samplers
                wis::span<VkSampler> static_samplers_span{
                    reinterpret_cast<VkSampler*>(impl.dsl_container->vk_dsls + impl.dsl_container->dsl_count),
                    impl.dsl_container->static_sampler_count
                };
                for (std::size_t i = 0; i < impl.dsl_container->static_sampler_count; i++) {
                    table.vkDestroySampler(impl.device, static_samplers_span[i], nullptr);
                }
            }

            ::operator delete(impl.dsl_container);
            impl.dsl_container = nullptr;
        }

        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
    }
}
#endif // WIS_VK_INSTANCE_CPP
