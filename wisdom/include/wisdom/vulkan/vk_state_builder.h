#pragma once
#include <wisdom/vulkan/vk_shader.h>

namespace wis
{
	class VKGraphicsPipelineDesc
	{
		friend class VKDevice;
	public:
		//VKGraphicsPipelineDesc(VKRootSignature sig)noexcept
		//	:sig(std::move(sig))
		//{}
	public:
		VKGraphicsPipelineDesc& SetVS(VKShader vs)noexcept
		{
			this->vs = std::move(vs); return *this;
		}
		VKGraphicsPipelineDesc& SetPS(VKShader ps)noexcept
		{
			this->ps = std::move(ps); return *this;
		}
		VKGraphicsPipelineDesc& SetGS(VKShader gs)noexcept
		{
			this->gs = std::move(gs); return *this;
		}
		VKGraphicsPipelineDesc& SetHS(VKShader hs)noexcept
		{
			this->hs = std::move(hs); return *this;
		}
		VKGraphicsPipelineDesc& SetDS(VKShader ds)noexcept
		{
			this->ds = std::move(ds); return *this;
		}
		VKGraphicsPipelineDesc& SetShader(VKShader shader)noexcept
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
		//VKGraphicsPipelineDesc& SetRenderTargets(std::span<const DataFormat> xtarget_formats)noexcept
		//{
		//	for (size_t i = 0; i < target_formats.size() && i < 8; i++)
		//		target_formats[i] = xtarget_formats[i];
		//	return *this;
		//}
		//VKGraphicsPipelineDesc& SetRenderTarget(DataFormat render_target, uint8_t slot)noexcept
		//{
		//	if (slot > 7)return *this;
		//	if (target_formats[slot] == DataFormat::unknown)num_targets++;
		//	target_formats[slot] = render_target;
		//	return *this;
		//}

	private:
		//VKRootSignature sig;
		VKShader vs;
		VKShader ps;
		VKShader gs;
		VKShader hs;
		VKShader ds;

		uint32_t num_targets = 0;
		//std::array<DataFormat, 8> target_formats{};
	};
}