#pragma once
#include <wisdom/vulkan/xvk_convert.h>
#include <wisdom/util/error_messages.h>

namespace wis {
/// @brief Check if the given HRESULT a success code, without logging, serves as an assert
/// @param hr HRESULT to check
/// @return True if the HRESULT is a success code
inline bool succeeded(VkResult hr) noexcept
{
    return hr >= 0;
}

template<string_literal func, wis::string_literal message>
constexpr inline wis::Result make_result(VkResult hr) noexcept
{
    static constinit auto str = wis::make_error_string<func, message>();
    return wis::Result{ convert(hr), str.c_str() };
}

template<wis::string_literal message>
constexpr inline wis::Result make_result(VkResult hr) noexcept
{
    return wis::Result{ convert(hr), message.c_str() };
}
}