#include <d3d11.h> // fix for stereo swapchain
#include <wisdom/dx12/dx12_checks.h>

inline constexpr DXGI_GPU_PREFERENCE to_dxgi(wis::AdapterPreference pref)
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

wis::DX12Factory::DX12Factory(const ApplicationInfo& app_info, bool use_preference)
{
	if (factory)
		return;
	EnableDebugLayer();
	wis::check_hresult(CreateDXGIFactory2(debug_flag,
		__uuidof(IDXGIFactory4), factory.put_void()));

	// TODO: consider constexpr
	has_preference = bool(factory.as<IDXGIFactory6>()) && use_preference;
}

wis::generator<wis::DX12Adapter> wis::DX12Factory::EnumerateAdapters(wis::AdapterPreference preference)const noexcept
{
	auto gen = has_preference ? AdaptersByGPUPreference(to_dxgi(preference)) : Adapters();
	for (auto&& i : gen)
		co_yield DX12Adapter(i);
}

void wis::DX12Factory::EnableDebugLayer() noexcept
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
wis::DX12Factory::AdaptersByGPUPreference(DXGI_GPU_PREFERENCE preference)const noexcept
{
	winrt::com_ptr<IDXGIAdapter1> adapter;
	auto factory6 = factory.as<IDXGIFactory6>();
	uint32_t index = 0;

	while (wis::succeded_weak(factory6->EnumAdapterByGpuPreference(index++,
		preference,
		__uuidof(IDXGIAdapter1), adapter.put_void())))
	{
		co_yield std::move(adapter);
	}
}

wis::generator<winrt::com_ptr<IDXGIAdapter1>>
wis::DX12Factory::Adapters()const noexcept
{
	winrt::com_ptr<IDXGIAdapter1> adapter;
	uint32_t index = 0;

	while (wis::succeded_weak(factory->EnumAdapters1(index++, adapter.put())))
		co_yield std::move(adapter);
}

inline winrt::com_ptr<ID3D11Device> CreateD3D11Device() noexcept
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

winrt::com_ptr<IDXGISwapChain4> 
wis::DX12Factory::SwapChainForCoreWindow(const DXGI_SWAP_CHAIN_DESC1& desc, IUnknown* core_window, IUnknown* queue)
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

winrt::com_ptr<IDXGISwapChain4>
wis::DX12Factory::SwapChainForWin32(const DXGI_SWAP_CHAIN_DESC1& desc, HWND hwnd, IUnknown* queue)
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