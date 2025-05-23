#ifndef WISDOM_DEBUG_H
#define WISDOM_DEBUG_H
#if defined(WISDOM_DX12)
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_factory_ext.h>
#include <wisdom/dx12/dx12_debug.h>
#include <wisdom/dx12/dx12_checks.h>
#include <d3d12.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class DX12DebugExtension;

template<>
struct Internal<DX12DebugExtension> {
};

class ImplDX12DebugExtension : public QueryInternalExtension<DX12DebugExtension, wis::DX12FactoryExtension>
{
public:
    [[nodiscard]] bool Supported() const noexcept override
    {
        return true;
    }
    wis::Result Init(const wis::DX12Factory& in_instance) noexcept override
    {
        wis::com_ptr<ID3D12Debug> debugController;
        if (wis::succeeded(
                    D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void()))) {
            debugController->EnableDebugLayer();
        }

        if (!debugController) {
            return wis::make_result<wis::Func<wis::FuncD()>(), "Debug Extension is unsupported">(E_NOTIMPL);
        }

        // if (auto dc = debugController.as<ID3D12Debug1>())
        //     dc->SetEnableGPUBasedValidation(true);

        return {};
    }

public:
    [[nodiscard]] WIS_INLINE DX12DebugMessenger
    CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) const noexcept;
};

#pragma region DX12DebugExtension

WISDOM_EXPORT
class DX12DebugExtension : public wis::ImplDX12DebugExtension
{
public:
    using wis::ImplDX12DebugExtension::ImplDX12DebugExtension;

public:
    /**
     * @brief Creates a debug messenger for the factory.
     * @param callback The callback that will receive the debug messages.
     * @param user_data The user data that will be passed to the callback.
     * @return wis::DX12DebugMessenger on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::DX12DebugMessenger CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) noexcept
    {
        return wis::ImplDX12DebugExtension::CreateDebugMessenger(result, callback, user_data);
    }
    /**
     * @brief Creates a debug messenger for the factory.
     * @param callback The callback that will receive the debug messages.
     * @param user_data The user data that will be passed to the callback.
     * @return wis::DX12DebugMessenger on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::DX12DebugMessenger> CreateDebugMessenger(wis::DebugCallback callback, void* user_data) noexcept
    {
        return wis::ResultValue<wis::DX12DebugMessenger>{ &wis::ImplDX12DebugExtension::CreateDebugMessenger, this, callback, user_data };
    }
};
#pragma endregion DX12DebugExtension

} // namespace wis

#endif // WISDOM_DX12

#if defined(WISDOM_VULKAN)
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_factory_ext.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class VKDebugExtension;

template<>
struct Internal<VKDebugExtension> {
    wis::SharedInstance instance;
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
};

class ImplVKDebugExtension : public QueryInternalExtension<VKDebugExtension, wis::VKFactoryExtensionImpl<VKDebugExtension>>
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
        "VK_LAYER_KHRONOS_validation",
        //"VK_LAYER_KHRONOS_synchronization2",
        //"VK_LAYER_LUNARG_monitor"
    };

    [[nodiscard]] wis::Result
    Init(const wis::VKFactory& in_instance) noexcept override
    {
        instance = in_instance.GetInternal().factory;

        vkCreateDebugUtilsMessengerEXT = instance.GetInstanceProcAddr<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
        vkDestroyDebugUtilsMessengerEXT = instance.GetInstanceProcAddr<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");
        return {};
    }

    [[nodiscard]] bool
    Supported() const noexcept override
    {
        return vkCreateDebugUtilsMessengerEXT && vkDestroyDebugUtilsMessengerEXT;
    }

public:
    [[nodiscard]] WIS_INLINE VKDebugMessenger
    CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) const noexcept;
};

#pragma region VKDebugExtension

WISDOM_EXPORT
class VKDebugExtension : public wis::ImplVKDebugExtension
{
public:
    using wis::ImplVKDebugExtension::ImplVKDebugExtension;

public:
    /**
     * @brief Creates a debug messenger for the factory.
     * @param callback The callback that will receive the debug messages.
     * @param user_data The user data that will be passed to the callback.
     * @return wis::VKDebugMessenger on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::VKDebugMessenger CreateDebugMessenger(wis::Result& result, wis::DebugCallback callback, void* user_data) noexcept
    {
        return wis::ImplVKDebugExtension::CreateDebugMessenger(result, callback, user_data);
    }
    /**
     * @brief Creates a debug messenger for the factory.
     * @param callback The callback that will receive the debug messages.
     * @param user_data The user data that will be passed to the callback.
     * @return wis::VKDebugMessenger on success (wis::Status::Ok).
     * */
    [[nodiscard]] inline wis::ResultValue<wis::VKDebugMessenger> CreateDebugMessenger(wis::DebugCallback callback, void* user_data) noexcept
    {
        return wis::ResultValue<wis::VKDebugMessenger>{ &wis::ImplVKDebugExtension::CreateDebugMessenger, this, callback, user_data };
    }
};
#pragma endregion VKDebugExtension
} // namespace wis
#endif // WISDOM_VULKAN

WISDOM_EXPORT
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
