#pragma once
#include <wisdom/api/api.h>
#include <dxgi.h>

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
} // namespace wis
