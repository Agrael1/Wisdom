#ifndef WIS_VK_SHADER_H
#define WIS_VK_SHADER_H
#include <wisdom/global/internal.h>
#include <wisdom/vulkan/vk_views.h>

namespace wis {
class VKShader;

template<>
struct Internal<VKShader> {
    wis::managed_handle_ex<VkShaderModule> shader;
};

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
