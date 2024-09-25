#ifndef WISDOM_DEBUG_CPP
#define WISDOM_DEBUG_CPP
#include <wisdom/wisdom_debug.h>

#if defined(WISDOM_DX12)
wis::ResultValue<wis::DX12DebugMessenger>
wis::ImplDX12DebugExtension::CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept
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

wis::ResultValue<wis::VKDebugMessenger>
wis::ImplVKDebugExtension::CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept
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
        .pfnUserCallback = DebugCallbackThunk,
        .pUserData = debug_callback.get()
    };

    VkDebugUtilsMessengerEXT messenger;
    auto vr = vkCreateDebugUtilsMessengerEXT(instance.get(), &create_info, nullptr,
                                             &messenger);
    if (!wis::succeeded(vr))
        return wis::make_result<FUNC, "Failed to create debug messenger">(vr);

    return wis::VKDebugMessenger{ instance, messenger, std::move(debug_callback), vkDestroyDebugUtilsMessengerEXT };
}
#endif // WISDOM_VULKAN
#endif // WISDOM_DEBUG_CPP
