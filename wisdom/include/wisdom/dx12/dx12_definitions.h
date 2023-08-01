#pragma once
#error aaaaa
#include <wisdom/global/definitions.h>
#include <d3dx12/d3dx12.h>
#include <winrt/base.h>
#include <dxgi1_6.h>
#include <array>

namespace wis {
/// @brief Get the array view of com_ptr
/// @tparam C Type of the com_ptr, deduced from the argument
/// @param self Pointer
/// @return Array view of the pointer
template<class C>
[[nodiscard]] inline auto** array_view(winrt::com_ptr<C>& self)
{
    return reinterpret_cast<C**>(&self);
}
} // namespace wis
