#ifndef WIS_DX12_CONVERT_H
#define WIS_DX12_CONVERT_H
#ifndef WISDOM_MODULE_DECL
#include <dxgi1_6.h>
#include <string>
#include <wisdom/generated/api/api.hpp>
#endif

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
    case DXGI_STATUS_OCCLUDED:
        return wis::Status::Occluded;
    default:
        return wis::Status::Error;
    }
}
inline std::string to_string(std::wstring_view value) noexcept
{
    const int size =
            WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()),
                                nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        return {};
    }

    std::string result(size, '?');
    WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()),
                        result.data(), size, nullptr, nullptr);
    return result;
}
} // namespace wis

#endif // WIS_DX12_CONVERT_H
