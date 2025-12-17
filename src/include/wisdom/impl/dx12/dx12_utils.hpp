#ifndef WIS_DX12_UTILS_HPP
#define WIS_DX12_UTILS_HPP
#include <wisdom/generated/c_api.h>
#include <wisdom/util/error_messages.hpp>
#include <wisdom/util/allocation.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace wis {

namespace detail {
static constexpr inline WisResult dx_success{ WisStatusOk, S_OK, "Operation succeeded." };

//-----------------------------------------------------------------------------
inline constexpr WisStatus convert_dx(HRESULT hr) noexcept
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
template<func_pair func, wis::fixed_string message>
WIS_CONSTEXPR23 inline WisResult make_result(HRESULT hr) noexcept
{
    static WIS_CONSTEXPR23 const auto str = wis::detail::make_error_string<message, func>();
    return { convert_dx(hr), hr, str.c_str() };
}

//-----------------------------------------------------------------------------
/// @brief Check if the given HRESULT a success code, without logging, serves as an assert
/// @param hr HRESULT to check
/// @return True if the HRESULT is a success code
constexpr inline bool succeeded(HRESULT hr) noexcept
{
    return hr >= 0;
}

//-----------------------------------------------------------------------------
/// @brief Releases an object via its Release() method if the pointer is non-null, then sets the pointer to nullptr.
/// @tparam T The type of the pointed-to object. T must provide a Release() member function.
/// @param ptr A reference to a pointer to the object to release. The function checks for null, calls ptr->Release() if non-null, and then sets the pointer to nullptr.
template<typename T>
constexpr inline void safe_release(T*& ptr) noexcept
{
    if (ptr) {
        ptr->Release();
        ptr = nullptr;
    }
}
template<typename T>
constexpr inline void safe_release_array(T** ptr, std::size_t size) noexcept
{
    for (std::size_t i = 0; i < size; ++i) {
        if (ptr[i]) {
            ptr[i]->Release();
            ptr[i] = nullptr;
        }
    }
}
} // namespace detail
} // namespace wis

#endif // WIS_DX12_UTILS_HPP
