#ifndef WIS_VK_PLATFORM_WAYLAND_CPP
#define WIS_VK_PLATFORM_WAYLAND_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)
#include <wisdom_platform/generated/cpp_platform_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

#include <vulkan/vulkan_wayland.h>

namespace wis::detail {
inline WisResult VKWaylandExtensionInit(VKInstanceExtensionHeader*    self,
                                        impl::VKInstanceImpl*         instance_impl,
                                        VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWaylandExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto  instance                 = instance_impl->instance;
        auto& gtable                   = impl.instance_control_block->header.global_table;
        impl.vkCreateWaylandSurfaceKHR = gtable.vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKInitWaylandExtension(WisVKWaylandExtension* self)
{
    new (self) wis::impl::VKWaylandExtensionImpl{
        .header                 = { &wis::detail::VKWaylandExtensionInit },
        .instance_control_block = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKDestroyWaylandExtension(WisVKWaylandExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWaylandExtensionImpl>(self);
    if (impl.instance_control_block) {
        wis::detail::release_vk_instance(impl.instance_control_block);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API WisResult wisVKWaylandExtensionCreateSurface(WisVKWaylandExtension*      self,
                                                                              const WisWaylandWindowDesc* info,
                                                                              WisVKSurface*               surface)
{
    auto& impl                      = wis::from_handle_ref<wis::impl::VKWaylandExtensionImpl>(self);
    auto  vkCreateWaylandSurfaceKHR = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(impl.vkCreateWaylandSurfaceKHR);

    VkWaylandSurfaceCreateInfoKHR vk_info{
        .sType   = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
        .pNext   = nullptr,
        .flags   = 0,
        .display = static_cast<struct wl_display*>(info->display),
        .surface = static_cast<struct wl_surface*>(info->surface),
    };

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    auto vr = vkCreateWaylandSurfaceKHR(impl.instance_control_block->header.instance,
                              &vk_info,
                              nullptr,
                                        &vk_surface);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Wayland surface">(vr);
    }

    new (surface) wis::impl::VKSurfaceImpl{ 
        .surface = vk_surface,
        .instance_header = impl.instance_control_block,
    };
    impl.instance_control_block->AddRef(); // Surface holds a reference to the instance

    return wis::detail::vk_success;
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)
#endif // WIS_VK_PLATFORM_WAYLAND_CPP
