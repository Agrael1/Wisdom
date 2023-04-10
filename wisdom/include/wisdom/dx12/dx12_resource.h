#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_buffer_views.h>
#include <d3dx12/d3dx12.h>
#include <D3D12MemAlloc.h>
#include <span>

namespace wis
{
	class DX12Resource;

	template<>
	class Internal<DX12Resource>
	{
	public:
		[[nodiscard]]
		ID3D12Resource* GetResource()const noexcept
		{
			return resource.get();
		}
	protected:
		winrt::com_ptr<ID3D12Resource> resource{};
		winrt::com_ptr<D3D12MA::Allocation> allocation{};
	};

	using DX12ResourceView = ID3D12Resource*;

	class DX12Resource : public QueryInternal<DX12Resource>
	{
	public:
		DX12Resource() = default;
		explicit DX12Resource(winrt::com_ptr<ID3D12Resource> rc, winrt::com_ptr<D3D12MA::Allocation> al)
		{
			resource = std::move(rc);
			allocation = std::move(al);
		}
		operator DX12ResourceView()const noexcept
		{
			return GetResource();
		}
	public:
		bool UpdateSubresource(std::span<const std::byte> data, uint32_t subresource = 0)
		{
			void* bytes = nullptr;
			if (!wis::succeded_weak(resource->Map(subresource, nullptr, &bytes)))
				return false;			
			
			std::copy(data.data(), data.data() + data.size(), (std::byte*)bytes);
			resource->Unmap(subresource, nullptr);
			return true;
		}
		[[nodiscard]]
		DX12VertexBufferView GetVertexBufferView(uint32_t byte_stride)
		{
			return DX12VertexBufferView{ D3D12_VERTEX_BUFFER_VIEW{resource->GetGPUVirtualAddress(), uint32_t(resource->GetDesc().Width), byte_stride}};
		}
	};
}