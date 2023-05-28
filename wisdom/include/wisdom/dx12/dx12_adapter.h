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
	public:
		Internal() = default;
		Internal(winrt::com_ptr<IDXGIAdapter1> adapter)noexcept
			:adapter(std::move(adapter)) {}
	public:
		[[nodiscard]] 
		IDXGIAdapter1* GetAdapter()const noexcept {
			return adapter.get();
		}
	protected:
		winrt::com_ptr<IDXGIAdapter1> adapter;
	};
	using DX12AdapterView = IDXGIAdapter1*;


	/// @brief DX12 physical adapter
	class DX12Adapter final : public QueryInternal<DX12Adapter>
	{
	public:
		DX12Adapter() = default;
		explicit DX12Adapter(winrt::com_ptr<IDXGIAdapter1> adapter)noexcept
			:QueryInternal(std::move(adapter))
		{}

		/// @brief Get the adapter internal view
		/// @return Adapter internal view
		/// @note Do not use the contents of a view directly unless you know what you are doing
		operator DX12AdapterView()const noexcept
		{
			return GetAdapter();
		}
	public:

		/// @brief Get the adapter description
		/// @return Adapter Description
		/// @note This function is thread safe
		[[nodiscard]]
		AdapterDesc GetDesc()const noexcept
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			return AdapterDesc{
				.description {winrt::to_string(desc.Description)},
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
	};
}
