#ifndef WIS_VK_INSTANCE_CPP
#define WIS_VK_INSTANCE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/impl/vulkan/vk_utils.hpp>
#include <wisdom/util/allocation.hpp>
#include <memory>
#include <unordered_set>
#include <algorithm>

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
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCreateInstance(bool                           debug_layer,
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
        reinterpret_cast<wis::VKInstanceExtensionHeader*>(extensions[i])->CollectInfo(collector);
    }

    // Prepared enabled extensions and layers arrays
    auto&& [ext_layer_array, ext_count, layer_count] = collector.GetExtensionsAndLayers(res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Create Vulkan instance
    VkApplicationInfo info{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion         = version,
    };
    VkInstanceCreateInfo create_info{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
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

    // Fill instance impl
    impl.instance      = instance_handle;
    impl.shared_header = header.release();
    impl.api_version   = version;
    impl.debug_layer   = debug_layer;

    // Initialize instance extensions
    for (auto* ext : wis::span<WisVKInstanceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<VKInstanceExtensionHeader*>(ext);
        if (table) {
            auto xres = table->Init(impl, collector);
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

    if (impl.shared_header && impl.shared_header->release() == 1) {
        // Last reference, destroy instance
        std::atomic_thread_fence(std::memory_order_acquire);
        impl.shared_header->header.instance_table.vkDestroyInstance(impl.instance, nullptr);
        delete impl.shared_header;
    }
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
    if (impl.shared_header && impl.shared_header->release() == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        impl.shared_header->header.instance_table.vkDestroyInstance(impl.instance, nullptr);
        delete impl.shared_header;
    }
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

    std::strncpy(desc->description, got_desc.deviceName, sizeof(desc->description) - 1);
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
            header->CollectInfo(collector);
        }
    }

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
    std::unique_ptr<VkQueueFamilyProperties[]> family_props = make_unique<VkQueueFamilyProperties[]>(count);
    if (!family_props) {
        return make_result<Func(), "Not enough memory for queue family properties array">(VK_ERROR_OUT_OF_HOST_MEMORY);
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

    // Create device
    VkDeviceCreateInfo device_create_info{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = feature_structs, // link to extension features
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

    // Fill device impl
    auto& device_impl           = *reinterpret_cast<VKDeviceImpl*>(device);
    device_impl.device_header   = header.release();
    device_impl.device          = device_handle;
    device_impl.instance_header = impl.shared_header;
    device_impl.instance_header->add_ref(); // hold reference to instance header
    device_impl.physical_device = adapter;
    device_impl.instance        = impl.instance;

    // Initialize device extensions
    for (auto* ext : wis::span<WisVKDeviceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<wis::VKDeviceExtensionHeader*>(ext);
        if (table) {
            auto xres = table->Init(device_impl, collector);
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
    if (impl.device_header && impl.device_header->release() == 1) {
        // Last reference, destroy device
        std::atomic_thread_fence(std::memory_order_acquire);
        impl.device_header->header.device_table.vkDestroyDevice(impl.device, nullptr);
        delete impl.device_header;
    }
    impl.device_header = nullptr;
    impl.device        = VK_NULL_HANDLE;
    if (impl.instance_header && impl.instance_header->release() == 1) {
        // Last reference, destroy instance
        std::atomic_thread_fence(std::memory_order_acquire);
        impl.instance_header->header.instance_table.vkDestroyInstance(impl.instance, nullptr);
        delete impl.instance_header;
    }
    impl.instance_header = nullptr;
}

#endif // WIS_VK_INSTANCE_CPP
