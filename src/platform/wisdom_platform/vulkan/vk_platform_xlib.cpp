#ifndef WIS_VK_PLATFORM_XLIB_CPP
#define WIS_VK_PLATFORM_XLIB_CPP

#if defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XLIB_PRESENT)
#include <wisdom_platform/generated/cpp_platform_api.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>

#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

// Undefine common macros that may interfere with Vulkan function pointer declarations
#undef Bool
#undef Status
#undef True
#undef False
#undef None
#undef Always


namespace wis::detail {
struct VKXlibExtensionTable {
    PFN_vkCreateXlibSurfaceKHR                        vkCreateXlibSurfaceKHR;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
};

inline WisResult VKXlibExtensionInit(VKInstanceExtensionHeader*    self,
                                     impl::VKInstanceImpl*         instance_impl,
                                     VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_SURFACE_EXTENSION_NAME);
        collector->EnableExtension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto  instance = instance_impl->instance;
        auto& gtable   = impl.instance_control_block->header.global_table;
        auto* ftable = impl.function_table = new (std::nothrow) wis::detail::VKXlibExtensionTable;
        if (!ftable) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory to create a function table">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        ftable->vkCreateXlibSurfaceKHR =
                reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR"));
        ftable->vkGetPhysicalDeviceXlibPresentationSupportKHR =
                reinterpret_cast<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR"));
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKInitXlibExtension(WisVKXlibExtension* self)
{
    new (self) wis::impl::VKXlibExtensionImpl{
        .header                 = { &wis::detail::VKXlibExtensionInit },
        .instance_control_block = nullptr,
    };
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_PLATFORM_API void wisVKDestroyXlibExtension(WisVKXlibExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXlibExtensionImpl>(self);
    if (impl.instance_control_block) {
        wis::detail::release_vk_instance(impl.instance_control_block);
    }
}

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XLIB_PRESENT)
#endif // WIS_VK_PLATFORM_XLIB_CPP
