#ifndef WIS_DX12_PLATFORM_TYPES_HPP
#define WIS_DX12_PLATFORM_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

namespace wis {
//-----------------------------------------------------------------------------
namespace detail {

} // namespace detail

namespace impl {
struct DX12Win32ExtensionImpl {
    DX12InstanceExtensionHeader header;
    IDXGIFactory6*              factory;
};

struct DX12UWPExtensionImpl {
    DX12InstanceExtensionHeader header;
    IDXGIFactory6*              factory;
};
} // namespace impl
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_PLATFORM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "dx12_platform_win32.cpp"
#include "dx12_platform_uwp.cpp"

#endif // WISDOM_BUILD_BINARIES
#endif // WIS_DX12_PLATFORM_TYPES_HPP
