#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_shader.h>
#endif

WIS_EXPORT namespace wis
{
    class DX12Shader;

    template<>
    class Internal<DX12Shader>
    {
    public:
        Internal() = default;
        Internal(shared_blob blob)
            : bytecode(std::move(blob)){};

    public:
        [[nodiscard]] std::span<const std::byte> GetShaderBytecode() const noexcept
        {
            return bytecode.span<std::byte>();
        }

    protected:
        wis::shared_blob bytecode;
    };

    /// @brief Shader object
    class DX12Shader : public QueryInternal<DX12Shader>
    {
    public:
        static constexpr inline ShaderLang language = ShaderLang::dxil;

    public:
        DX12Shader() = default;
        explicit DX12Shader(shared_blob blob, ShaderType type)
            : QueryInternal(std::move(blob)), type(type)
        {
        }
        operator bool() const noexcept
        {
            return type != ShaderType::unknown && !bytecode.empty();
        }

    public:
        /// @brief Type of shader e.g vertex, fragment, compute
        /// @return shader type
        [[nodiscard]] auto GetType() const noexcept
        {
            return type;
        }

    public:
        ShaderType type = ShaderType::unknown;
    };
}