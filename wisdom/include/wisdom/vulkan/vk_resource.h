#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_buffer_views.h>
#include <wisdom/vulkan/vk_views.h>
#endif

WIS_EXPORT namespace wis
{
	class VKBuffer;

	template<>
	class Internal<VKBuffer>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation, vk::DeviceSize size)
			:buffer(std::move(buffer)), allocation(std::move(allocation)), size(size) {}
	public:
		auto GetResource()const noexcept
		{
			return buffer.get();
		}
		auto GetAllocation()const noexcept
		{
			return allocation.get();
		}
		auto GetSize()const noexcept
		{
			return size;
		}
	protected:
		wis::shared_handle<vma::Allocation> allocation; //order mandated
		wis::shared_handle<vk::Buffer> buffer;
		vk::DeviceSize size = 0u;
	};


	class VKBuffer : public QueryInternal<VKBuffer>
	{
	public:
		VKBuffer() = default;
		explicit VKBuffer(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation, size_t size)
			:QueryInternal(std::move(buffer), std::move(allocation), size)
		{}
		operator VKBufferView()const noexcept
		{
			return GetResource();
		}
	public:
		bool UpdateSubresource(std::span<const std::byte> data)noexcept
		{
			auto vma = allocation.get_parent();
			auto al = allocation.get();
			auto* mem = vma.mapMemory(al);
			if (!mem)return false;

			auto data_size = data.size();
			std::memcpy(mem, data.data(), data_size > size ? size : data_size);
			vma.unmapMemory(al);
			return true;

			
		}
		[[nodiscard]]
		VKVertexBufferView GetVertexBufferView(uint32_t byte_stride)const noexcept
		{
			return VKVertexBufferView{ buffer.get(), size, byte_stride };
		}
	};



	class VKTexture;

	template<>
	class Internal<VKTexture>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Image> buffer, wis::shared_handle<vma::Allocation> allocation, vk::Format format)
			:buffer(std::move(buffer)), allocation(std::move(allocation)), format(format) {}
	public:
		auto GetResource()const noexcept
		{
			return buffer.get();
		}
	protected:
		wis::shared_handle<vma::Allocation> allocation; //order mandated
		wis::shared_handle<vk::Image> buffer;
		vk::Format format;
	};


	class VKTexture : public QueryInternal<VKTexture>
	{
	public:
		VKTexture() = default;
		explicit VKTexture(vk::Format format, wis::shared_handle<vk::Image> buffer, wis::shared_handle<vma::Allocation> allocation = {})
			:QueryInternal(std::move(buffer), std::move(allocation), format)
		{}
		operator VKTextureView()const noexcept
		{
			return { GetResource(), format };
		}
	};
}