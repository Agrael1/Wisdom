#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/vulkan/vk_shared_handle.h>
#include <wisdom/api/api_adapter.h>

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


	//AdapterFlags GetFlags()
	//{

	//}

	class VKAdapter : public QueryInternal<VKAdapter>
	{
	public:
		VKAdapter() = default;
		VKAdapter(vk::PhysicalDevice adapter)
		{
			this->adapter = adapter;
		}
	public:
		[[nodiscard]]
		AdapterDesc GetDesc()const noexcept
		{
			vk::PhysicalDeviceProperties2 properties;
			vk::PhysicalDeviceIDProperties id_props;
			properties.pNext = &id_props;
			DynamicLoader::loader.vkGetPhysicalDeviceProperties2(adapter, reinterpret_cast<VkPhysicalDeviceProperties2*>(&properties));

			auto& desc = properties.properties;
			auto desc2 = adapter.getMemoryProperties();

			//DXGI_ADAPTER_DESC1 desc;
			//adapter->GetDesc1(&desc);
			std::string_view x = desc.deviceName;

			return AdapterDesc{
				.description{x.begin(), x.end()},
				.vendor_id = desc.vendorID,
				.device_id = desc.deviceID,
				.subsys_id = desc.apiVersion,
				.revision = desc.driverVersion,

				//.dedicated_video_memory = desc2.DedicatedVideoMemory,
				//.dedicated_system_memory = desc.DedicatedSystemMemory,
				//.shared_system_memory = desc.SharedSystemMemory,
				.adapter_id{reinterpret_cast<uint64_t&>(id_props.deviceLUID)},
				//.flags = AdapterFlags(desc.Flags)
			};
		}

		operator VKAdapterView()const noexcept
		{
			return GetAdapter();
		}
	};
}