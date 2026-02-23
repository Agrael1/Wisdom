#ifndef WIS_VK_COMMAND_QUEUE_CPP
#define WIS_VK_COMMAND_QUEUE_CPP
#include <wisdom/generated/vk_cpp_api.hpp>
#include <wisdom/generated/vk_api.h>
#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <bit>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self)
{
    auto& impl = *reinterpret_cast<VKCommandQueueImpl*>(self);
    if (impl.queue) {
        detail::release_vk_device(impl.device, impl.device_header);
        impl.device_header = nullptr;
        impl.device        = VK_NULL_HANDLE;
        impl.queue         = VK_NULL_HANDLE;
    }
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueSubmit(const WisVKCommandQueue*    self,
                                                          const WisVKCommandListView* lists,
                                                          size_t                      count)
{
    auto& impl = *reinterpret_cast<const VKCommandQueueImpl*>(self);

    // I am not sorry, reinterpret_cast is the only way to convert from WisVKCommandListView
    // (which is a pointer to an opaque handle) to VkCommandBuffer* without violating strict aliasing rules.
    VkSubmitInfo submit_info{
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext              = nullptr,
        .commandBufferCount = static_cast<uint32_t>(count),
        .pCommandBuffers    = reinterpret_cast<const VkCommandBuffer*>(lists),
    };

    auto vr = impl.device_header->header.command_queue_table.vkQueueSubmit(impl.queue, 1, &submit_info, nullptr);
    if (!succeeded(vr)) {
        return make_result<Func(), "vkQueueSubmit failed to submit command list(s)">(vr);
    }
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueSignalFence(const WisVKCommandQueue* self,
                                                               WisVKFenceView           fence,
                                                               uint64_t                 value)
{
    auto&                 impl  = *reinterpret_cast<const VKCommandQueueImpl*>(self);
    VkQueue               queue = impl.queue;
    VkSemaphore           sem   = std::bit_cast<VkSemaphore>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value     = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &sem_submit
    };
    VkResult result = impl.device_header->header.command_queue_table.vkQueueSubmit2(queue, 1, &info, nullptr);
    if (!succeeded(result)) {
        return make_result<Func(), "vkQueueSubmit failed to signal fence">(result);
    }
    return vk_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisVKCommandQueueWaitFence(const WisVKCommandQueue* self,
                                                             WisVKFenceView           fence,
                                                             uint64_t                 value)
{
    auto&                 impl  = *reinterpret_cast<const VKCommandQueueImpl*>(self);
    VkQueue               queue = impl.queue;
    VkSemaphore           sem   = std::bit_cast<VkSemaphore>(fence);
    VkSemaphoreSubmitInfo sem_submit{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = sem,
        .value     = value,
        .stageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    VkSubmitInfo2 info{
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos    = &sem_submit
    };
    VkResult result = impl.device_header->header.command_queue_table.vkQueueSubmit2(queue, 1, &info, nullptr);
    if (!succeeded(result)) {
        return make_result<Func(), "vkQueueSubmit failed to signal fence">(result);
    }
    return vk_success;
}

#endif // WIS_VK_FENCE_CPP
