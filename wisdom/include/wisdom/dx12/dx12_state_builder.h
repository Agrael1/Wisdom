#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/dx12/dx12_render_pass.h>
#include <wisdom/api/api_common.h>
#include <wisdom/util/small_allocator.h>
#endif

namespace wis {
WIS_EXPORT class DX12GraphicsPipelineDesc
{
    friend class DX12Device;

public:
    DX12GraphicsPipelineDesc(DX12RootSignature sig) noexcept
        : sig(std::move(sig))
    {
    }

public:
    DX12GraphicsPipelineDesc& SetVS(DX12Shader vs) noexcept
    {
        this->vs = std::move(vs);
        return *this;
    }
    DX12GraphicsPipelineDesc& SetPS(DX12Shader ps) noexcept
    {
        this->ps = std::move(ps);
        return *this;
    }
    DX12GraphicsPipelineDesc& SetGS(DX12Shader gs) noexcept
    {
        this->gs = std::move(gs);
        return *this;
    }
    DX12GraphicsPipelineDesc& SetHS(DX12Shader hs) noexcept
    {
        this->hs = std::move(hs);
        return *this;
    }
    DX12GraphicsPipelineDesc& SetDS(DX12Shader ds) noexcept
    {
        this->ds = std::move(ds);
        return *this;
    }
    DX12GraphicsPipelineDesc& SetShader(DX12Shader shader) noexcept
    {
        using enum ShaderType;
        switch (shader.GetType()) {
        case vertex:
            return SetVS(std::move(shader));
        case pixel:
            return SetPS(std::move(shader));
        case geometry:
            return SetGS(std::move(shader));
        case hull:
            return SetHS(std::move(shader));
        case domain:
            return SetDS(std::move(shader));
        default:
            break;
        }
        return *this;
    }
    DX12GraphicsPipelineDesc& SetRenderPass(DX12RenderPass pass) noexcept
    {
        render_pass = std::move(pass);
        return *this;
    }

private:
    DX12RootSignature sig;
    DX12Shader vs;
    DX12Shader ps;
    DX12Shader gs;
    DX12Shader hs;
    DX12Shader ds;
    DX12RenderPass render_pass;
};

// TODO: Mesh and Compute pipelines
} // namespace wis
