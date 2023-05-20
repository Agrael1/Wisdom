#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/api/api_adapter.h>
#include <span>

namespace wis
{
	class VKAdapter;

	template<>
	class Internal<VKAdapter>
	{
	public:
		[[nodiscard]]
		vk::PhysicalDevice GetAdapter()const noexcept
		{
			return adapter;
		}
	protected:
		vk::PhysicalDevice adapter;
	};
	using VKAdapterView = vk::PhysicalDevice;

	class VKAdapter : public QueryInternal<VKAdapter>
	{
	public:
		VKAdapter() = default;
		explicit VKAdapter(vk::PhysicalDevice adapter)
		{
			this->adapter = adapter;
		}
	public:
		[[nodiscard]]
		AdapterDesc GetDesc()const noexcept
		{
			using namespace river::flags;
			vk::PhysicalDeviceProperties2 properties;
			vk::PhysicalDeviceIDProperties id_props;
			properties.pNext = &id_props;
			DynamicLoader::loader.vkGetPhysicalDeviceProperties2(adapter, reinterpret_cast<VkPhysicalDeviceProperties2*>(&properties));

			auto& desc = properties.properties;
			auto desc2 = adapter.getMemoryProperties();

			uint64_t local_mem = 0;
			uint64_t system_mem = 0;
			std::span types{desc2.memoryTypes.data(), desc2.memoryTypeCount};
			for (auto& i : types)
			{
				if (i.propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal &&
					desc2.memoryHeaps[i.heapIndex].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
				{
					local_mem = desc2.memoryHeaps[i.heapIndex].size;
				}

				if (i.propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent)
				{
					system_mem = desc2.memoryHeaps[i.heapIndex].size;
				}
				if (system_mem && local_mem)break;
			}

			AdapterFlags flag{AdapterFlags(+AdapterFlags::Remote && (uint32_t(desc.deviceType) & uint32_t(vk::PhysicalDeviceType::eVirtualGpu)))
			| AdapterFlags(+AdapterFlags::Software && (uint32_t(desc.deviceType) & uint32_t(vk::PhysicalDeviceType::eCpu))) };

			std::string_view x = desc.deviceName;
			return AdapterDesc{
				.description{x.begin(), x.end()},
				.vendor_id = desc.vendorID,
				.device_id = desc.deviceID,
				.subsys_id = desc.apiVersion,
				.revision = desc.driverVersion,

				.dedicated_video_memory = local_mem,
				.dedicated_system_memory = 0,
				.shared_system_memory = system_mem,
				.adapter_id = reinterpret_cast<uint64_t&>(id_props.deviceLUID),
				.flags = flag
			};
		}

		operator VKAdapterView()const noexcept
		{
			return GetAdapter();
		}
	};
}