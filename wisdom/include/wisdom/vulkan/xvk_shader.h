#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/vulkan/xvk_handles.h>

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
    explicit VKShader(wis::managed_handle_ex<VkShaderModule> shader) noexcept
        : QueryInternal(std::move(shader))
    {
    }
    operator bool() const noexcept
    {
        return bool(shader);
    }
};
} // namespace wis
