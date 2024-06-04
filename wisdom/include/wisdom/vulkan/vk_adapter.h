#ifndef VK_ADAPTER_H
#define VK_ADAPTER_H
#include <span>
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisvk/vk_loader.hpp>
#include <wisdom/generated/vulkan/vk_functions.hpp>

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

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_adapter.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ADAPTER_H
