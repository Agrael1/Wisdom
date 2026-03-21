#ifndef WIS_VK_PLATFORM_WIN32_CPP
#define WIS_VK_PLATFORM_WIN32_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)
#include <wisdom_platform/generated/backend_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

namespace wis::detail {
inline WisResult VKWin32ExtensionInit(VKInstanceExtensionHeader*    self,
                                      impl::VKInstanceImpl*         instance_impl,
                                      VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWin32ExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    } else {
        // Create control block for instance
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef(); // AddRef instance control block to ensure it lives as long as the extension

        // Collect functions
        auto  instance               = instance_impl->instance;
        auto& gtable                 = impl.instance_control_block->header.global_table;
        impl.vkCreateWin32SurfaceKHR = gtable.vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
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
        wis::detail::VKReleaseInstance(impl.instance_control_block);
    }
}

//-----------------------------------------------------------------------------
WISDOM_PLATFORM_API WisResult wisVKWin32ExtensionCreateSurface(WisVKWin32Extension*      self,
                                                               const WisWin32WindowDesc* info,
                                                               WisVKSurface*             surface)
{
    auto& impl                    = wis::from_handle_ref<wis::impl::VKWin32ExtensionImpl>(self);
    auto  vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(impl.vkCreateWin32SurfaceKHR);

    VkWin32SurfaceCreateInfoKHR vk_info{
        .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext     = nullptr,
        .flags     = 0,
        .hinstance = reinterpret_cast<HINSTANCE>(info->hinstance),
        .hwnd      = reinterpret_cast<HWND>(info->hwnd),
    };

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    auto         vr         = vkCreateWin32SurfaceKHR(impl.instance_control_block->header.instance,
                                      &vk_info,
                                      nullptr,
                                      &vk_surface);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Win32 surface">(vr);
    }

    // create surface header
    auto* header = new (std::nothrow) wis::detail::VKSurfaceControlBlock;
    if (!header) {
        auto& itable = impl.instance_control_block->header.instance_table;
        itable.vkDestroySurfaceKHR(impl.instance_control_block->header.instance, vk_surface, nullptr);
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate surface control block">(VK_ERROR_OUT_OF_HOST_MEMORY);
    }

    header->header.instance_header = impl.instance_control_block,
    header->header.surface         = vk_surface,

    new (surface) wis::impl::VKSurfaceImpl{
        .surface        = vk_surface,
        .surface_header = header,
    };
    impl.instance_control_block->AddRef(); // Surface holds a reference to the instance

    return wis::detail::vk_success;
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WIN32_PRESENT)
#endif // WIS_VK_PLATFORM_WIN32_CPP
