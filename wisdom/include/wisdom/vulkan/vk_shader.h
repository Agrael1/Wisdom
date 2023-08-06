#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

namespace wis {
class VKShader;

template<>
struct Internal<VKShader> {
    wis::shared_handle<vk::ShaderModule> module;
};

/// @brief Shader object
WIS_EXPORT class VKShader : public QueryInternal<VKShader>
{
public:
    using DataType = uint32_t;
    static constexpr inline ShaderLang language = ShaderLang::spirv;

    VKShader() = default;
    explicit VKShader(wis::shared_handle<vk::ShaderModule> module, ShaderType type) noexcept
        : QueryInternal(std::move(module)), type(type)
    {
    }

    operator bool() const noexcept
    {
        return type != ShaderType::Unknown && module;
    }

public:
    /// @brief Get shader type e.g. vertex, fragment
    /// @return Type of shader
    [[nodiscard]] ShaderType GetType() const noexcept
    {
        return type;
    }

protected:
    ShaderType type = ShaderType::Unknown;
};
} // namespace wis
