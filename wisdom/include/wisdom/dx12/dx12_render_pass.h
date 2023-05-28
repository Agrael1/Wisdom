#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/api/api_common.h>
#include <wisdom/global/definitions.h>


namespace wis
{
	class DX12RenderPass;

	template<>
	class Internal<DX12RenderPass>
	{
	public:
		Internal() = default;
		Internal(wis::internals::uniform_allocator<DataFormat, max_render_targets> target_formats, 
			std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs, 
			std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc)
			:target_formats(target_formats), rt_descs(std::move(rt_descs)), ds_desc(std::move(ds_desc))
		{}
	public:
		std::span<D3D12_RENDER_PASS_RENDER_TARGET_DESC> GetRTDescs()const noexcept{
			return rt_descs;
		}
		D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* GetDSDesc()const noexcept {
			return ds_desc.has_value() ? &ds_desc.value() : nullptr;
		}
		const wis::internals::uniform_allocator<DataFormat, max_render_targets> GetTargetFormats()const noexcept
		{
			return target_formats;
		}
		std::span<const DataFormat> GetTargetFormatSpan()const noexcept
		{
			return { target_formats.data(), target_formats.size() };
		}
	private:
		wis::internals::uniform_allocator<DataFormat, max_render_targets> target_formats;
		mutable std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs;
		mutable std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc;
	};

	using DX12RenderPassView = const DX12RenderPass&; //rp is too large

	// TODO: better RenderPass
	class DX12RenderPass : public QueryInternal<DX12RenderPass>
	{
	public:
		DX12RenderPass() = default;
		explicit DX12RenderPass(wis::internals::uniform_allocator<DataFormat, max_render_targets> target_formats,
			std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rt_descs, 
			std::optional<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> ds_desc = {})
			:QueryInternal(target_formats, std::move(rt_descs), std::move(ds_desc))
		{};
	public:
	};
}