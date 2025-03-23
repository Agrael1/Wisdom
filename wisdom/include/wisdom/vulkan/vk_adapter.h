#ifndef WIS_VK_ADAPTER_H
#define WIS_VK_ADAPTER_H
#ifndef WISDOM_MODULE_DECL
#include <span>
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_checks.h>
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/generated/vulkan/vk_functions.hpp>
#endif // !WISDOM_MODULE_DECL

namespace wis {
class VKAdapter;

WISDOM_EXPORT
template<>
struct Internal<VKAdapter> {
    wis::SharedInstance instance;
    VkPhysicalDevice adapter = VK_NULL_HANDLE;
};

class ImplVKAdapter : public QueryInternal<VKAdapter>
{
public:
    ImplVKAdapter() noexcept = default;
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
WISDOM_EXPORT
class VKAdapter : public wis::ImplVKAdapter
{
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
