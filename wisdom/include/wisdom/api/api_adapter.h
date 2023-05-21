#pragma once
#include <wisdom/util/flags.h>
#include <wisdom/bridge/format.h>


namespace wis
{
	enum class AdapterFlags : uint32_t
	{
		None = 0,
		Remote = 1,
		Software = 2,
		ACGCompatible = 4,

		//mostly unused 
		SUPPORT_MONITORED_FENCES = 8,
		SUPPORT_NON_MONITORED_FENCES = 0x10,
		KEYED_MUTEX_CONFORMANCE = 0x20,
	};


	struct AdapterDesc
	{
		std::wstring description;
		uint32_t vendor_id;
		uint32_t device_id;
		uint32_t subsys_id;
		uint32_t revision;

		size_t dedicated_video_memory;
		size_t dedicated_system_memory;
		size_t shared_system_memory;
		uint64_t adapter_id;

		AdapterFlags flags = AdapterFlags::None;

		bool IsSoftware()const noexcept{
			using namespace river::flags;
			return flags & AdapterFlags::Software;
		}
		std::wstring to_string()const noexcept
		{
			return wis::format(
				L"[description]: {}\n"
				L"[vendor id]: 0x{:X}\n"
				L"[device id]: 0x{:X}\n"
				L"[subsys id]: 0x{:X}\n"
				L"[revision]: {}\n"
				L"[dedicated video memory]: {}\n"
				L"[dedicated system memory]: {}\n"
				L"[shared system memory]: {}\n"
				L"[adapter id]: {:X}\n",
				description, vendor_id, device_id, subsys_id, revision, dedicated_video_memory, dedicated_system_memory, shared_system_memory, adapter_id
			);
		}
	};
}