#ifndef VK_MEMORY_H
#define VK_MEMORY_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_handles.h>
#include <wisdom/vulkan/vk_views.h>

namespace wis {
class VKMemory;

namespace h {
using VmaAllocation = wis::movable_handle<::VmaAllocation>;
}

template<>
struct Internal<VKMemory> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;

    Internal() noexcept = default;
    Internal(wis::shared_handle<VmaAllocator> allocator, ::VmaAllocation allocation) noexcept
        : allocator(std::move(allocator)), allocation(allocation) { }
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        allocator = std::move(o.allocator);
        allocation = std::move(o.allocation);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }
    void Destroy() noexcept
    {
        if (allocation) {
            vmaFreeMemory(allocator.get(), allocation);
        }
    }
};

class VKMemory : public QueryInternal<VKMemory>
{
public:
    VKMemory() noexcept = default;
    explicit VKMemory(wis::shared_handle<VmaAllocator> allocator, VmaAllocation allocation) noexcept
        : QueryInternal<VKMemory>(std::move(allocator), allocation)
    {
    }
    operator bool() const noexcept
    {
        return bool(allocation);
    }
    operator VKMemoryView() const noexcept
    {
        return { allocator.get(), allocation };
    }
    [[nodiscard]] bool
    operator==(std::nullptr_t) const noexcept
    {
        return !allocation;
    }
    [[nodiscard]] bool
    operator==(const VKMemory& other) const noexcept
    {
        return allocator.get() == other.allocator.get() && allocation == other.allocation;
    }

public:
    [[nodiscard]] uint64_t GetBlockOffset() const noexcept
    {
        if (!allocation)
            return 0;

        VmaAllocationInfo2 info{};
        vmaGetAllocationInfo2(allocator.get(), allocation, &info);
        return info.allocationInfo.offset;
    }

public:
    [[nodiscard]] void* VKMap() const noexcept
    {
        void* data;
        vmaMapMemory(allocator.get(), allocation, &data);
        return data;
    }
    void VKUnmap() const noexcept
    {
        vmaUnmapMemory(allocator.get(), allocation);
    }
};

} // namespace wis

#endif // !VK_MEMORY_H