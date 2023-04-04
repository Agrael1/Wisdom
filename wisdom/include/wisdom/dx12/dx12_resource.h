#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>

namespace wis
{
	class DX12Resource;

	template<>
	class Internal<DX12Resource>
	{
	public:
		template<class Self>
		[[nodiscard]]auto GetResource(this Self&& s)noexcept
		{
			return s.resource;
		}
	protected:
		winrt::com_ptr<ID3D12Resource> resource{};
	};

	class DX12Resource : public QueryInternal<DX12Resource>
	{
	public:
		DX12Resource(winrt::com_ptr<ID3D12Resource> rc)
		{
			resource = std::move(rc);
		}
	public:
	};
}