#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>
#include <wisdom/dx12/dx12_views.h>

#include <winrt/base.h>
#include <dxgi.h>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    class DX12Adapter;

    template<>
    class Internal<DX12Adapter>
    {
    public:
        Internal() = default;
        Internal(winrt::com_ptr<IDXGIAdapter1> adapter) noexcept
            : adapter(std::move(adapter)) { }

        [[nodiscard]] IDXGIAdapter1 *GetAdapter() const noexcept
        {
            return adapter.get();
        }

    protected:
        winrt::com_ptr<IDXGIAdapter1> adapter;
    };

    /// @brief DX12 physical adapter
    class DX12Adapter final : public QueryInternal<DX12Adapter>
    {
    public:
        DX12Adapter() = default;
        explicit DX12Adapter(winrt::com_ptr<IDXGIAdapter1> adapter) noexcept
            : QueryInternal(std::move(adapter))
        {
        }
        operator DX12AdapterView() const noexcept
        {
            return GetAdapter();
        }

        /// @brief Get the adapter description
        /// @return Adapter Description
        /// @note This function is thread safe
        WIS_INLINE [[nodiscard]] AdapterDesc GetDesc() const noexcept;
    };
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_adapter.inl"
#endif
