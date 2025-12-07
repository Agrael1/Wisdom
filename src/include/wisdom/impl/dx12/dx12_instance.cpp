#ifndef WIS_DX12_INSTANCE_CPP
#define WIS_DX12_INSTANCE_CPP
#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/com_ptr.hpp>
#include <wisdom/impl/dx12/dx12_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

WIS_EXTERN_C WisResult wisDX12CreateInstance(bool                             debug_layer,
                                             WisDX12InstanceExtensionHeader** extensions,
                                             size_t                           extension_count,
                                             WisDX12Instance*                 instance)
{
    WisResult res = success;
    // Instance can come as partially constructed from C side
    auto& impl = *reinterpret_cast<DX12InstanceImpl*>(instance);

    com_ptr<IDXGIFactory6> ref;

    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 IID_IDXGIFactory6,
                                 ref.put_void_unchecked());

    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create DXGI Factory">(hr);
    }

    impl.factory = ref.detach();
    return res;
}

WIS_EXTERN_C void wisDX12DestroyInstance(WisDX12Instance* self)
{
    auto& impl = *reinterpret_cast<DX12InstanceImpl*>(self);
    if (impl.factory) {
        impl.factory->Release();
        impl.factory = nullptr;
    }
}

// wis::DX12Factory
// wis::ImplDX12CreateFactory(wis::Result& res, bool enable_debug, DX12FactoryExtension** extensions, size_t extension_count) noexcept
//{
//     // Enable RVO
//     DX12Factory f;
//     auto& internal = f.GetMutableInternal();
//
//     auto hr = CreateDXGIFactory2(enable_debug * DXGI_CREATE_FACTORY_DEBUG, internal.factory.iid(),
//                                  internal.factory.put_void());
//
//     if (!wis::succeeded(hr)) {
//         res = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to create DXGI factory">(hr);
//         return f;
//     }
//
//     for (auto ext : std::span<DX12FactoryExtension*>{ extensions, extension_count }) {
//         ext->Init(f);
//     }
//     return f;
// }
//
// wis::DX12Adapter
// wis::ImplDX12Factory::GetAdapter(wis::Result& result, uint32_t index, AdapterPreference preference) const noexcept
//{
//     wis::DX12Adapter adapter;
//     auto& internal = adapter.GetMutableInternal();
//
//     auto hr = factory->EnumAdapterByGpuPreference(index, convert_dx(preference), internal.adapter.iid(),
//                                                   internal.adapter.put_void());
//     if (!wis::succeeded(hr)) {
//         result = wis::make_result<wis::Func<wis::FuncD()>(), "Failed to get adapter">(hr);
//     }
//     return adapter;
// }
#endif // !WIS_DX12_INSTANCE_CPP
