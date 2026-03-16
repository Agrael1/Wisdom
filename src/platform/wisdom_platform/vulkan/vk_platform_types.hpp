#ifndef WIS_VK_PLATFORM_TYPES_HPP
#define WIS_VK_PLATFORM_TYPES_HPP
#ifndef __cplusplus
#error "This header requires C++"
#endif // __cplusplus

#include <wisdom/vk/vk_types.hpp>

namespace wis {
//-----------------------------------------------------------------------------
namespace detail {

} // namespace detail

namespace impl {

struct VKWin32ExtensionImpl {
};

} // namespace impl
} // namespace wis

// Include implementation if header only build
#ifndef WISDOM_PLATFORM_BUILD_BINARIES
#if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#error "C++20 is required to build wisdom as header-only library"
#endif // !WIS_HAS_CPP20

#include "vk_platform_win32.cpp"

#endif // WISDOM_BUILD_BINARIES
#endif // WIS_VK_PLATFORM_TYPES_HPP
