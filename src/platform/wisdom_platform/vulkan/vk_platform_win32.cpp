#ifndef WIS_VK_PLATFORM_WIN32_CPP
#define WIS_VK_PLATFORM_WIN32_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)
#include <wisdom_platform/generated/cpp_platform_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>


namespace wis::detail {
struct VKWin32ExtensionTable {
    // Function pointers for Win32 surface creation
    PFN_vkCreateWin32SurfaceKHR                        vkCreateWin32SurfaceKHR;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
};
inline WisResult VKWin32ExtensionInit(VKInstanceExtensionHeader*    self,
                                      impl::VKInstanceImpl*         instance_impl,
                                      VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWin32ExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_SURFACE_EXTENSION_NAME);
        collector->EnableExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    } else {
        // Create control block for instance
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef(); // AddRef instance control block to ensure it lives as long as the extension

        // Collect functions
        auto  instance = instance_impl->instance;
        auto& gtable   = impl.instance_control_block->header.global_table;
        auto* ftable = impl.function_table = new(std::nothrow) wis::detail::VKWin32ExtensionTable;
        if (!ftable) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory to create a function table">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }
        ftable->vkCreateWin32SurfaceKHR =
                reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR"));
        ftable->vkGetPhysicalDeviceWin32PresentationSupportKHR =
                reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKInitWin32Extension(WisVKWin32Extension* self)
{
    new (self) wis::impl::VKWin32ExtensionImpl{
        .header                 = { &wis::detail::VKWin32ExtensionInit },
        .instance_control_block = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKDestroyWin32Extension(WisVKWin32Extension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWin32ExtensionImpl>(self);
    if (impl.instance_control_block) {
        wis::detail::release_vk_instance(impl.instance_control_block);
    }
}
#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)
#endif // WIS_VK_PLATFORM_WIN32_CPP
