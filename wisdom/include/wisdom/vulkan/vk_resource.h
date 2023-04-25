#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>

namespace wis
{
	class VKBuffer;

	template<>
	class Internal<VKBuffer>
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

	using VKBufferView = vk::Buffer;

	class VKBuffer : public QueryInternal<VKBuffer>
	{
	public:
		VKBuffer() = default;
		explicit VKBuffer(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation)
			:QueryInternal(std::move(buffer), std::move(allocation))
		{}
		operator VKBufferView()const noexcept
		{
			return GetResource();
		}
	public:
		//[[nodiscard]]
		//DX12VertexBufferView GetVertexBufferView(uint32_t byte_stride)
		//{
		//	return DX12VertexBufferView{ D3D12_VERTEX_BUFFER_VIEW{resource->GetGPUVirtualAddress(), uint32_t(resource->GetDesc().Width), byte_stride} };
		//}
	};



	class VKTexture;

	template<>
	class Internal<VKTexture>
	{
		friend class VKSwapChain;
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Image> buffer, wis::shared_handle<vma::Allocation> allocation)
			:buffer(std::move(buffer)), allocation(std::move(allocation)) {}
	public:
		auto GetResource()const noexcept
		{
			return buffer.get();
		}
	protected:
		wis::shared_handle<vma::Allocation> allocation; //order mandated
		wis::shared_handle<vk::Image> buffer;
	};

	using VKTextureView = vk::Image;

	class VKTexture : public QueryInternal<VKTexture>
	{
	public:
		VKTexture() = default;
		explicit VKTexture(wis::shared_handle<vk::Image> buffer, wis::shared_handle<vma::Allocation> allocation = {})
			:QueryInternal(std::move(buffer), std::move(allocation))
		{}
		operator VKTextureView()const noexcept
		{
			return GetResource();
		}
	public:
		//[[nodiscard]]
		//DX12VertexBufferView GetVertexBufferView(uint32_t byte_stride)
		//{
		//	return DX12VertexBufferView{ D3D12_VERTEX_BUFFER_VIEW{resource->GetGPUVirtualAddress(), uint32_t(resource->GetDesc().Width), byte_stride} };
		//}
	};
}