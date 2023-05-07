#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_buffer_views.h>

namespace wis
{
	class VKBuffer;

	template<>
	class Internal<VKBuffer>
	{
	public:
		Internal() = default;
		Internal(wis::shared_handle<vk::Buffer> buffer, wis::shared_handle<vma::Allocation> allocation)
			:buffer(std::move(buffer)), allocation(std::move(allocation)) {}
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
		bool UpdateSubresource(std::span<const std::byte> data)
		{
			auto vma = allocation.get_parent();
			auto al = allocation.get();
			auto* mem = vma.mapMemory(al);
			if (!mem)return false;

			auto al_size = vma.getAllocationInfo(al).size;
			auto data_size = data.size();
			std::memcpy(mem, data.data(), data_size > al_size ? al_size : data_size);
			vma.unmapMemory(al);
			return true;
		}
		[[nodiscard]]
		VKVertexBufferView GetVertexBufferView(uint32_t byte_stride)
		{
			return VKVertexBufferView{ buffer.get(), allocation.get_parent().getAllocationInfo(allocation.get()).size, byte_stride };
		}
	};



	class VKTexture;

	template<>
	class Internal<VKTexture>
	{
		friend class VKSwapChain;
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

	using VKTextureView = struct { vk::Image image; vk::Format format; uint32_t width; uint32_t height;	};

	class VKTexture : public QueryInternal<VKTexture>
	{
	public:
		VKTexture() = default;
		explicit VKTexture(vk::Format format, wis::shared_handle<vk::Image> buffer, wis::shared_handle<vma::Allocation> allocation = {})
			:QueryInternal(std::move(buffer), std::move(allocation), format)
		{}
		operator VKTextureView()const noexcept
		{
			return { GetResource(), format, width, height };
		}
	public:
		std::pair<uint32_t, uint32_t> GetSize()const noexcept
		{
			return{ width, height };
		}
	protected:
		uint32_t width;
		uint32_t height;
	};
}