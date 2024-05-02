#pragma once
#include <span>
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisvk/vk_loader.hpp>

namespace wis {
class VKAdapter;

template<>
struct Internal<VKAdapter> {
    wis::SharedInstance instance;
    VkPhysicalDevice adapter;
};

class VKAdapter : public QueryInternal<VKAdapter>
{
public:
    VKAdapter() noexcept = default;
    explicit VKAdapter(wis::SharedInstance instance, VkPhysicalDevice adapter) noexcept
        : QueryInternal(std::move(instance), adapter) { }

    operator bool() const noexcept
    {
        return adapter != nullptr;
    }

public:
    [[nodiscard]] WIS_INLINE wis::Result
    GetDesc(AdapterDesc* pout_desc) const noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_adapter.cpp"
#endif // !WISDOM_HEADER_ONLY
