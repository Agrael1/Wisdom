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
struct VKXCBExtensionTable {
    PFN_vkCreateXcbSurfaceKHR                        vkCreateXcbSurfaceKHR;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
};

inline WisResult VKXCBExtensionInit(VKInstanceExtensionHeader*    self,
                                    impl::VKInstanceImpl*         instance_impl,
                                    VKInstanceExtensionCollector* collector) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKXCBExtensionImpl>(self);

    if (!instance_impl) {
        collector->EnableExtension(VK_KHR_SURFACE_EXTENSION_NAME);
        collector->EnableExtension(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    } else {
        impl.instance_control_block = instance_impl->shared_header;
        impl.instance_control_block->AddRef();

        auto  instance = instance_impl->instance;
        auto& gtable   = impl.instance_control_block->header.global_table;
        auto* ftable = impl.function_table = new (std::nothrow) wis::detail::VKXCBExtensionTable;
        if (!ftable) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory to create a function table">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        ftable->vkCreateXcbSurfaceKHR =
                reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR"));
        ftable->vkGetPhysicalDeviceXcbPresentationSupportKHR =
                reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(
                        gtable.vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR"));
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

#endif // defined(WISDOM_VULKAN) && defined(WIS_PLATFORM_XCB_PRESENT)
#endif // WIS_VK_PLATFORM_XCB_CPP
