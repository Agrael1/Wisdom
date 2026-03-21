#ifndef WIS_VK_FENCE_CPP
#define WIS_VK_FENCE_CPP
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyFence(WisVKFence* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKFenceImpl>(self);
    if (impl.fence != VK_NULL_HANDLE) {
        auto& table = impl.device_header->header.device_table;
        table.vkDestroySemaphore(impl.device, impl.fence, nullptr);
        impl.fence = VK_NULL_HANDLE;
        wis::detail::release_vk_device(impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisVKFenceGetCompletedValue(const WisVKFence* self)
{
    auto&    impl  = wis::from_handle_ref<const wis::impl::VKFenceImpl>(self);
    uint64_t value = 0;
    std::ignore    = impl.device_header->header.device_table.vkGetSemaphoreCounterValue(impl.device,
                                                                                     impl.fence,
                                                                                     &value); // always succeeds
    return value;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKFenceWait(const WisVKFence* self,
                                                 uint64_t          value,
                                                 uint64_t          wait_ns)
{
    auto&    impl    = wis::from_handle_ref<const wis::impl::VKFenceImpl>(self);
    uint64_t timeout = wait_ns == UINT64_MAX ? UINT64_MAX : wait_ns / 1'000'000; // convert to ms, with special handling for infinite timeout

    VkSemaphoreWaitInfo wait_info{
        .sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
        .pNext          = nullptr,
        .flags          = 0,
        .semaphoreCount = 1,
        .pSemaphores    = &impl.fence,
        .pValues        = &value
    };
    auto res = impl.device_header->header.device_table.vkWaitSemaphores(impl.device, &wait_info, wait_ns);
    if (res == VK_SUCCESS) {
        return wis::detail::vk_success;
    } else if (res == VK_TIMEOUT) {
        return { WisStatus::WisStatusTimeout, 0, "Wait timed out." };
    } else {
        return wis::detail::make_result<wis::detail::Func(), "Failed to wait for fence">(res);
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKFenceSignal(const WisVKFence* self,
                                                   uint64_t          value)
{
    auto&                 impl  = wis::from_handle_ref<const wis::impl::VKFenceImpl>(self);
    auto&                 table = impl.device_header->header.device_table;
    VkSemaphoreSignalInfo signal_info{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
        .pNext     = nullptr,
        .semaphore = impl.fence,
        .value     = value
    };
    auto res = table.vkSignalSemaphore(impl.device, &signal_info);
    if (res == VK_SUCCESS) {
        return wis::detail::vk_success;
    } else {
        return wis::detail::make_result<wis::detail::Func(), "Failed to signal fence">(res);
    }
}

#endif // WIS_VK_FENCE_CPP
