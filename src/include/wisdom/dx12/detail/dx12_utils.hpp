#ifndef WIS_DX12_UTILS_HPP
#define WIS_DX12_UTILS_HPP
#include <wisdom/generated/c_api.h>
#include <wisdom/util/error_messages.hpp>
#include <wisdom/util/allocation.hpp>

namespace wis::detail {
static constexpr WisResult dx_success{ WisStatusOk, S_OK, "Operation succeeded." };

//-----------------------------------------------------------------------------
/**
 * @brief Converts a DirectX 12 HRESULT into a WisStatus. This function maps common HRESULT values to their corresponding WisStatus codes, allowing for consistent error handling across the Wisdom library when interfacing with DirectX 12.
 * @param hr The HRESULT value returned by a DirectX 12 function call.
 * @return A WisStatus value that corresponds to the given HRESULT, enabling the Wisdom library to interpret and handle DirectX 12 errors in a standardized way.
 */
constexpr WisStatus convert_dx(const HRESULT hr) noexcept
{
    switch (hr) {
    case S_OK:
        return WisStatus::WisStatusOk;
    case E_INVALIDARG:
        return WisStatus::WisStatusInvalidArgument;
    case E_OUTOFMEMORY:
        return WisStatus::WisStatusOutOfHostMemory;
    case DXGI_ERROR_DEVICE_REMOVED:
        return WisStatus::WisStatusDeviceLost;
    case DXGI_STATUS_OCCLUDED:
        return WisStatus::WisStatusOccluded;
    default:
        return WisStatus::WisStatusError;
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief Creates a WisResult from a DirectX 12 HRESULT, including an error message that incorporates the function name and a custom message. This function uses compile-time string manipulation to generate a descriptive error message based on the source location of the call and the provided custom message.
 * @tparam func A compile-time pair of indices representing the start and end positions of the function name in the source location's function name string. This is used to extract the function name for the error message.
 * @tparam message A compile-time fixed string that provides additional context about the error. This message is included in the generated error string to give more information about the failure.
 * @param hr The HRESULT value returned by a DirectX 12 function call that indicates the result of the operation.
 * @param sl The source location information, which defaults to the current location where make_result is called. This is used to extract the function name for the error message.
 * @return A WisResult object that contains the converted WisStatus, original HRESULT, and a descriptive error message that includes both the function name and the custom message.
 */
template<func_pair func, wis::fixed_string message>
WIS_CONSTEXPR23 inline WisResult make_result(HRESULT hr, std::source_location sl = std::source_location::current()) noexcept
{
    static const auto str = wis::detail::make_error_string<message, func>(sl);
    return { convert_dx(hr), hr, str.c_str() };
}

//-----------------------------------------------------------------------------
/**
 * @brief Check if the given HRESULT indicates a successful operation.
 * @param hr The HRESULT value to check for success.
 * @return True if the HRESULT indicates success (non-negative), false otherwise.
 */
constexpr bool succeeded(HRESULT hr) noexcept
{
    return SUCCEEDED(hr);
}
} // namespace wis::detail

#endif // WIS_DX12_UTILS_HPP
