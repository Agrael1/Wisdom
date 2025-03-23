#ifndef WIS_DX12_FACTORY_CPP
#define WIS_DX12_FACTORY_CPP
#ifndef WISDOM_MODULE_DECL
#include <wisdom/dx12/dx12_factory.h>
#include <wisdom/dx12/dx12_checks.h>
#endif

wis::DX12Factory
wis::ImplDX12CreateFactory(wis::Result& res, bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept
{
    // Enable RVO
    DX12Factory f;
    auto& internal = f.GetMutableInternal();

    auto hr = CreateDXGIFactory2(enable_debug * DXGI_CREATE_FACTORY_DEBUG, internal.factory.iid(),
                                 internal.factory.put_void());

    if (!wis::succeeded(hr)) {
        res = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create DXGI factory">(hr);
        return f;
    }

    for (auto ext : std::span<DX12FactoryExtension*>{ extensions, extension_count }) {
        ext->Init(f);
    }
    return f;
}

wis::DX12Adapter
wis::ImplDX12Factory::GetAdapter(wis::Result& result, uint32_t index, AdapterPreference preference) const noexcept
{
    wis::DX12Adapter adapter;
    auto& internal = adapter.GetMutableInternal();

    auto hr = factory->EnumAdapterByGpuPreference(index, convert_dx(preference), internal.adapter.iid(),
                                                  internal.adapter.put_void());
    if (!wis::succeeded(hr)) {
        result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to get adapter">(hr);
    }
    return adapter;
}
#endif // !DX12_FACTORY_CPP
