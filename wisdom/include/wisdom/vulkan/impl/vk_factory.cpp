#ifndef WIS_VK_FACTORY_CPP
#define WIS_VK_FACTORY_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_checks.h>

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <array>
#include <wisdom/global/definitions.h>
#include <wisdom/util/misc.h>
#endif // !WISDOM_MODULE_DECL

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

wis::Result VKFactoryGlobals::InitializeFactoryGlobals() noexcept
{
    if (initialized) {
        return {};
    }

    wis::Result vr = {};

    std::call_once(
            global_flag, [this, &vr]() {
                vr = InitializeGlobalTable();
                if (vr.status != wis::Status::Ok) {
                    return;
                }

                vr = InitializeInstanceExtensions();
                if (vr.status != wis::Status::Ok) {
                    return;
                }

                vr = InitializeInstanceLayers();
                if (vr.status != wis::Status::Ok) {
                    return;
                }
            });

    initialized = true;
    return vr;
}

wis::Result VKFactoryGlobals::InitializeGlobalTable() noexcept
{
    if (!global_table.Init(lib_token)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to initialize global table">(VK_ERROR_INITIALIZATION_FAILED);
    }
    return {};
}

wis::Result VKFactoryGlobals::InitializeInstanceExtensions() noexcept
{
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
    uint32_t count = 0;
    gt.vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    auto extensions = wis::detail::make_fixed_allocation<VkExtensionProperties>(count);
    if (!extensions) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Not enough memory">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    auto vr = gt.vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.get());
    if (!wis::succeeded(vr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to enumerate extensions">(vr);
    }

    // may throw
    instance_extensions.reserve(count);
    for (uint32_t i = 0; i < count; i++) {
        instance_extensions.insert(extensions[i].extensionName);
    }
    return {};
}
wis::Result VKFactoryGlobals::InitializeInstanceLayers() noexcept
{
    auto& gt = wis::detail::VKFactoryGlobals::Instance().global_table;
    uint32_t count = 0;
    gt.vkEnumerateInstanceLayerProperties(&count, nullptr);
    auto layers = wis::detail::make_fixed_allocation<VkLayerProperties>(count);
    if (!layers) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Not enough memory">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    auto vr = gt.vkEnumerateInstanceLayerProperties(&count, layers.get());
    if (!wis::succeeded(vr)) {
        return wis::make_result<wis::Func<wis::FuncD()>(), "Failed to enumerate layers">(vr);
    }

    // may throw
    instance_layers.reserve(count);
    for (uint32_t i = 0; i < count; i++) {
        instance_layers.insert(layers[i].layerName);
    }
    return {};
}
} // namespace wis::detail

wis::VKAdapter
wis::ImplVKFactory::GetAdapter(wis::Result& result, uint32_t index, AdapterPreference preference) const noexcept
{
    VKAdapter out_adapter;
    auto& internal = out_adapter.GetMutableInternal();

    if (index >= adapters.size()) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Index out of range">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_adapter;
    }
    auto& adapter = adapters[index];
    switch (preference) {
    default:
        out_adapter = adapter.adapter;
    case AdapterPreference::MinConsumption:
        out_adapter = adapters[adapter.index_consumption].adapter;
    case AdapterPreference::Performance:
        out_adapter = adapters[adapter.index_performance].adapter;
    }
    return out_adapter;
}

VkResult wis::ImplVKFactory::VKEnumeratePhysicalDevices() noexcept
{
    auto& itable = factory.table();

    std::vector<VkPhysicalDevice> phys_adapters;
    uint32_t count = 0;
    auto vr = factory.table().vkEnumeratePhysicalDevices(factory.get(), &count, nullptr);
    do {
        phys_adapters.resize(count);
    } while ((vr = itable.vkEnumeratePhysicalDevices(factory.get(), &count,
                                                     phys_adapters.data())) == VK_INCOMPLETE);
    if (!wis::succeeded(vr)) {
        return vr;
    }

    adapters.resize(count);

    if (phys_adapters.size() > 1) {
        auto indices = std::views::iota(0u, count);
        std::vector<uint32_t> indices_cons{indices.begin(), indices.end()};
        std::vector<uint32_t> indices_perf{indices.begin(), indices.end()};

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

        for (uint32_t i = 0; i < count; i++) {
            auto& adapter = adapters[i];
            auto& internal = adapter.adapter.GetMutableInternal();

            internal.instance = factory;
            internal.adapter = phys_adapters[i];
            adapter.index_performance = indices_perf[i];
            adapter.index_consumption = indices_cons[i];
        }
    } else {
        for (uint32_t i = 0; i < count; i++) {
            auto& adapter = adapters[i];
            auto& internal = adapter.adapter.GetMutableInternal();

            internal.instance = factory;
            internal.adapter = phys_adapters[i];
            adapter.index_performance = i;
            adapter.index_consumption = i;
        }
    }
    return VK_SUCCESS;
}

#endif // VK_CREATE_FACTORY_CPP
