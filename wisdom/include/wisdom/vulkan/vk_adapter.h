#ifndef WIS_VK_ADAPTER_H
#define WIS_VK_ADAPTER_H
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

class ImplVKAdapter : public QueryInternal<VKAdapter>
{
public:
    ImplVKAdapter() noexcept = default;
    explicit ImplVKAdapter(wis::SharedInstance instance, VkPhysicalDevice adapter) noexcept
        : QueryInternal(std::move(instance), adapter) { }

    operator bool() const noexcept
    {
        return adapter != nullptr;
    }

public:
    [[nodiscard]] WIS_INLINE wis::Result
    GetDesc(AdapterDesc* pout_desc) const noexcept;
};

#pragma region VKAdapter
/**
 * @brief Represents physical device.
 * Can safely be deleted once logical device has been created.
 * */
struct VKAdapter : public wis::ImplVKAdapter {
public:
    using wis::ImplVKAdapter::ImplVKAdapter;

public:
    /**
     * @brief Fills wis::AdapterDesc with physical adapter's data.
     * @param inout_desc The wis::AdapterDesc to fill.
     * Must not be NULL.
     * */
    [[nodiscard]] inline wis::Result GetDesc(wis::AdapterDesc* inout_desc) const noexcept
    {
        return wis::ImplVKAdapter::GetDesc(inout_desc);
    }
};
#pragma endregion VKAdapter
} // namespace wis

#ifndef WISDOM_BUILD_BINARIES
#include "impl/vk_adapter.cpp"
#endif // !WISDOM_HEADER_ONLY
#endif // !VK_ADAPTER_H
