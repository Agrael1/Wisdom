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
// hash for VkExtensionProperties
struct VkExtensionPropertiesHash {
    // transparent hash with string_view
    using is_transparent = void;

    std::size_t operator()(const VkExtensionProperties& ext) const noexcept
    {
        return std::hash<std::string_view>{}(ext.extensionName);
    }
    std::size_t operator()(std::string_view name) const noexcept
    {
        return std::hash<std::string_view>{}(name);
    }
};
// equality with VkExtensionProperties and string_view
struct VkExtensionPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkExtensionProperties& ext, std::string_view name) const noexcept
    {
        return std::string_view{ ext.extensionName } == name;
    }
    bool operator()(std::string_view name, const VkExtensionProperties& ext) const noexcept
    {
        return name == std::string_view{ ext.extensionName };
    }
    bool operator()(const VkExtensionProperties& a, const VkExtensionProperties& b) const noexcept
    {
        return std::strcmp(a.extensionName, b.extensionName) == 0;
    }
};

//-----------------------------------------------------------------------------
// struct for VkLayerProperties
struct VkLayerPropertiesHash {
    using is_transparent = void;
    std::size_t operator()(const VkLayerProperties& layer) const noexcept
    {
        return std::hash<std::string_view>{}(layer.layerName);
    }
    std::size_t operator()(std::string_view name) const noexcept
    {
        return std::hash<std::string_view>{}(name);
    }
};
struct VkLayerPropertiesEqual {
    using is_transparent = void;
    bool operator()(const VkLayerProperties& layer, std::string_view name) const noexcept
    {
        return std::string_view{ layer.layerName } == name;
    }
    bool operator()(std::string_view name, const VkLayerProperties& layer) const noexcept
    {
        return name == std::string_view{ layer.layerName };
    }
    bool operator()(const VkLayerProperties& a, const VkLayerProperties& b) const noexcept
    {
        return std::strcmp(a.layerName, b.layerName) == 0;
    }
};

//-----------------------------------------------------------------------------
inline std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual>
GetInstanceExtensions(WisResult& result, const VKMainGlobal& table) noexcept
{
    std::unordered_set<VkExtensionProperties, VkExtensionPropertiesHash, VkExtensionPropertiesEqual> exts;
    // Get available extensions
    uint32_t ext_count = 0;
    VkResult vr        = table.vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
    if (!succeeded(vr)) {
        result = make_result<Func(), "Failed to enumerate Vulkan instance extension properties">(vr);
        return exts;
    }
    std::unique_ptr<VkExtensionProperties[]> ext_props_raw = make_unique<VkExtensionProperties[]>(ext_count);
    if (!ext_props_raw) {
        result = make_result<Func(), "Not enough memory for extension properties">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    vr = table.vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, ext_props_raw.get());

    // Unique set of extensions
    try {
        exts.reserve(ext_count);
    } catch (const std::bad_alloc&) {
        result = make_result<Func(), "Not enough memory for extensions">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return exts;
    }
    for (const auto& i : wis::span{ ext_props_raw.get(), ext_count }) {
        exts.insert(i);
    }
    return exts;
}

