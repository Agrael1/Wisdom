#pragma once
#include <wisdom/global/definitions.h>
#include <wisdom/dx12/xdx12_info.h>

namespace wis {
/// @brief Log any errors in the current context
/// @return true if there were any errors
inline void log_dxgi_errors() noexcept
{
    DX12Info::Poll();
}

/// @brief Check if the given HRESULT a success code, without logging, serves as an assert
/// @param hr HRESULT to check
/// @return True if the HRESULT is a success code
inline bool succeeded(HRESULT hr) noexcept
{
    if constexpr (debug_layer)
        log_dxgi_errors();
    return hr >= 0;
}

/// @brief Check if there are any errors in the current context, logging any errors
inline void check_context() noexcept
{
    if constexpr (debug_layer)
        log_dxgi_errors();
}
} // namespace wis
