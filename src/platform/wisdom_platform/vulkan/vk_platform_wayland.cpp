#ifndef WIS_VK_PLATFORM_WAYLAND_CPP
#define WIS_VK_PLATFORM_WAYLAND_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)
#include <wisdom_platform/generated/cpp_platform_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

#include <vulkan/vulkan_wayland.h>


namespace wis::detail {
struct VKWaylandExtensionTable {
    PFN_vkCreateWaylandSurfaceKHR                        vkCreateWaylandSurfaceKHR;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
};
inline WisResult VKWaylandExtensionInit(VKInstanceExtensionHeader*    self,
                                        impl::VKInstanceImpl*         instance_impl,
                                        VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKWaylandExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_SURFACE_EXTENSION_NAME);
        collector->EnableExtension(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto  instance = instance_impl->instance;
        auto& gtable   = impl.instance_control_block->header.global_table;
        auto* ftable = impl.function_table = new (std::nothrow) wis::detail::VKWaylandExtensionTable;
        if (!ftable) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory to create a function table">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        ftable->vkCreateWaylandSurfaceKHR =
                reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR"));
        ftable->vkGetPhysicalDeviceWaylandPresentationSupportKHR =
                reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));
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

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_WAYLAND_PRESENT)
#endif // WIS_VK_PLATFORM_WAYLAND_CPP
