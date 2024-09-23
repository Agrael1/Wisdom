#ifndef DX12_FACTORY_CPP
#define DX12_FACTORY_CPP
#ifdef WISDOM_BUILD_BINARIES
#include <wisdom/dx12/dx12_factory.h>
#endif // !WISDOM_HEADER_ONLY

#include <wisdom/dx12/dx12_checks.h>

wis::ImplDX12Factory::ImplDX12Factory(wis::com_ptr<IDXGIFactory6> factory) noexcept
    : QueryInternal(std::move(factory))
{
}

wis::ResultValue<wis::DX12Factory>
wis::ImplDX12CreateFactory(bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept
{
    std::span<DX12FactoryExtension*> exts{
        extensions, extensions + extension_count
    };

    wis::com_ptr<IDXGIFactory6> factory;
    auto hr = CreateDXGIFactory2(enable_debug * DXGI_CREATE_FACTORY_DEBUG, __uuidof(*factory),
                                 factory.put_void());

    if (!wis::succeeded(hr)) {
        hr = CreateDXGIFactory2(enable_debug * DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory4),
                                factory.put_void());
        DX12Factory::has_preference = false;
        if (!wis::succeeded(hr)) {
            return wis::make_result<FUNC, "Failed to create DXGI factory">(hr);
        }
    }
    auto f = DX12Factory(std::move(factory));

    for (auto ext : exts) {
        ext->Init(f);
    }
    return std::move(f);
}

wis::ResultValue<wis::DX12Adapter>
wis::ImplDX12Factory::GetAdapter(uint32_t index, AdapterPreference preference) const noexcept
{
    auto gen = has_preference ? GetAdapterByGPUPreference(index, convert_dx(preference))
                              : GetAdapter1(index);

    if (!gen)
        return wis::make_result<FUNC, "Failed to get adapter">(gen.result);

    return wis::DX12Adapter(std::move(gen.ptr));
}

wis::com_with_result<IDXGIAdapter1>
wis::ImplDX12Factory::GetAdapterByGPUPreference(uint32_t index,
                                                DXGI_GPU_PREFERENCE preference) const noexcept
{
    wis::com_ptr<IDXGIAdapter1> adapter;
    auto hr = factory->EnumAdapterByGpuPreference(index, preference, __uuidof(*adapter),
                                                  adapter.put_void());
    return { hr, std::move(adapter) };
}

wis::com_with_result<IDXGIAdapter1>
wis::ImplDX12Factory::GetAdapter1(uint32_t index) const noexcept
{
    wis::com_ptr<IDXGIAdapter1> adapter;
    auto hr = factory->EnumAdapters1(index, adapter.put());
    return { hr, std::move(adapter) };
}

#endif // !DX12_FACTORY_CPP
