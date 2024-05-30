#ifndef VK_CREATE_FACTORY_CPP
#define VK_CREATE_FACTORY_CPP
#include <wisdom/vulkan/vk_factory.h>

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <array>
#include <wisdom/global/definitions.h>
#include <wisdom/util/misc.h>

namespace wis::detail {
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

VKAPI_ATTR VkBool32 VKAPI_CALL wis::VKFactory::DebugCallbackThunk(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept
{
    auto& [callback, user_data] = *reinterpret_cast<std::pair<wis::DebugCallback, void*>*>(pUserData);
    callback(convert_vk(messageSeverity),
             wis::format("\n[Validation layer]: {}\n [Message]:{}",
                         pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "",
                         pCallbackData->pMessage)
                     .c_str(),
             user_data);
    return false;
}

wis::VKFactory::VKFactory(
        wis::SharedInstance instance, uint32_t api_ver, bool debug) noexcept
    : QueryInternal(std::move(instance), api_ver, debug)
{
}

wis::ResultValue<wis::VKAdapter>
wis::VKFactory::GetAdapter(uint32_t index, AdapterPreference preference) const noexcept
{
    if (index >= adapters.size()) {
        return wis::make_result<FUNC, "Index out of range">(VK_ERROR_UNKNOWN);
    }
    auto& adapter = adapters[index];
    switch (preference) {
    default:
        return adapter.adapter;
    case AdapterPreference::MinConsumption:
        return adapters[adapter.index_consumption].adapter;
    case AdapterPreference::Performance:
        return adapters[adapter.index_performance].adapter;
    }
}

wis::ResultValue<wis::VKDebugMessenger>
wis::VKFactory::CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept
{
    auto debug_callback = wis::detail::make_unique<detail::DebugCallbackData>(callback, user_data);
    if (!debug_callback)
        return wis::make_result<FUNC, "Failed to create debug callback data">(VK_ERROR_OUT_OF_HOST_MEMORY);

    VkDebugUtilsMessengerCreateInfoEXT create_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = VKFactory::DebugCallbackThunk,
        .pUserData = debug_callback.get()
    };

    VkDebugUtilsMessengerEXT messenger;
    auto vr = factory.table().vkCreateDebugUtilsMessengerEXT(factory.get(), &create_info, nullptr,
                                                             &messenger);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to create debug messenger">(vr);

    return wis::VKDebugMessenger{ factory, messenger, std::move(debug_callback) };
}

VkResult wis::VKFactory::EnumeratePhysicalDevices() noexcept
{
    auto& itable = factory.table();

    std::vector<VkPhysicalDevice> phys_adapters;
    uint32_t count = 0;
    auto vr = factory.table().vkEnumeratePhysicalDevices(factory.get(), &count, nullptr);
    do
        phys_adapters.resize(count);
    while ((vr = itable.vkEnumeratePhysicalDevices(factory.get(), &count,
                                                   phys_adapters.data())) == VK_INCOMPLETE);
    if (!wis::succeeded(vr))
        return vr;

    adapters.resize(count);

    if (phys_adapters.size() > 1) {
        auto indices = std::views::iota(0u, count);
        std::vector<uint32_t> indices_cons(indices.begin(), indices.end());
        std::vector<uint32_t> indices_perf(indices.begin(), indices.end());

        auto less_consumption = [this](VkPhysicalDevice a, VkPhysicalDevice b) {
            auto& itable = factory.table();
            VkPhysicalDeviceProperties a_properties{};
            VkPhysicalDeviceProperties b_properties{};
            itable.vkGetPhysicalDeviceProperties(a, &a_properties);
            itable.vkGetPhysicalDeviceProperties(b, &b_properties);

            return wis::detail::order_power(a_properties.deviceType) > wis::detail::order_power(b_properties.deviceType)
                    ? true
                    : a_properties.limits.maxMemoryAllocationCount >
                            b_properties.limits.maxMemoryAllocationCount;
        };
        auto less_performance = [this](VkPhysicalDevice a, VkPhysicalDevice b) {
            auto& itable = factory.table();
            VkPhysicalDeviceProperties a_properties{};
            VkPhysicalDeviceProperties b_properties{};
            itable.vkGetPhysicalDeviceProperties(a, &a_properties);
            itable.vkGetPhysicalDeviceProperties(b, &b_properties);

            return wis::detail::order_performance(a_properties.deviceType) > wis::detail::order_performance(b_properties.deviceType)
                    ? true
                    : a_properties.limits.maxMemoryAllocationCount >
                            b_properties.limits.maxMemoryAllocationCount;
        };

        std::ranges::sort(indices_cons,
                          [this, &phys_adapters, less_consumption](uint32_t a, uint32_t b) {
                              return less_consumption(phys_adapters[a], phys_adapters[b]);
                          });
        std::ranges::sort(indices_perf,
                          [this, &phys_adapters, less_consumption](uint32_t a, uint32_t b) {
                              return less_consumption(phys_adapters[a], phys_adapters[b]);
                          });

        for (size_t i = 0; i < count; i++) {
            auto& adapter = adapters[i];
            adapter.adapter = VKAdapter{
                factory, phys_adapters[i]
            };
            adapter.index_performance = indices_perf[i];
            adapter.index_consumption = indices_cons[i];
        }
    } else {
        for (size_t i = 0; i < count; i++) {
            auto& adapter = adapters[i];
            adapter.adapter = VKAdapter{
                factory, phys_adapters[i]
            };
            adapter.index_performance = i;
            adapter.index_consumption = i;
        }
    }
    return VK_SUCCESS;
}

#endif // VK_CREATE_FACTORY_CPP
