#pragma once
#include <wisdom/api/api_factory.h>
#include <wisdom/dx12/dx12_adapter.h>
#include <wisdom/dx12/dx12_command_queue.h>
#include <wisdom/dx12/dx12_info.h>
#include <wisdom/bridge/generator.h>

#include <d3d11.h> // fix for stereo swapchain


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
	public:
		[[nodiscard]]
		static IDXGIFactory4* GetFactory()noexcept {
			return factory.get();
		}
	protected:
		static inline winrt::com_ptr<IDXGIFactory4> factory{};
	};


	/// @brief Main Factory class, since we don't need more than one factory it is a static resource
	/// @note Not thread safe on creation
	class DX12Factory final : public QueryInternal<DX12Factory>
	{
		friend class DX12Device;
		friend class DX12Surface;
		static inline constexpr uint32_t debug_flag = wis::debug_mode & DXGI_CREATE_FACTORY_DEBUG;
	public:
		/// @brief Creates a new factory
		/// @param app_info Application info, not used
		///	@param use_preference Use the preference when enumerating adapters
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
		/// @brief Enumerates all adapters on the system
		/// @param preference Preference to use when enumerating adapters, changes the order of the adapters
		/// @return coroutine that yields DX12Adapter
		[[nodiscard]]
		wis::generator<DX12Adapter> EnumerateAdapters(AdapterPreference preference = AdapterPreference::Performance)const noexcept
		{
			auto gen = has_preference ? AdaptersByGPUPreference(to_dxgi(preference)) : Adapters();
			for (auto&& i : gen)
				co_yield DX12Adapter(i);
		}
	public:
		/// @brief Used as a direct bridge to the IDXGIFactory, used only by internal classes
		/// @return Factory interface
		[[nodiscard]]
		static auto GetFactory()noexcept
		{
			return factory;
		}
	private:

		/// @brief enables the debug layer if debug mode is enabled by compiler
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

		/// @brief A fix for stereo swapchain
		/// @return D3D11 device
		[[nodiscard]]
		static auto CreateD3D11Device() noexcept
		{
			constexpr D3D_FEATURE_LEVEL featureLevels[]
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1
			};

			winrt::com_ptr<ID3D11Device> device11;
			D3D11CreateDevice(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr, 0,
				featureLevels, 3, D3D11_SDK_VERSION, device11.put(), nullptr, nullptr);
			return device11;
		}

		/// @brief Creates a swapchain for a core window
		/// @param desc Description of the swapchain
		/// @param core_window Core window to create the swapchain for
		/// @param queue Queue to use for the swapchain
		/// @return Handle to the swapchain
		[[nodiscard]]
		static winrt::com_ptr<IDXGISwapChain4> SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue)
		{
			winrt::com_ptr<IDXGISwapChain1> swap;
			if (desc.Stereo) //until microsoft fixes this
			{
				wis::check_hresult(factory->CreateSwapChainForCoreWindow(
					CreateD3D11Device().get(),
					core_window,
					&desc,
					nullptr,
					swap.put()
				));
			}

			wis::check_hresult(factory->CreateSwapChainForCoreWindow(
				queue,        // Swap chain needs the queue so that it can force a flush on it.
				core_window,
				&desc,
				nullptr,
				swap.put()
			));
			return swap.as<IDXGISwapChain4>();
		}

		/// @brief Creates a swapchain for a win32 window
		/// @param desc Description of the swapchain
		/// @param hwnd Window handle to create the swapchain for
		/// @param queue Queue to use for the swapchain
		/// @return Handle to the swapchain
		[[nodiscard]]
		static winrt::com_ptr<IDXGISwapChain4> SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue)
		{
			winrt::com_ptr<IDXGISwapChain1> swap;
			if (desc.Stereo) //until microsoft fixes this
			{
				wis::check_hresult(factory->CreateSwapChainForHwnd(
					CreateD3D11Device().get(),        // Swap chain needs the queue so that it can force a flush on it.
					hwnd,
					&desc,
					nullptr,
					nullptr,
					swap.put()
				));
			}

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
		static inline bool has_preference = false; //< if the user has specified a preference for adapters
		DX12Info info; //< infromation queue for debugging and error messages
	};
}