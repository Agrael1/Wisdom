#ifndef WIS_VK_PLATFORM_XLIB_CPP
#define WIS_VK_PLATFORM_XLIB_CPP

#if defined(WISDOM_VULKAN)
#    include <wisdom/vulkan/detail/vk_detail.hpp>
#    include <wisdom/vulkan/detail/vk_utils.hpp>
#    include <wisdom/vulkan/vk_extensions.hpp>
#    include <wisdom_platform/generated/cpp_api.hpp>

// manual definitions of xlib, because xlib is really polluting.
typedef unsigned long XID;
typedef XID Window;
typedef unsigned long VisualID;
typedef struct _XDisplay Display;
#    include <vulkan/vulkan_xlib.h>

namespace wis::detail {
inline WisResult VKXlibExtensionInit(
    VKInstanceExtensionHeader* self,
    impl::VKInstanceImpl* instance_impl,
    VKInstanceExtensionCollector* collector
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto instance = instance_impl->instance;
        auto& gtable = impl.instance_control_block->header.global_table;
        impl.vkCreateXlibSurfaceKHR = gtable.vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKInitXlibExtension(WisVKXlibExtension* self)
{
    new (self) wis::impl::VKXlibExtensionImpl{
        .header = {&wis::detail::VKXlibExtensionInit},
        .instance_control_block = nullptr,
        .vkCreateXlibSurfaceKHR = nullptr,
    };
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKDestroyXlibExtension(WisVKXlibExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);
    if (impl.instance_control_block) {
        wis::detail::VKReleaseInstance(impl.instance_control_block);
    }
}

//----------------------------------------------------------------------------------------------------------------------
WISDOM_PLATFORM_API WisResult wisVKXlibExtensionCreateSurface(
    WisVKXlibExtension* self,
    const WisXlibWindowDesc* info,
    WisVKSurface* surface
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);
    auto vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(impl.vkCreateXlibSurfaceKHR);

    VkXlibSurfaceCreateInfoKHR vk_info{
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .dpy = static_cast<Display*>(info->display),
        .window = static_cast<Window>(info->window),
    };

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    auto vr = vkCreateXlibSurfaceKHR(impl.instance_control_block->header.instance, &vk_info, nullptr, &vk_surface);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create Xlib surface">(vr);
    }

    auto* header = new (std::nothrow) wis::detail::VKSurfaceControlBlock;
    if (!header) {
        auto& itable = impl.instance_control_block->header.instance_table;
        itable.vkDestroySurfaceKHR(impl.instance_control_block->header.instance, vk_surface, nullptr);
        return wis::detail::make_result<wis::detail::Func(), "Failed to allocate surface control block">(
            VK_ERROR_OUT_OF_HOST_MEMORY
        );
    }

    header->header.instance_header = impl.instance_control_block, header->header.surface = vk_surface,

    new (surface) wis::impl::VKSurfaceImpl{
        .surface = vk_surface,
        .surface_header = header,
    };
    impl.instance_control_block->AddRef(); // Surface holds a reference to the instance

    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API bool wisVKXlibExtensionSupported(WisVKXlibExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);
    return impl.vkCreateXlibSurfaceKHR != nullptr;
}

#endif // defined(WISDOM_VULKAN)
#endif // WIS_VK_PLATFORM_XLIB_CPP
