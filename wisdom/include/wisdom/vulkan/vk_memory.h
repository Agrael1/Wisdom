#ifndef WIS_VK_MEMORY_H
#define WIS_VK_MEMORY_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_handles.h>
#include <wisdom/vulkan/vk_views.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
WISDOM_EXPORT class VKMemory;

WISDOM_EXPORT
namespace h {
using VmaAllocation = wis::movable_handle<::VmaAllocation>;
}

WISDOM_EXPORT
template<>
struct Internal<VKMemory> {
    wis::shared_handle<VmaAllocator> allocator;
    h::VmaAllocation allocation;

    Internal() noexcept = default;
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

class ImplVKMemory : public QueryInternal<VKMemory>
{
public:
    ImplVKMemory() noexcept = default;
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
    operator==(const ImplVKMemory& other) const noexcept
    {
        return allocator.get() == other.allocator.get() && allocation == other.allocation;
    }

public:
    [[nodiscard]] uint64_t GetBlockOffset() const noexcept
    {
        if (!allocation) {
            return 0;
        }

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

#pragma region VKMemory
/**
 * @brief Represents memory object for binding resources.
 * */
WISDOM_EXPORT
class VKMemory : public wis::ImplVKMemory
{
public:
    using wis::ImplVKMemory::ImplVKMemory;

public:
    /**
     * @brief Returns the offset of the block in the global memory.
     * @return The offset of the block in the global memory.
     * */
    inline uint64_t GetBlockOffset() const noexcept
    {
        return wis::ImplVKMemory::GetBlockOffset();
    }
};
#pragma endregion VKMemory

} // namespace wis

#endif // !VK_MEMORY_H
