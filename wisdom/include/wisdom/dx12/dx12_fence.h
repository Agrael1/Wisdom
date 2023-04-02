#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wil/resource.h>

namespace wis
{
	class DX12Fence;

	template<>
	class Internal<DX12Fence>
	{
	protected:
		winrt::com_ptr<ID3D12Fence1> fence;
		wil::unique_event_nothrow fence_event;
	};

	class DX12Fence : public QueryInternal<DX12Fence>
	{
	public:
		DX12Fence() = default;
		explicit DX12Fence(winrt::com_ptr<ID3D12Fence1> xfence)
		{
			fence = std::move(xfence);
			wis::check_hresult(fence_event.create()); // rethrow windows error (nothrow policy)
		}
	};
}