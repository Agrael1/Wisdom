#ifndef WIS_DX12_CHECKS_H
#define WIS_DX12_CHECKS_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/util/error_messages.h>
#include <wisdom/dx12/dx12_convert.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/global/constants.h>
#endif

WISDOM_EXPORT
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
    if constexpr (debug_layer) {
        log_dxgi_errors();
    }
    return hr >= 0;
}

/// @brief Check if there are any errors in the current context, logging any errors
inline void check_context() noexcept
{
    if constexpr (debug_layer) {
        log_dxgi_errors();
    }
}

template<wis::fixed_string func, wis::fixed_string message>
WIS_CONSTEXPR23 inline wis::Result make_result(HRESULT hr) noexcept
{
    static WIS_CONSTEXPR23 auto str = wis::make_error_string<func, message>();
    return wis::Result{ convert(hr), str.c_str() };
}

} // namespace wis

#endif // !WIS_DX12_CHECKS_H
