#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/util/exception.h>
#include <winrt/base.h>
#include <wisdom/dx12/xdx12_info.h>
#endif

WIS_EXPORT namespace wis
{
    /// @brief Log any errors in the current context
    /// @return true if there were any errors
    WIS_INLINE bool log_dxgi_errors() noexcept
    {
        DX12Info::Poll();
        return true;
    }

    /// @brief Chaeck if there are any errors in the current context, logging any errors
    /// @param sl Source location
    inline void check_context(wis::source_location sl = wis::source_location::current())
    {
        if constexpr (debug_mode)
            if (log_dxgi_errors())
                throw wis::exception{ sl };
    }

    /// @brief Check if the given HRESULT a success code, without logging, serves as an assert
    /// @param hr HRESULT to check
    /// @return True if the HRESULT is a success code
    inline bool succeeded(winrt::hresult hr) noexcept
    {
        if constexpr (debug_mode || runtime_asserts)
            log_dxgi_errors();
        return hr >= 0;
    }

    /// @brief Check if there are any errors in the current context, logging any errors
    /// @param sl Source location
    inline void log_context(wis::source_location sl = wis::source_location::current())
    {
        if constexpr (debug_mode)
            log_dxgi_errors();
    }

    /// @brief Check if the given HRESULT a success code, without logging, serves as an assert
    /// @param hr HRESULT to check
    /// @return True if the HRESULT is a success code
    // inline bool succeded_weak(winrt::hresult hr)
    //{
    //     if constexpr (!debug_mode || !runtime_asserts)
    //         return hr >= 0;
    //     return check_hresult_nothrow(hr);
    // }
}

#if defined(WISDOM_HEADER_ONLY)
#include "impl/dx12_checks.inl"
#endif
