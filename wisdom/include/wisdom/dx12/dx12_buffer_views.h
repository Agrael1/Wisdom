#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_resource.h>
#include <d3d12.h>

namespace wis
{
	class DX12VertexBufferView;

	template<>
	class Internal<DX12VertexBufferView>
	{
	public:
		D3D12_VERTEX_BUFFER_VIEW GetView()const noexcept
		{
			return view;
		}
	protected:
		D3D12_VERTEX_BUFFER_VIEW view;
	};


	class DX12VertexBufferView : public QueryInternal<DX12VertexBufferView>
	{
	public:
		DX12VertexBufferView() = default;
		explicit DX12VertexBufferView(D3D12_VERTEX_BUFFER_VIEW xhandle)
		{
			view = xhandle;
		}
		operator D3D12_VERTEX_BUFFER_VIEW()const noexcept
		{
			return view;
		}
	};
}