#pragma once
#include <wisdom/xvulkan/vk_views.h>
#include <wisdom/global/internal.h>

namespace wis {
class VKCommandList;

template<>
struct Internal<VKCommandList> {
    wis::managed_handle_ex<VkCommandPool> allocator;
    VkCommandBuffer command_list;
    VkPipeline pipeline;
};
class VKCommandList : public QueryInternal<VKCommandList>
{
public:
    VKCommandList() noexcept = default;
    VKCommandList(wis::managed_handle_ex<VkCommandPool> allocator,
                  VkCommandBuffer command_list) noexcept
        : QueryInternal(std::move(allocator), command_list) { }
    operator bool() const noexcept
    {
        return bool(command_list);
    }
    operator VKCommandListView() const noexcept
    {
        return command_list;
    }

public:
};

} // namespace wis