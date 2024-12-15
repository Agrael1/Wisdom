#ifndef WIS_WISDOM_DEBUG_CPP
#define WIS_WISDOM_DEBUG_CPP
#include <wisdom/wisdom_debug.hpp>

#if defined(WISDOM_DX12)
wis::DX12DebugMessenger
wis::ImplDX12DebugExtension::CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) const noexcept
{
    return wis::DX12DebugMessenger{
        DX12InfoToken{ true }, callback, user_data
    };
}
#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_checks.h>

VKAPI_ATTR VkBool32 VKAPI_CALL wis::ImplVKDebugExtension::DebugCallbackThunk(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept
{
    // Ignore this validation error, until it is fixed in the Vulkan SDK
    if (pCallbackData->pMessageIdName && std::string_view(pCallbackData->pMessageIdName) == "VUID-vkCmdSetDescriptorBufferOffsetsEXT-pOffsets-08063")
        return false;

    auto& [callback, user_data] = *reinterpret_cast<std::pair<wis::DebugCallback, void*>*>(pUserData);

    callback(convert_vk(messageSeverity),
             wis::format("\n[Validation layer]: {}\n [Message]:{}",
                         pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "",
                         pCallbackData->pMessage)
             .c_str(),
             user_data);
    return false;
}

wis::VKDebugMessenger
wis::ImplVKDebugExtension::CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) const noexcept
{
    VKDebugMessenger out_messenger;
    auto& internal = out_messenger.GetMutableInternal();

    internal.data = wis::detail::make_unique<detail::DebugCallbackData>(callback, user_data);
    if (!internal.data) {
        result = wis::make_result<FUNC, "Failed to create debug callback data">(VK_ERROR_OUT_OF_HOST_MEMORY);
        return out_messenger;
    }

    VkDebugUtilsMessengerCreateInfoEXT create_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = DebugCallbackThunk,
        .pUserData = internal.data.get()
    };
    auto vr = vkCreateDebugUtilsMessengerEXT(instance.get(), &create_info, nullptr,
              &internal.messenger);
    if (!wis::succeeded(vr)) {
        result = wis::make_result<FUNC, "Failed to create debug messenger">(vr);
        return out_messenger;
    }
    internal.instance = instance;
    internal.vkDestroyDebugUtilsMessengerEXT = vkDestroyDebugUtilsMessengerEXT;
    return out_messenger;
}
#endif // WISDOM_VULKAN
#endif // WISDOM_DEBUG_CPP
