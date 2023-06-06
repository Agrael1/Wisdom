#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/util/flags.h>
#include <wisdom/bridge/format.h>
#endif // !WISDOM_MODULES



WIS_EXPORT namespace wis
{
	/// @brief Adapter flags used to describe the adapter properties
	enum class AdapterFlags : uint32_t
	{
		None = 0, //< Adapter is a hardware adapter
		Remote = 1, //< Adapter is a remote adapter
		Software = 2, //< Adapter is a software adapter
		ACGCompatible = 4, //< Adapter is Active Code Guard compatible (DX 12.1 only)

		// unused 
		SUPPORT_MONITORED_FENCES = 8,  //< Adapter supports monitored fences (DX 12.1)
		SUPPORT_NON_MONITORED_FENCES = 0x10, //< Adapter supports non-monitored fences (DX 12.1)
		KEYED_MUTEX_CONFORMANCE = 0x20, //< Adapter supports KeyedMutex (DX 12.1)
	};


	/// @brief Adapter description, contains information about the adapter capabilities
	struct AdapterDesc
	{
	public:
		/// @brief Checks if the adapter is a software adapter
		/// @return true if the adapter is a software adapter
		bool IsSoftware()const noexcept{
			using namespace river::flags;
			return flags & AdapterFlags::Software;
		}

		/// @brief Outputs a string representation of the adapter properties
		/// @return String representation of the adapter properties
		std::string to_string()const noexcept
		{
			return wis::format(
				"[description]: {}\n"
				"[vendor id]: 0x{:X}\n"
				"[device id]: 0x{:X}\n"
				"[subsys id]: 0x{:X}\n"
				"[revision]: {}\n"
				"[dedicated video memory]: {}\n"
				"[dedicated system memory]: {}\n"
				"[shared system memory]: {}\n"
				"[adapter id]: {:X}\n",
				description, vendor_id, device_id, subsys_id, revision, dedicated_video_memory, dedicated_system_memory, shared_system_memory, adapter_id
			);
		}
	public:
		std::string description; //< Adapter description
		uint32_t vendor_id; //< Adapter vendor id
		uint32_t device_id; //< Adapter device id
		uint32_t subsys_id; //< Adapter subsystem id (DX12)/ api version (Vulkan)
		uint32_t revision; //< Adapter revision (DX12)/ driver version (Vulkan)

		size_t dedicated_video_memory; //< Dedicated video memory
		size_t dedicated_system_memory; //< Dedicated system memory (DX12 only)
		size_t shared_system_memory; //< Shared system memory
		uint64_t adapter_id; //< Adapter id(LUID)

		AdapterFlags flags = AdapterFlags::None; //< Adapter flags describing the adapter properties
	};
}