#pragma once
#include <wisdom/vulkan/vk_shader.h>
#include <wisdom/vulkan/vk_root_signature.h>
#include <wisdom/vulkan/vk_render_pass.h>

namespace wis
{
	class VKGraphicsPipelineDesc
	{
		friend class VKDevice;
	public:
		VKGraphicsPipelineDesc(VKRootSignature sig)noexcept
			:sig(std::move(sig))
		{}
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
		VKGraphicsPipelineDesc& SetRenderPass(VKRenderPass pass)noexcept
		{
			this->pass = std::move(pass);
			return *this;
		}
	private:
		VKRootSignature sig;
		VKShader vs;
		VKShader ps;
		VKShader gs;
		VKShader hs;
		VKShader ds;
		VKRenderPass pass;
	};
}