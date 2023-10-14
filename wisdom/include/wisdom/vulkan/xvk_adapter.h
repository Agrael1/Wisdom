#pragma once
#include <wisdom/api/api.h>
#include <wisdom/api/internal.h>
#include <vulkan/vulkan.h>

namespace wis {
class VKAdapter;

template<>
struct Internal<VKAdapter> {
    VkPhysicalDevice adapter{};
};

class VKAdapter : public QueryInternal<VKAdapter>
{
public:
    VKAdapter() noexcept = default;
    explicit VKAdapter(VkPhysicalDevice adapter) noexcept
        : QueryInternal(adapter)
    {
    }
    operator bool() const noexcept { return adapter != nullptr; }

public:
};
}