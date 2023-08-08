#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>
#include <wisdom/dx12/dx12_views.h>

#include <winrt/base.h>
#include <dxgi.h>
#endif // !WISDOM_MODULES

namespace wis {
class DX12Adapter;

template<>
struct Internal<DX12Adapter> {
    winrt::com_ptr<IDXGIAdapter1> adapter;
};

/// @brief DX12 physical adapter
WIS_EXPORT class DX12Adapter : public QueryInternal<DX12Adapter>
{
public:
    DX12Adapter() = default;
    explicit DX12Adapter(winrt::com_ptr<IDXGIAdapter1> adapter) noexcept
        : QueryInternal(std::move(adapter))
    {
    }
    operator DX12AdapterView() const noexcept
    {
        return adapter.get();
    }
    operator bool() const noexcept
    {
        return adapter != nullptr;
    }

public:
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
#include "impl/dx12_adapter.inl"
#endif
