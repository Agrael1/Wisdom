#ifndef WIS_DX12_UTILS_HPP
#define WIS_DX12_UTILS_HPP
#include <wisdom/global/definitions.h>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/error_messages.hpp>
#include <wisdom/util/allocation.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace wis::detail {
static constexpr inline WisResult success{ WisStatusOk, S_OK, "Operation succeeded." };

inline constexpr WisStatus convert(HRESULT hr) noexcept
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

//TODO: Evaluate if this can be removed
inline std::unique_ptr<char[]> to_string(std::wstring_view value) noexcept
{
    const int size =
            WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()), nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        return {};
    }
    auto result = wis::make_unique<char[]>(std::size_t(size + 1));
    WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(), static_cast<int32_t>(value.size()), result.get(), size, nullptr, nullptr);
    return result;
}

template<func_pair func, wis::fixed_string message>
constexpr inline WisResult make_result(HRESULT hr) noexcept
{
    static WIS_CONSTEXPR23 const auto str = wis::detail::make_error_string<message, func>();
    return { convert(hr), hr, str.c_str() };
}

/// @brief Check if the given HRESULT a success code, without logging, serves as an assert
/// @param hr HRESULT to check
/// @return True if the HRESULT is a success code
constexpr inline bool succeeded(HRESULT hr) noexcept
{
    return hr >= 0;
}

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
} // namespace wis::detail

#endif // WIS_DX12_UTILS_HPP