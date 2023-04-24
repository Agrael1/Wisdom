#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>

namespace wis
{
	class VKResource;

	template<>
	class Internal<VKResource>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation)
			:buffer(std::move(buffer)), allocation(std::move(allocation)){}
	public:
		auto GetResource()const noexcept
		{
			return buffer.get();
		}
	protected:
		wis::shared_handle<vma::Allocation> allocation; //order mandated
		wis::shared_handle<vk::Buffer> buffer;
	};

	using VKResourceView = vk::Buffer;

	class VKResource : public QueryInternal<VKResource>
	{
	public:
		VKResource() = default;
		explicit VKResource(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation)
			:QueryInternal(std::move(buffer), std::move(allocation))
		{}
		operator VKResourceView()const noexcept
		{
			return GetResource();
		}
	public:
		bool UpdateSubresource(std::span<const std::byte> data, uint32_t subresource = 0)
		{
			
		}
		//[[nodiscard]]
		//DX12VertexBufferView GetVertexBufferView(uint32_t byte_stride)
		//{
		//	return DX12VertexBufferView{ D3D12_VERTEX_BUFFER_VIEW{resource->GetGPUVirtualAddress(), uint32_t(resource->GetDesc().Width), byte_stride} };
		//}
	};
}