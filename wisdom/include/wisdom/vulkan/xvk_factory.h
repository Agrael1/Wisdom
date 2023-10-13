#pragma once
#include <wisvk/vk_managed_handles.hpp>
#include <wisvk/vk_loader.hpp>
#include <wisdom/api/internal.h>
#include <wisdom/api/consts.h>
#include <wisdom/vulkan/xvk_checks.h>
#include <wisdom/util/log_layer.h>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>

#include <mutex>
#include <vector>

namespace wis {
class VKFactory;

template<>
struct Internal<VKFactory> {
    wis::shared_handle<VkInstance> factory;
    VkDebugUtilsMessengerEXT messenger{};
    uint32_t api_version{};
    std::unique_ptr<wis::VkInstanceTable> instance_table{};

public:
    static inline wis::LibToken lib_token;
    static inline wis::VkGlobalTable global_table{};
    static inline std::once_flag global_flag;
};

inline std::pair<wis::Result, wis::VKFactory> VKCreateFactory(bool debug_layer = false, wis::DebugCallback callback = nullptr, void* user_data = nullptr) noexcept;

class VKFactory : public QueryInternal<VKFactory>
{
    friend std::pair<wis::Result, wis::VKFactory> VKCreateFactory(bool, wis::DebugCallback, void*) noexcept;
    static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) noexcept
    {
        auto& [callback, user_data] = *reinterpret_cast<std::pair<wis::DebugCallback, void*>*>(pUserData);
        callback(convert(messageSeverity),
                 wis::format("\n[Validation layer]: {}\n [Message]:{}", pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "", pCallbackData->pMessage).c_str(), user_data);
        return false;
    }

public:
    VKFactory() noexcept = default;
    VKFactory(wis::shared_handle<VkInstance> instance, uint32_t api_ver, bool debug_layer = false, std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback = {}) noexcept
        : QueryInternal(std::move(instance))
    {
        instance_table = std::make_unique<wis::VkInstanceTable>();
        api_version = api_ver;
        instance_table->Init(factory.get(), global_table);
        if constexpr (wis::debug_layer) {
            if (debug_layer && debug_callback) {
                EnableDebugLayer(std::move(debug_callback));
            }
        }
    }
    ~VKFactory() noexcept
    {
        if (debug_callback) {
            instance_table->vkDestroyDebugUtilsMessengerEXT(factory.get(), messenger, nullptr);
            factory.reset();
        }
    }
    VKFactory(const VKFactory&) = delete;
    VKFactory(VKFactory&&) noexcept = default;
    VKFactory& operator=(const VKFactory&) = delete;
    VKFactory& operator=(VKFactory&&) noexcept = default;

private:
    void EnableDebugLayer(std::unique_ptr<std::pair<wis::DebugCallback, void*>> xdebug_callback) noexcept
    {
        debug_callback = std::move(xdebug_callback);
        VkDebugUtilsMessengerCreateInfoEXT create_info{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = VKFactory::DebugCallbackThunk,
            .pUserData = debug_callback.get()
        };

        auto vr = instance_table->vkCreateDebugUtilsMessengerEXT(factory.get(), &create_info, nullptr, &messenger);
        if (!wis::succeeded(vr)) {
            wis::lib_error("Failed to create debug messenger");
        }
    }
    static void InitializeGlobalTable() noexcept
    {
        std::call_once(global_flag, []() {
            global_table.Init(lib_token);
        });
    }
    static WIS_INLINE std::vector<const char*> FoundExtensions() noexcept;
    static WIS_INLINE std::vector<const char*> FoundLayers() noexcept;

private:
    std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback{};
};
} // namespace wis

std::pair<wis::Result, wis::VKFactory> wis::VKCreateFactory(bool debug_layer, wis::DebugCallback callback, void* user_data) noexcept
{
    VKFactory::InitializeGlobalTable();
    auto& gt = VKFactory::global_table;

    uint32_t version = 0;
    if (!wis::succeeded(gt.vkEnumerateInstanceVersion(&version)))
        return std::pair{ wis::Result{ wis::Status::Error, "Failed to enumerate vulkan instance version" }, wis::VKFactory{} };

    wis::lib_info(wis::format("Vulkan version: {}.{}.{}", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version)));

    VkApplicationInfo info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        nullptr,
        "",
        VK_MAKE_API_VERSION(0, 1, 0, 0),
        "",
        VK_MAKE_API_VERSION(0, 1, 0, 0),
        version
    };

    auto found_extension = VKFactory::FoundExtensions();
    auto found_layers = VKFactory::FoundLayers();

    VkInstanceCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &info,
        .enabledLayerCount = static_cast<uint32_t>(found_layers.size()),
        .ppEnabledLayerNames = found_layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(found_extension.size()),
        .ppEnabledExtensionNames = found_extension.data()
    };

    std::unique_ptr<std::pair<wis::DebugCallback, void*>> debug_callback = std::make_unique<std::pair<wis::DebugCallback, void*>>(callback, user_data);

    VkDebugUtilsMessengerCreateInfoEXT create_instance_debug{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = VKFactory::DebugCallbackThunk,
        .pUserData = debug_callback.get()
    };

    if (debug_layer) {
        create_info.pNext = &create_instance_debug;
    }

    wis::shared_handle<VkInstance> instance;
    auto vr = gt.vkCreateInstance(&create_info, nullptr, instance.put_unsafe(gt.vkDestroyInstance));
    return !wis::succeeded(vr)
            ? std::pair{ wis::Result{ wis::Status::Error, "Failed to create instance" }, wis::VKFactory{} }
            : std::pair{ wis::success, VKFactory{ std::move(instance), version, debug_layer, std::move(debug_callback) } };
}

#define VKGT wis::Internal<wis::VKFactory>::global_table