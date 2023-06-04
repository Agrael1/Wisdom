#ifndef WISDOM_MODULES
//#include <wisdom/vulkan/vk_allocator.h>
#include <wisdom/vulkan/vk_factory.h>
#endif

wis::VKResourceAllocator::VKResourceAllocator(VKDeviceView device, VKAdapterView adapter)
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
	
	std::ignore = vma::createAllocator(&allocatorInfo, &al);

	allocator = wis::shared_handle<vma::Allocator>{ al, std::move(device) };
}