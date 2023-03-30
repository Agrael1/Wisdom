#pragma once
#include <wisdom/util/flags.h>
#include <string>
#include <array>
#include <ranges>

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
		FORCE_DWORD = 0xffffffff
	};


	struct AdapterDesc
	{
		static inline constexpr uint32_t uid_length = 8;

		std::wstring description;
		uint32_t vendor_id;
		uint32_t device_id;
		uint32_t subsys_id;
		uint32_t revision;

		size_t dedicated_video_memory;
		size_t dedicated_system_memory;
		size_t shared_system_memory;
		std::array<uint8_t, uid_length> adapter_id;

		AdapterFlags flags = AdapterFlags::None;

		bool IsSoftware()const noexcept{
			using namespace river::flags;
			return flags & AdapterFlags::Software;
		}
		std::wstring to_string()const noexcept
		{
			return std::format(
				L"[description]: {}\n"
				L"[vendor id]: 0x{:X}\n"
				L"[device id]: 0x{:X}\n"
				L"[subsys id]: 0x{:X}\n"
				L"[revision]: {}\n"
				L"[dedicated video memory]: {}\n"
				L"[dedicated system memory]: {}\n"
				L"[shared system memory]: {}\n"
				L"[adapter id]: {}\n",
				description, vendor_id, device_id, subsys_id, revision, dedicated_video_memory, dedicated_system_memory, shared_system_memory, GetAdapterID()
			);
		}
		std::wstring GetAdapterID()const noexcept
		{
			std::wstring ws;
			ws.reserve(uid_length * 3);
			for (auto i : std::views::reverse(adapter_id))
			{
				std::format_to(std::back_inserter(ws), L"{:02X}:", i);
			}
			return ws;
		}
	};
}