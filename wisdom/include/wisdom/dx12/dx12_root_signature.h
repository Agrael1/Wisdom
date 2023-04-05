#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>

namespace wis
{
	class DX12RootSignature;

	template<>
	class Internal<DX12RootSignature>
	{
	public:
		auto GetRootSignature()const noexcept
		{
			return root;
		}
	protected:
		winrt::com_ptr<ID3D12RootSignature> root;
	};

	class DX12RootSignature : public QueryInternal<DX12RootSignature>
	{
	public:
		DX12RootSignature() = default;
		explicit DX12RootSignature(winrt::com_ptr<ID3D12RootSignature> xroot)
		{
			root = std::move(xroot);
		}
	public:

	};
}