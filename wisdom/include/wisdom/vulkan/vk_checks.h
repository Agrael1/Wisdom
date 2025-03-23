#ifndef WIS_VK_CHECKS_H
#define WIS_VK_CHECKS_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/vulkan/vk_convert.h>
#include <wisdom/util/error_messages.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
/// @brief Check if the given HRESULT a success code, without logging, serves as an assert
/// @param hr HRESULT to check
/// @return True if the HRESULT is a success code
inline bool succeeded(VkResult hr) noexcept
{
    return hr >= 0;
}

template<wis::fixed_string func, wis::fixed_string message>
inline wis::Result make_result(VkResult hr) noexcept
{
    static auto str = wis::make_error_string<func, message>();
    return wis::Result{ convert_vk(hr), str.c_str() };
}

template<wis::fixed_string message>
constexpr inline wis::Result make_result(VkResult hr) noexcept
{
    return wis::Result{ convert_vk(hr), message.c_str() };
}
} // namespace wis

#endif // !VK_CHECKS_H
