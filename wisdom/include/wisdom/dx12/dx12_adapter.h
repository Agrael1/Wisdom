#pragma once
#include <wisdom/api/api_internal.h>
#include <wisdom/api/api_adapter.h>

#include <winrt/base.h>
#include <dxgi1_6.h>

namespace wis
{
	class DX12Adapter;

	template<>
	class Internal<DX12Adapter>
	{
		static constexpr inline bool valid = true;
	protected: 
		explicit Internal(winrt::com_ptr<IDXGIAdapter1> adapter)noexcept
			:adapter(std::move(adapter)) {}
	public:
		template<class Self>
		[[nodiscard]] auto GetAdapter(this Self&& s) {
			return s.adapter;
		}
	protected:
		winrt::com_ptr<IDXGIAdapter1> adapter;
	};


	class DX12Adapter final : Internal<DX12Adapter>
	{
		using intern = Internal<DX12Adapter>;
	public:
		DX12Adapter() = default;
		explicit DX12Adapter(winrt::com_ptr<IDXGIAdapter1> adapter)noexcept
			:intern(std::move(adapter))
		{}
	public:
		[[nodiscard]]
		AdapterDesc GetDesc()const noexcept
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			return AdapterDesc{
				.description{desc.Description},
				.vendor_id = desc.VendorId,
				.device_id = desc.DeviceId,
				.subsys_id = desc.SubSysId,
				.revision = desc.Revision,

				.dedicated_video_memory = desc.DedicatedVideoMemory,
				.dedicated_system_memory = desc.DedicatedSystemMemory,
				.shared_system_memory = desc.SharedSystemMemory,
				.adapter_id{reinterpret_cast<uint64_t&>(desc.AdapterLuid)},
				.flags = AdapterFlags(desc.Flags)
			};
		}
	public:
		[[nodiscard]]
		auto& GetInternal()const noexcept
		{
			return static_cast<const intern&>(*this);
		}
	};
}