#pragma once
#include <wisdom/generated/api/api.h>
#include <wisdom/xvulkan/vk_views.h>
#include <wisdom/global/internal.h>

namespace wis {
class VKCommandList;

template<>
struct Internal<VKCommandList> {
    wis::SharedDevice device;
    h::VkCommandPool allocator;
    h::VkCommandBuffer command_list;

    wis::SharedPipeline pipeline;

    Internal() noexcept = default;
    Internal(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
        : device(std::move(device)), allocator(allocator), command_list(command_list) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&&) noexcept = default;

    ~Internal() noexcept
    {
        if (command_list) {
            device.table().vkDestroyCommandPool(device.get(), allocator, nullptr);
        }
    }
};
class VKCommandList : public QueryInternal<VKCommandList>
{
public:
    VKCommandList() noexcept = default;
    explicit VKCommandList(wis::SharedDevice device, VkCommandPool allocator, VkCommandBuffer command_list) noexcept
        : QueryInternal(std::move(device), allocator, command_list) { }
    operator bool() const noexcept
    {
        return bool(command_list);
    }
    operator VKCommandListView() const noexcept
    {
        return command_list;
    }

public:
    bool Closed() const noexcept { return closed; }
    WIS_INLINE bool Close() noexcept;
    [[nodiscard]] WIS_INLINE wis::Result Reset(VKPipelineHandle pipeline = {}) noexcept;
    WIS_INLINE void CopyBuffer(VKBufferView source, VKBufferView destination, wis::BufferRegion region) const noexcept;

protected:
    bool closed = true;
};

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY