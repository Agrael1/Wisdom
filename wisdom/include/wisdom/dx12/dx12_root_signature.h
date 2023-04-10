#pragma once
#include <wisdom/api/api_internal.h>
#include <d3d12.h>
#include <winrt/base.h>

namespace wis
{
	class DX12RootSignature;

	template<>
	class Internal<DX12RootSignature>
	{
	public:
		ID3D12RootSignature* GetRootSignature()const noexcept
		{
			return root.get();
		}
	protected:
		winrt::com_ptr<ID3D12RootSignature> root;
	};

	using DX12RootSignatureView = ID3D12RootSignature*;

	class DX12RootSignature : public QueryInternal<DX12RootSignature>
	{
	public:
		DX12RootSignature() = default;
		explicit DX12RootSignature(winrt::com_ptr<ID3D12RootSignature> xroot)
		{
			root = std::move(xroot);
		}
	public:
		operator DX12RootSignatureView()const noexcept
		{
			return GetRootSignature();
		}
	};
}