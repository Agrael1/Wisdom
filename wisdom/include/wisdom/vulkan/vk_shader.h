#ifndef WIS_VK_SHADER_H
#define WIS_VK_SHADER_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>
#endif // !WISDOM_MODULE_DECL

namespace wis {
class VKShader;

WISDOM_EXPORT
template<>
struct Internal<VKShader> {
    wis::managed_handle_ex<VkShaderModule> shader;
};

WISDOM_EXPORT
class VKShader : public QueryInternal<VKShader>
{
public:
    VKShader() = default;
    operator VKShaderView() const noexcept
    {
        return shader.get();
    }
    operator bool() const noexcept
    {
        return bool(shader);
    }
};
} // namespace wis

#endif // VK_SHADER_H
