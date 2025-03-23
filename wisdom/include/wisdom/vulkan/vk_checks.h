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
WIS_CONSTEXPR23 inline wis::Result make_result(VkResult hr) noexcept
{
    using namespace wis::literals;
    WIS_CONSTEXPR23 static auto str = make_error_string<func, message>();
    return wis::Result{ convert_vk(hr), str.c_str() };
}
} // namespace wis

#endif // !VK_CHECKS_H
