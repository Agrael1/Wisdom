// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_DX12_CONVERT_HPP
#define WISDOM_CPP_DX12_CONVERT_HPP
#ifndef __cplusplus
#error "This is a C++ only header"
#endif // __cplusplus

#include <wisdom/generated/c_api.h>
#include <dxgi1_6.h>
#include <d3d12.h>

namespace wis {
namespace detail {

inline DXGI_GPU_PREFERENCE convert(WisAdapterPreference value) noexcept
{
    return static_cast<DXGI_GPU_PREFERENCE>(value);
}

} // namespace detail
} // namespace wis
#endif // WISDOM_DX12_CONVERT_HPP
