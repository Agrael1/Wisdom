#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#endif

WIS_EXPORT namespace wis
{
    class VKShader;

    template<>
    class Internal<VKShader>
    {
    public:
        wis::shared_handle<vk::ShaderModule> module;
    };

    /// @brief Shader object
    class VKShader : public QueryInternal<VKShader>
    {
    public:
        using DataType = uint32_t;
        static constexpr inline ShaderLang language = ShaderLang::spirv;

        VKShader() = default;
        explicit VKShader(wis::shared_handle<vk::ShaderModule> module, ShaderType type)
            : QueryInternal(std::move(module)), type(type)
        {
        }

        operator bool() const noexcept
        {
            return type != ShaderType::unknown && module;
        }
        /// @brief Get shader type e.g. vertex, fragment
        /// @return Type of shader
        [[nodiscard]] auto GetType() const noexcept
        {
            return type;
        }

        ShaderType type = ShaderType::unknown;
    };
}
