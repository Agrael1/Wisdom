#pragma once
#include <wisdom/api/api_internal.h>
#include <d3dx12/d3dx12.h>

namespace wis
{
	class DX12RenderTargetView;

	template<>
	class Internal<DX12RenderTargetView>
	{
	public:
		auto GetHandle()const noexcept
		{
			return handle;
		}
	protected:
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
	};


	class DX12RenderTargetView : public QueryInternal<DX12RenderTargetView>
	{
	public:
		DX12RenderTargetView() = default;
		explicit DX12RenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE xhandle)
		{
			handle = xhandle;
		}
	};
}