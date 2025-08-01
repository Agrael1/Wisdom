#ifndef WIS_VK_RTAS_H
#define WIS_VK_RTAS_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_handles.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/global/internal.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
class VKAccelerationStructure;

template<>
struct Internal<VKAccelerationStructure> {
    wis::SharedDevice device;
    h::VkAccelerationStructureKHR handle;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;

public:
    Internal() noexcept = default;
    Internal(Internal&&) noexcept = default;
    Internal& operator=(Internal&& o) noexcept
    {
        if (this == &o) {
            return *this;
        }
        Destroy();
        device = std::move(o.device);
        handle = std::move(o.handle);
        vkDestroyAccelerationStructureKHR = std::move(o.vkDestroyAccelerationStructureKHR);
        return *this;
    }
    ~Internal() noexcept
    {
        Destroy();
    }

    void Destroy() noexcept
    {
        if (handle) {
            vkDestroyAccelerationStructureKHR(device.get(), handle, nullptr);
            handle = VK_NULL_HANDLE;
        }
    }
};

class VKAccelerationStructure : public QueryInternal<VKAccelerationStructure>
{
public:
    VKAccelerationStructure() noexcept = default;
    operator bool() const noexcept
    {
        return handle != VK_NULL_HANDLE;
    }
    operator VKAccelerationStructureView() const noexcept
    {
        return { handle };
    }
};
} // namespace wis
#endif // WIS_VK_RTAS_H
