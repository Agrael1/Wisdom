#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>


namespace wis
{
	class DX12RenderPass;

	template<>
	class Internal<DX12RenderPass>
	{
	public:
		Internal() = default;
		Internal(std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs, std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc)
			:rt_descs(std::move(rt_descs)), ds_desc(std::move(ds_desc))
		{}
	public:
		std::span<D3D12_RENDER_PASS_RENDER_TARGET_DESC> GetRTDescs()const noexcept{
			return rt_descs;
		}
		D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* GetDSDesc()const noexcept {
			return ds_desc.has_value() ? &ds_desc.value() : nullptr;
		}
	private:
		mutable std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs;
		mutable std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc;
	};

	class DX12RenderPass : public QueryInternal<DX12RenderPass>
	{
	public:
		DX12RenderPass() = default;
		explicit DX12RenderPass(std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs, std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc = {})
		{};
	public:
	};
}