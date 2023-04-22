#pragma once
#include <wisdom/api/api_factory.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/util/generator.h>

#include <dxgi1_6.h>
#include <d3d12.h>


namespace wis
{
	class DX12Factory;
	inline constexpr DXGI_GPU_PREFERENCE to_dxgi(AdapterPreference pref)
	{
		switch (pref)
		{
		default:
		case wis::AdapterPreference::None:
			return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_UNSPECIFIED;
		case wis::AdapterPreference::MinConsumption:
			return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER;
		case wis::AdapterPreference::Performance:
			return DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
		}
	}


	template<>
	class Internal<DX12Factory>
	{
		static constexpr inline bool valid = true;
	public:
		Internal()
		{
			if (factory)
				factory->AddRef();
		}
		~Internal()
		{
			if (!factory->Release())
				factory.detach();
		}
		static IDXGIFactory4* GetFactory()noexcept {
			return factory.get();
		}
	protected:
		static inline winrt::com_ptr<IDXGIFactory4> factory{};
	};


	/// Main Factory class, since we don't need more than one factory it is a static resource
	/// Not thread safe on creation
	class DX12Factory final : public QueryInternal<DX12Factory>
	{
		friend class DX12Device;
		static inline constexpr uint32_t debug_flag = wis::debug_mode & DXGI_CREATE_FACTORY_DEBUG;
	public:
		DX12Factory(const ApplicationInfo& app_info, bool use_preference = true)
		{
			if (factory)
				return;
			EnableDebugLayer();
			wis::check_hresult(CreateDXGIFactory2(debug_flag,
				__uuidof(IDXGIFactory4), factory.put_void()));

			// TODO: consider constexpr
			has_preference = bool(factory.as<IDXGIFactory6>()) && use_preference;
		}
	public:
		[[nodiscard]]
		wis::generator<DX12Adapter> EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance)const noexcept
		{
			auto gen = has_preference ? AdaptersByGPUPreference(to_dxgi(preference)) : Adapters();
			for (auto&& i : gen)
				co_yield DX12Adapter(i);
		}
	private:
		void EnableDebugLayer() noexcept
		{
			if constexpr (wis::debug_mode) {
				winrt::com_ptr<ID3D12Debug> debugController;
				if (wis::succeded(D3D12GetDebugInterface(__uuidof(*debugController), debugController.put_void())))
					debugController->EnableDebugLayer();

				if (auto d1 = debugController.as<ID3D12Debug1>())
					d1->SetEnableGPUBasedValidation(true);
			}
		}

		wis::generator<winrt::com_ptr<IDXGIAdapter1>>
			AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference = DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE)const noexcept
		{
			winrt::com_ptr<IDXGIAdapter1> adapter;
			auto factory6 = factory.as<IDXGIFactory6>();
			uint32_t index = 0;

			while (SUCCEEDED(factory6->EnumAdapterByGpuPreference(index++,
				preference,
				__uuidof(IDXGIAdapter1), adapter.put_void())))
			{
				co_yield std::move(adapter);
			}
		}
		wis::generator<winrt::com_ptr<IDXGIAdapter1>>
			Adapters()const noexcept
		{
			winrt::com_ptr<IDXGIAdapter1> adapter;
			uint32_t index = 0;

			while (SUCCEEDED(factory->EnumAdapters1(index++, adapter.put())))
				co_yield std::move(adapter);
		}

		static winrt::com_ptr<IDXGISwapChain4> SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue)
		{
			winrt::com_ptr<IDXGISwapChain1> swap;
			wis::check_hresult(factory->CreateSwapChainForCoreWindow(
				queue,        // Swap chain needs the queue so that it can force a flush on it.
				core_window,
				&desc,
				nullptr,
				swap.put()
			));
			return swap.as<IDXGISwapChain4>();
		}
		static winrt::com_ptr<IDXGISwapChain4> SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue)
		{
			winrt::com_ptr<IDXGISwapChain1> swap;

			wis::check_hresult(factory->CreateSwapChainForHwnd(
				queue,        // Swap chain needs the queue so that it can force a flush on it.
				hwnd,
				&desc,
				nullptr,
				nullptr,
				swap.put()
			));
			return swap.as<IDXGISwapChain4>();
		}
	private:
		static inline bool has_preference = false;
		DX12Info info;
	};
}