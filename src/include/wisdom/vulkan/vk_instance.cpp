#ifndef WIS_VK_INSTANCE_CPP
#define WIS_VK_INSTANCE_CPP

#include <wisdom/generated/cpp_api.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

#include <algorithm>

namespace wis::detail {
//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Orders Vulkan physical device types by their expected performance, with discrete GPUs being the highest and
 * CPUs being the lowest.
 * @param t The type of the physical device
 * @return A numeric value representing the performance of the device type, higher is better
 */
constexpr uint32_t order_performance(const VkPhysicalDeviceType t)
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

//----------------------------------------------------------------------------------------------------------------------
/**
 * @brief Orders Vulkan physical device types by their expected power efficiency, with integrated GPUs being the most
 * power efficient and discrete GPUs being the least.
 * @param t The type of the physical device
 * @return A numeric value representing the power efficiency of the device type, higher is more power efficient
 */
constexpr uint32_t order_power(VkPhysicalDeviceType t)
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

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCreateInstance(
    const WisDebugDesc* debug_desc,
    WisVKInstanceExtensionHeader** extensions,
    size_t extension_count,
    WisVKInstance* instance
)
{
    auto header = wis::make_unique<wis::detail::VKInstanceControlBlock>();
    if (!header) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate memory for Vulkan instance header">(
            VK_ERROR_OUT_OF_HOST_MEMORY
        );
    }

    header->header.library = wis::detail::unique_library{wis::detail::InitializeVulkanLibrary()};
    if (!header->header.library) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to load Vulkan library">(VK_ERROR_UNKNOWN);
    }

    if (!header->header.global_table.Init(header->header.library.get())) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan global function table">(
            VK_ERROR_UNKNOWN
        );
    }

    const auto& gt = header->header.global_table;

    VkResult vr = VK_SUCCESS;
    uint32_t version = 0;
    if (!gt.vkEnumerateInstanceVersion) {
        return wis::detail::make_result<wis::detail::Func(), "The Vulkan version is too old!">(VK_ERROR_UNKNOWN);
    }

    // Get Vulkan instance version
    vr = gt.vkEnumerateInstanceVersion(&version);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to enumerate Vulkan instance version">(vr);
    }

    // Gather extensions
    WisResult res = wis::detail::vk_success;
    wis::VKInstanceExtensionCollector collector{gt, res};
    if (res.status != WisStatusOk) {
        return res;
    }

    // Let extensions collect their info
    for (size_t i = 0; i < extension_count; ++i) {
        auto* ext_header = wis::from_handle<wis::VKInstanceExtensionHeader>(extensions[i]);
        if (!ext_header || !ext_header->init_fptr) {
            continue; // skip invalid extension headers
        }

        auto res2 = ext_header->init_fptr(ext_header, nullptr, &collector);
        // Non-fatal, allow to silently fail
        (void)res2;
    }

    // Setup debug layer if requested
    if (debug_desc && debug_desc->enable_debug_layer) {
        collector.EnableLayer("VK_LAYER_KHRONOS_validation");
        collector.EnableExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // Prepared enabled extensions and layers arrays
    auto&& [ext_layer_array, ext_count, layer_count] = collector.GetExtensionsAndLayers(res);
    if (res.status != WisStatusOk) {
        return res;
    }

    // Prepare debug callback thunk
    std::unique_ptr<wis::detail::VKDebugCallbackThunk> debug_layer_thunk;
    if (debug_desc && debug_desc->callback) {
        debug_layer_thunk = wis::make_unique<wis::detail::VKDebugCallbackThunk>();
        if (debug_layer_thunk) {
            debug_layer_thunk->callback = debug_desc->callback;
            debug_layer_thunk->user_data = debug_desc->user_data;
        }
        // Non-fatal, allow to silently fail
    }

    // Create Vulkan instance
    VkApplicationInfo info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = version,
    };
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                     | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = wis::detail::VKDebugCallbackThunk::DebugUtilsMessengerCallbackThunk,
        .pUserData = debug_layer_thunk.get(),
    };
    const VkInstanceCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = debug_layer_thunk ? &debug_create_info : nullptr,
        .flags = 0,
        .pApplicationInfo = &info,
        .enabledLayerCount = static_cast<uint32_t>(layer_count),
        .ppEnabledLayerNames = layer_count ? ext_layer_array.get() + ext_count : nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(ext_count),
        .ppEnabledExtensionNames = ext_layer_array.get(),
    };

    VkInstance instance_handle = VK_NULL_HANDLE;
    vr = gt.vkCreateInstance(&create_info, nullptr, &instance_handle);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Vulkan instance">(vr);
    }

    // Initialize instance table
    auto& instance_table = header->header.instance_table;
    if (!instance_table.Init(instance_handle, gt.vkGetInstanceProcAddr)) {
        instance_table.vkDestroyInstance(instance_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan instance function table">(
            VK_ERROR_UNKNOWN
        );
    }

    // Initialize adapter table
    if (!header->header.adapter_table.Init(instance_handle, gt.vkGetInstanceProcAddr)) {
        instance_table.vkDestroyInstance(instance_handle, nullptr); // cleanup
        return wis::detail::make_result<wis::detail::Func(), "Failed to initialize Vulkan adapter function table">(
            VK_ERROR_UNKNOWN
        );
    }

    // Setup debug messenger if requested
    if (debug_layer_thunk && instance_table.vkCreateDebugUtilsMessengerEXT) {
        auto vr2 = instance_table.vkCreateDebugUtilsMessengerEXT(
            instance_handle,
            &debug_create_info,
            nullptr,
            &header->header.debug_messenger
        );
        // Non-fatal, allow to silently fail
        (void)vr2;
    }

    // Store debug thunk
    header->header.instance = instance_handle;
    header->header.debug_callback_thunk = std::move(debug_layer_thunk);
    header->header.api_version = version;

    // Fill instance impl
    auto& impl = *new (instance) wis::impl::VKInstanceImpl{
        .instance = instance_handle,
        .shared_header = header.release(),
    };

    // Initialize instance extensions
    for (auto* ext : wis::span<WisVKInstanceExtensionHeader*>{extensions, extension_count}) {
        if (auto* table = wis::from_handle<wis::VKInstanceExtensionHeader>(ext); table && table->init_fptr) {
            if (auto xres = table->init_fptr(table, &impl, &collector); xres.status != WisStatusOk) {
                res.status = WisStatusPartial; // mark as partial success if any extension fails
                res.error = xres.error;
                res.platform_code = xres.platform_code;
            }
        }
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyInstance(WisVKInstance* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKInstanceImpl>(self);
    if (!impl.instance) {
        return;
    }

    wis::detail::VKReleaseInstance(impl.shared_header);
    impl.shared_header = nullptr;
    impl.instance = VK_NULL_HANDLE;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult
wisVKInstanceQueryAdapters(const WisVKInstance* self, WisAdapterPreference preference, WisVKAdapterQuery* query)
{
    // Query can come as partially constructed from C side
    auto& instance_impl = wis::from_handle_ref<const wis::impl::VKInstanceImpl>(self);

    const auto& header = *instance_impl.shared_header;
    const auto& table = header.header.instance_table;
    const auto& atable = header.header.adapter_table;

    std::unique_ptr<VkPhysicalDevice[]> devices_ref;

    std::unique_ptr<std::byte[]> aux_pool;

    // Get number of physical devices
    uint32_t device_count = 0;
    VkResult vr = table.vkEnumeratePhysicalDevices(instance_impl.instance, &device_count, nullptr);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to enumerate Vulkan physical devices">(vr);
    }
    if (device_count == 0) {
        return wis::detail::make_result<wis::detail::Func(), "No Vulkan physical devices found">(
            VK_ERROR_INITIALIZATION_FAILED
        );
    }

    // Get physical devices
    devices_ref = wis::make_unique<VkPhysicalDevice[]>(device_count);
    if (!devices_ref) {
        return wis::detail::make_result<wis::detail::Func(), "Not enough memory for physical devices array">(
            VK_ERROR_OUT_OF_HOST_MEMORY
        );
    }

    vr = table.vkEnumeratePhysicalDevices(instance_impl.instance, &device_count, devices_ref.get());
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to enumerate Vulkan physical devices">(vr);
    }

    if (preference == WisAdapterPreference::WisAdapterPreferenceNone) {
        // No sorting needed
        auto& impl = *new (query) wis::impl::VKAdapterQueryImpl();
        impl.adapter_count = device_count;
        impl.physical_devices = devices_ref.release();
        impl.instance = instance_impl.instance;
        impl.shared_header = instance_impl.shared_header;
        impl.shared_header->AddRef(); // hold reference to instance header
        return wis::detail::vk_success;
    }

    // Sort devices based on preference
    constexpr static std::size_t max_align = std::max(alignof(VkPhysicalDeviceProperties), alignof(std::uintptr_t));
    std::size_t total_aux_size = sizeof(VkPhysicalDeviceProperties) * device_count
                               + device_count * sizeof(std::uintptr_t);

    aux_pool = wis::make_unique<std::byte[]>(total_aux_size + max_align - 1);
    if (!aux_pool) {
        return wis::detail::make_result<wis::detail::Func(), "Not enough memory for auxiliary sorting buffer">(
            VK_ERROR_OUT_OF_HOST_MEMORY
        );
    }

    // Aligned pointers
    auto* aux_ptr = wis::aligned_address(aux_pool.get(), max_align);
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
        auto a_order = wis::detail::order_power(a_properties.deviceType);
        auto b_order = wis::detail::order_power(b_properties.deviceType);
        if (a_order != b_order) {
            return a_order > b_order;
        }
        return a_properties.limits.maxMemoryAllocationCount > b_properties.limits.maxMemoryAllocationCount;
    };
    auto less_performance = [&](std::uintptr_t a, std::uintptr_t b) {
        VkPhysicalDeviceProperties& a_properties = properties_span[a];
        VkPhysicalDeviceProperties& b_properties = properties_span[b];
        auto a_order = wis::detail::order_performance(a_properties.deviceType);
        auto b_order = wis::detail::order_performance(b_properties.deviceType);
        if (a_order != b_order) {
            return a_order > b_order;
        }
        return a_properties.limits.maxMemoryAllocationCount > b_properties.limits.maxMemoryAllocationCount;
    };

    // Sort indices based on preference
    switch (preference) {
    case WisAdapterPreference::WisAdapterPreferenceMinConsumption:
        std::ranges::sort(index_span, [&](std::uintptr_t a, std::uintptr_t b) { return less_consumption(a, b); });
        break;
    case WisAdapterPreference::WisAdapterPreferencePerformance:
        std::ranges::sort(index_span, [&](std::uintptr_t a, std::uintptr_t b) { return less_performance(a, b); });
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
    // Copy back in sorted order
    for (std::size_t i = 0; i < device_count; ++i) {
        devices_ref[i] = ptr_span[i];
    }

    // Fill query impl
    auto& impl = *new (query) wis::impl::VKAdapterQueryImpl{
        .physical_devices = devices_ref.release(),
        .adapter_count = device_count,
        .instance = instance_impl.instance,
        .shared_header = instance_impl.shared_header,
    };

    impl.shared_header->AddRef(); // hold reference to instance header
    return wis::detail::vk_success;
}

#endif // !WIS_VK_INSTANCE_CPP
