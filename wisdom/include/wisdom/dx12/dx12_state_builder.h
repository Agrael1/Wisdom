#pragma once
#include <wisdom/dx12/dx12_root_signature.h>
#include <wisdom/dx12/dx12_shader.h>
#include <wisdom/api/api_common.h>


namespace wis
{
	class DX12GraphicsPipelineDesc
	{
		friend class DX12Device;
	public:
		DX12GraphicsPipelineDesc(DX12RootSignature sig)noexcept
			:sig(std::move(sig))
		{}
	public:
		DX12GraphicsPipelineDesc& SetVS(DX12Shader vs)noexcept
		{
			this->vs = std::move(vs); return *this;
		}
		DX12GraphicsPipelineDesc& SetPS(DX12Shader ps)noexcept
		{
			this->ps = std::move(ps); return *this;
		}
		DX12GraphicsPipelineDesc& SetGS(DX12Shader gs)noexcept
		{
			this->gs = std::move(gs); return *this;
		}
		DX12GraphicsPipelineDesc& SetHS(DX12Shader hs)noexcept
		{
			this->hs = std::move(hs); return *this;
		}
		DX12GraphicsPipelineDesc& SetDS(DX12Shader ds)noexcept
		{
			this->ds = std::move(ds); return *this;
		}
		DX12GraphicsPipelineDesc& SetShader(DX12Shader shader)noexcept
		{
			using enum ShaderType;
			switch (shader.GetType())
			{
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
		DX12GraphicsPipelineDesc& SetRenderTargets(std::span<const DataFormat> xtarget_formats)noexcept
		{
			for (size_t i = 0; i < target_formats.size() && i < 8; i++)
				target_formats[i] = xtarget_formats[i];
			return *this;
		}
		DX12GraphicsPipelineDesc& SetRenderTarget(DataFormat render_target, uint8_t slot)noexcept
		{
			if (slot > 7)return *this;
			if (target_formats[slot] == DataFormat::unknown)num_targets++;
			target_formats[slot] = render_target;
			return *this;
		}

	private:
		DX12RootSignature sig;
		DX12Shader vs;
		DX12Shader ps;
		DX12Shader gs;
		DX12Shader hs;
		DX12Shader ds;

		uint32_t num_targets = 0;
		std::array<DataFormat, 8> target_formats{};
	};

	// TODO: Mesh and Compute pipelines
}