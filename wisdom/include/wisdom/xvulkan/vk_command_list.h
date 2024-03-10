#pragma once
#include <wisdom/generated/api/api.h>
#include <wisdom/xvulkan/vk_views.h>
#include <wisdom/global/internal.h>

namespace wis {
class VKCommandList;
struct VKBufferBarrier2;
struct VKTextureBarrier2;

template<>
struct Internal<VKCommandList> {
    wis::SharedDevice device;
    h::VkCommandPool allocator = nullptr;
    h::VkCommandBuffer command_list = nullptr;

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


    WIS_INLINE void BufferBarrier(wis::BufferBarrier barrier, VKBufferView buffer) noexcept;
    // 8 buffers at once max for efficiency
    WIS_INLINE void BufferBarriers(wis::VKBufferBarrier2* barriers, uint32_t barrier_count) noexcept;


    WIS_INLINE void TextureBarrier(wis::TextureBarrier barrier, VKTextureView texture) noexcept;
    // 8 textures at once max for efficiency
    WIS_INLINE void TextureBarriers(wis::VKTextureBarrier2* barrier, uint32_t barrier_count) noexcept;

protected:
    bool closed = false;
};

} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_command_list.cpp"
#endif // !WISDOM_HEADER_ONLY