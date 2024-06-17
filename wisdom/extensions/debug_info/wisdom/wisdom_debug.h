#ifndef WISDOM_DEBUG_H
#define WISDOM_DEBUG_H
#if defined(WISDOM_DX12)
#include <wisdom/dx12/dx12_factory_ext.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>

namespace wis {
class DX12DebugExtension;

template<>
struct Internal<DX12DebugExtension> {
};

class DX12DebugExtension : public QueryInternalExtension<DX12DebugExtension, DX12FactoryExtension>
{
public:
    [[nodiscard]] bool Supported() const noexcept override
    {
        wis::com_ptr<ID3D12Debug> debugController;
        return (wis::succeeded(
                D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())));
    }
    wis::Result Init(const wis::DX12Factory& in_instance) noexcept override
    {
        wis::com_ptr<ID3D12Debug> debugController;
        if (wis::succeeded(
                    D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
            debugController->EnableDebugLayer();

        if (!debugController)
            return wis::make_result<FUNC, "Debug Extension is unsupported">(E_NOTIMPL);

        if (auto dc = debugController.as<ID3D12Debug1>())
            dc->SetEnableGPUBasedValidation(true);

        return {};
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<DX12DebugMessenger>
    CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept;
};
} // namespace wis

#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_factory_ext.h>

namespace wis {
class VKDebugExtension;

template<>
struct Internal<VKDebugExtension> {
    wis::SharedInstance instance;
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
};

class VKDebugExtension : public QueryInternalExtension<VKDebugExtension, wis::VKFactoryExtensionImpl<VKDebugExtension>>
{
    static WIS_INLINE VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackThunk(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) noexcept;

public:
    static constexpr std::array required_extensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    static constexpr std::array required_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    [[nodiscard]] bool Supported() const noexcept override
    {
        return detail::VKFactoryGlobals::Instance().instance_extensions.contains(VK_EXT_DEBUG_UTILS_EXTENSION_NAME) &&
                detail::VKFactoryGlobals::Instance().instance_layers.contains("VK_LAYER_KHRONOS_validation");
    }

    [[nodiscard]] wis::Result Init(const wis::VKFactory& in_instance) noexcept override
    {
        auto& gt = detail::VKFactoryGlobals::Instance().global_table;
        instance = in_instance.GetInternal().factory;

        vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(gt.vkGetInstanceProcAddr(instance.get(), "vkCreateDebugUtilsMessengerEXT"));
        vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(gt.vkGetInstanceProcAddr(instance.get(), "vkDestroyDebugUtilsMessengerEXT"));

        if (!vkCreateDebugUtilsMessengerEXT || !vkDestroyDebugUtilsMessengerEXT) {
            return wis::make_result<FUNC, "Failed to load Debug extension functions">(VK_ERROR_FEATURE_NOT_PRESENT);
        }
        return {};
    }

public:
    [[nodiscard]] WIS_INLINE wis::ResultValue<VKDebugMessenger>
    CreateDebugMessenger(wis::DebugCallback callback, void* user_data) const noexcept;
};
} // namespace wis
#endif // WISDOM_VULKAN

namespace wis {
#if defined(WISDOM_DX12) && !defined(WISDOM_FORCE_VULKAN)
using DebugExtension = DX12DebugExtension;
#elif defined(WISDOM_VULKAN)
using DebugExtension = VKDebugExtension;
#endif // WISDOM_DX12
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "wisdom_debug.cpp"
#endif // !WISDOM_PLATFORM_HEADER_ONLY
#endif // WISDOM_DEBUG_H
