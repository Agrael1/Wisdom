#pragma once
#include <span>
#include <wisdom/global/internal.h>
#include <wisdom/xvulkan/vk_checks.h>
#include <wisdom/xvulkan/vk_views.h>
#include <wisvk/vk_loader.hpp>

namespace wis {
class VKAdapter;

template<>
struct Internal<VKAdapter> {
    VkPhysicalDevice adapter{};
    VkInstanceTable* instance_table{};
};

class VKAdapter : public QueryInternal<VKAdapter>
{
public:
    VKAdapter() noexcept = default;
    explicit VKAdapter(VkPhysicalDevice adapter, VkInstanceTable* instance_table) noexcept
        : QueryInternal(adapter, instance_table) { }
    operator bool() const noexcept { return adapter != nullptr; }
    operator VKAdapterHandle() const noexcept { return { adapter, instance_table }; }

public:
    WIS_INLINE [[nodiscard]] wis::Result GetDesc(AdapterDesc* pout_desc) const noexcept;
};
} // namespace wis

#ifdef WISDOM_HEADER_ONLY
#include "impl/vk_adapter.cpp"
#endif // !WISDOM_HEADER_ONLY