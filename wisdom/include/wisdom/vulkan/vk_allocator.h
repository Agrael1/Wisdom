#pragma once
#include <wisdom/vulkan/vk_device.h>
#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_allocator_handles.h>
#include <wisdom/vulkan/vk_resource.h>

namespace wis
{
	class VKResourceAllocator;

	template<>
	class Internal<VKResourceAllocator>
	{
	public:
		VmaAllocator GetAllocator()const noexcept
		{
			return allocator.get();
		}
	protected:
		wis::shared_handle<vma::Allocator> allocator;
	};

	//move only
	class VKResourceAllocator : public QueryInternal<VKResourceAllocator>
	{
	public:
		VKResourceAllocator() = default;
		VKResourceAllocator(VKDeviceView device, VKAdapterView adapter)
		{
			vma::AllocatorCreateInfo allocatorInfo
			{
				vma::AllocatorCreateFlags(0),
				adapter,
				device.get(),
				0,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				VKFactory::GetInstance(),
				VKFactory::GetApiVer()
			};

			vma::Allocator al;
			[[maybe_unused]]
			auto res = vma::createAllocator(&allocatorInfo, &al);

			allocator = wis::shared_handle<vma::Allocator>{ al, std::move(device) };
		}
	public:
		[[nodiscard]]
		VKBuffer CreatePersistentBuffer(size_t size)
		{
			vk::BufferCreateInfo desc{
				{}, size, vk::BufferUsageFlagBits::eTransferDst,
				vk::SharingMode::eExclusive, 0, nullptr, nullptr
			};

			vma::AllocationCreateInfo alloc{
				{}, vma::MemoryUsage::eAuto
			};
			auto[a,b] = allocator->createBuffer(desc, alloc);
			return VKBuffer{ wis::shared_handle<vk::Buffer>{a, allocator.get_device_handle()}, wis::shared_handle<vma::Allocation>{b, allocator} };
		}
	};
}