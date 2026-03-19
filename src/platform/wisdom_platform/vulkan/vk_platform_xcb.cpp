#ifndef WIS_VK_PLATFORM_XCB_CPP
#define WIS_VK_PLATFORM_XCB_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XCB_PRESENT)
#include <wisdom_platform/generated/cpp_platform_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>

namespace wis::detail {
inline WisResult VKXCBExtensionInit(VKInstanceExtensionHeader*    self,
                                    impl::VKInstanceImpl*         instance_impl,
                                    VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXCBExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto  instance             = instance_impl->instance;
        auto& gtable               = impl.instance_control_block->header.global_table;
        impl.vkCreateXcbSurfaceKHR = gtable.vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKInitXCBExtension(WisVKXCBExtension* self)
{
    new (self) wis::impl::VKXCBExtensionImpl{
        .header                 = { &wis::detail::VKXCBExtensionInit },
        .instance_control_block = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKDestroyXCBExtension(WisVKXCBExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXCBExtensionImpl>(self);
    if (impl.instance_control_block) {
        wis::detail::release_vk_instance(impl.instance_control_block);
    }
}

//-----------------------------------------------------------------------------
WISDOM_PLATFORM_API WisResult wisVKXCBExtensionCreateSurface(WisVKXCBExtension*      self,
                                                             const WisXCBWindowDesc* info,
                                                             WisVKSurface*           surface)
{
    auto& impl                  = wis::from_handle_ref<wis::impl::VKXCBExtensionImpl>(self);
    auto  vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(impl.vkCreateXcbSurfaceKHR);

    VkXcbSurfaceCreateInfoKHR vk_info{
        .sType     = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .pNext     = nullptr,
        .flags     = 0,
        .hinstance = static_cast<xcb_connection_t*>(info->connection),
        .hwnd      = static_cast<xcb_window_t>(info->window),
    };

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    auto         vr         = vkCreateXcbSurfaceKHR(impl.instance_control_block->header.instance,
                                    &vk_info,
                                    nullptr,
                                    &vk_surface);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create XCB surface">(vr);
    }

    new (surface) wis::impl::VKSurfaceImpl{
        .surface         = vk_surface,
        .instance_header = impl.instance_control_block,
    };
    impl.instance_control_block->AddRef(); // Surface holds a reference to the instance

    return wis::detail::vk_success;
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XCB_PRESENT)
#endif // WIS_VK_PLATFORM_XCB_CPP
