#ifndef WIS_DX12_RAYTRACING_TYPES_HPP
#define WIS_DX12_RAYTRACING_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

namespace wis {
//----------------------------------------------------------------------------------------------------------------------
namespace detail {} // namespace detail

namespace impl {
struct DX12RaytracingExtensionImpl {
    DX12DeviceExtensionHeader header;
    ID3D12Device* device;
};
} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "dx12_raytracing.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_DX12_PLATFORM_TYPES_HPP