inline std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual>
GetInstanceLayers(WisResult& result, const VKMainGlobal& table) noexcept
{
    std::unordered_set<VkLayerProperties, VkLayerPropertiesHash, VkLayerPropertiesEqual> layers;
    // Get available layers
    uint32_t layer_count = 0;
    VkResult vr          = table.vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (!succeeded(vr)) {
        result = make_result<Func(), "Failed to enumerate Vulkan instance layer properties">(vr);
        return layers;
    }
    std::unique_ptr<VkLayerProperties[]> layer_props_raw = make_unique<VkLayerProperties[]>(layer_count);
    if (!layer_props_raw) {
        result = make_result<Func(), "Not enough memory for layer properties">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return layers;
    }
    vr = table.vkEnumerateInstanceLayerProperties(&layer_count, layer_props_raw.get());
    // Unique set of layers
    try {
        layers.reserve(layer_count);
    } catch (const std::bad_alloc&) {
        result = make_result<Func(), "Not enough memory for layers">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return layers;
    }
    for (const auto& i : wis::span{ layer_props_raw.get(), layer_count }) {
        layers.insert(i);
    }
    return layers;
}

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

    // Get available extensions and layers
    auto available_extensions = detail::GetInstanceExtensions(res, gt);
    if (res.status != WisStatusOk) {
        return res;
    }

    auto available_layers = detail::GetInstanceLayers(res, gt);
    if (res.status != WisStatusOk) {
        return res;
    }

    // allocate arrays for enabled extensions and layers (joined allocation)
    size_t                         enabled_extension_count = 0;
    size_t                         enabled_layer_count     = 0;
    std::unique_ptr<const char*[]> enabled_extensions_and_layers_names;

    // Gather extensions
    InstanceExtensionCollector collector{};
    for (size_t i = 0; i < extension_count; ++i) {
        reinterpret_cast<wis::VKInstanceExtensionHeader*>(extensions[i])->CollectInfo(collector);
    }

    // Prepared enabled extensions and layers arrays
    size_t total_enabled_count          = collector.GetEnabledExtensionCount() + collector.GetEnabledLayerCount();
    enabled_extensions_and_layers_names = make_unique<const char*[]>(total_enabled_count);
    if (!enabled_extensions_and_layers_names) {
        return make_result<Func(), "Not enough memory for enabled extensions and layers names">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    // Gather enabled extensions
    const auto& enabled_extension_names_set = collector.GetEnabledExtensionNamesSet();
    for (const auto& name : enabled_extension_names_set) {
        if (available_extensions.find(std::string_view(name)) != available_extensions.end()) {
            enabled_extensions_and_layers_names[enabled_extension_count++] = name;
        }
    }

    // Gather enabled layers
    const auto& enabled_layer_names_set = collector.GetEnabledLayerNamesSet();
    for (const auto& name : enabled_layer_names_set) {
        if (available_layers.find(std::string_view(name)) != available_layers.end()) {
            enabled_extensions_and_layers_names[enabled_extension_count + enabled_layer_count++] = name;
        }
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
        .enabledLayerCount       = static_cast<uint32_t>(enabled_layer_count),
        .ppEnabledLayerNames     = enabled_extensions_and_layers_names.get() + enabled_extension_count,
        .enabledExtensionCount   = static_cast<uint32_t>(enabled_extension_count),
        .ppEnabledExtensionNames = enabled_extensions_and_layers_names.get(),
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
            auto xres = table->Init(impl, wis::span{ enabled_extensions_and_layers_names.get(), enabled_extension_count }, wis::span{ enabled_extensions_and_layers_names.get() + enabled_extension_count, enabled_layer_count });
            if (xres.status != WisStatusOk) {
                res.status = WisStatusPartial; // mark as partial success if any extension fails
            }
        }
    }
    return res;
}

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
        return wis::detail::order_power(a_properties.deviceType) > wis::detail::order_power(b_properties.deviceType)
                ? true
                : a_properties.limits.maxMemoryAllocationCount >
                        b_properties.limits.maxMemoryAllocationCount;
    };
    auto less_performance = [&](std::uintptr_t a, std::uintptr_t b) {
        VkPhysicalDeviceProperties& a_properties = properties_span[a];
        VkPhysicalDeviceProperties& b_properties = properties_span[b];
        return wis::detail::order_performance(a_properties.deviceType) > wis::detail::order_performance(b_properties.deviceType)
                ? true
                : a_properties.limits.maxMemoryAllocationCount >
                        b_properties.limits.maxMemoryAllocationCount;
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

WIS_EXTERN_C WISDOM_API size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self)
{
    return reinterpret_cast<const VKAdapterQueryImpl*>(self)->adapter_count;
}

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

#endif // WIS_VK_INSTANCE_CPP
