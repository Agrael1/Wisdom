#pragma once
#include <wisdom/bridge/source_location.h>
#include <Windows.h>

namespace wis {
/// @brief Get the last windows error
/// @return HRESULT of the last windows error
HRESULT last_windows_error() noexcept;

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
