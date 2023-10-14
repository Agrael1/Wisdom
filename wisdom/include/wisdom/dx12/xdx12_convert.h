#pragma once
#include <wisdom/api/api.h>
#include <dxgi1_6.h>
#include <string>

namespace wis {
inline constexpr wis::Status convert(HRESULT hr) noexcept
{
    switch (hr) {
    case S_OK:
        return wis::Status::Ok;
    case E_INVALIDARG:
        return wis::Status::InvalidArgument;
    case E_OUTOFMEMORY:
        return wis::Status::OutOfMemory;
    case DXGI_ERROR_DEVICE_REMOVED:
        return wis::Status::DeviceLost;
    default:
        return wis::Status::Error;
    }
}
inline constexpr DXGI_GPU_PREFERENCE convert_dx(wis::AdapterPreference pref) noexcept
{
    switch (pref) {
    default:
    case wis::AdapterPreference::None:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_UNSPECIFIED;
    case wis::AdapterPreference::MinConsumption:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER;
    case wis::AdapterPreference::Performance:
        return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
    }
}
inline std::string to_string(std::wstring_view value) noexcept
{
    const int size = WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()), nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        return {};
    }

    std::string result(size, '?');
    WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()), result.data(), size, nullptr, nullptr);
    return result;
}
} // namespace wis
