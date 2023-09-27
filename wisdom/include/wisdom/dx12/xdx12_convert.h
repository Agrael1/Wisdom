#pragma once
#include <wisdom/api/api.h>
#include <wisdom/util/error_messages.h>
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


template<wis::string_literal func, wis::string_literal message>
constexpr inline wis::Result MakeResult(HRESULT hr) noexcept
{
    static constinit auto str = wis::MakeErrorString<func, message>();
    return wis::Result{ convert(hr), str.c_str() };
}
} // namespace wis
