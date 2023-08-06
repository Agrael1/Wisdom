#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_views.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>
#endif // !WISDOM_MODULES

namespace wis {

class VKAdapter;

template<>
struct Internal<VKAdapter> {
    vk::PhysicalDevice adapter{};
};

/// @brief Vulkan physcial adapter
WIS_EXPORT class VKAdapter : public QueryInternal<VKAdapter>
{
public:
    VKAdapter() = default;
    explicit VKAdapter(vk::PhysicalDevice adapter)
        : QueryInternal{ adapter }
    {
    }
    operator VKAdapterView() const noexcept
    {
        return adapter;
    }
    operator bool() const noexcept
	{
		return bool(adapter);
	}

    /// @brief Get the adapter description
    /// @return Adapter Description
    WIS_INLINE void GetDesc(AdapterDesc& desc) const noexcept;

    /// @brief Get the adapter description
    /// @return Adapter Description
    [[nodiscard]] AdapterDesc GetDesc() const noexcept
    {
        AdapterDesc desc{};
        GetDesc(desc);
        return desc;
    }
};
} // namespace wis

#if defined(WISDOM_HEADER_ONLY)
#include "impl/vk_adapter.inl"
#endif
