#pragma once
#include <memory>
#include <span>

WIS_EXPORT namespace wis
{
    /// @brief Shader intermediate language
    enum class ShaderLang {
        dxil,
        spirv
    };

    /// @brief Type of shader
    enum class ShaderType {
        unknown,
        vertex,
        pixel,
        geometry,
        hull,
        domain,
        amplification,
        mesh,
        compute
    };
}
