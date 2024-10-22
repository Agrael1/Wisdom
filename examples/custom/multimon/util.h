#pragma once
#include <wisdom/bridge/source_location.h>
#include <Windows.h>

// #include <wisdom/util/exception.h>
// #include <winrt/base.h>

namespace wis {
/// @brief Get the last windows error
/// @return HRESULT of the last windows error
HRESULT last_windows_error() noexcept;

// class hr_exception : public wis::exception
//{
// public:
//     WIS_INLINE hr_exception(winrt::hresult hr, wis::source_location sl = wis::source_location::current());
//
//     WIS_INLINE const char* what() const noexcept override;
//     std::string_view type() const noexcept override
//     {
//         return "Vertas Window Exception";
//     }
//     winrt::hresult error_code() const noexcept
//     {
//         return hResult;
//     }
//     WIS_INLINE std::string description() const noexcept;
//
// private:
//     winrt::hresult hResult;
// };

/// @brief Check if the given boolean is true, logging any errors and throwing a windows exception if it is not true
/// @param check Value to check
/// @param sl Source location
inline void check_windows(bool check, wis::source_location sl = wis::source_location::current())
{
    if (check)
        return;
    throw last_windows_error();
}

} // namespace wis
