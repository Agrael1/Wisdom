#ifndef WIS_DX12_INSTANCE_CPP
#define WIS_DX12_INSTANCE_CPP

#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/util/com_ptr.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
                                                        WisDX12InstanceExtensionHeader** extensions,
                                                        size_t                           extension_count,
                                                        WisDX12Instance*                 instance)
{
    wis::com_ptr<IDXGIFactory6> ref;
    uint32_t                    debug_layer = debug_desc && debug_desc->enable_debug_layer;

    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 IID_IDXGIFactory6,
                                 ref.put_void_unchecked());

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create DXGI Factory">(hr);
    }

    // Create and setup debug layer if requested
    wis::detail::DX12DebugLayer* debug_layer_ptr = nullptr;
    if (debug_layer) {
        wis::com_ptr<ID3D12Debug> debug_controller;
        auto                      hr2 = D3D12GetDebugInterface(IID_ID3D12Debug, reinterpret_cast<void**>(debug_controller.put_void_unchecked()));
        if (wis::detail::succeeded(hr2)) {
            debug_controller->EnableDebugLayer();
            wis::com_ptr<wis::detail::DX12DebugLayer> debug_layer_impl{ new wis::detail::DX12DebugLayer, wis::take_ownership };
            debug_layer_impl->callback  = debug_desc->callback;
            debug_layer_impl->user_data = debug_desc->user_data;
            debug_layer_ptr             = debug_layer_impl.detach();
        }
    }

    auto& impl = *new (instance) wis::impl::DX12InstanceImpl{
        .factory     = ref.detach(),
        .debug_layer = debug_layer_ptr,
    };

    WisResult res = wis::detail::dx_success;
    for (auto* ext : wis::span<WisDX12InstanceExtensionHeader*>{ extensions, extension_count }) {
        if (auto* table = wis::from_handle<wis::DX12InstanceExtensionHeader>(ext); table && table->init_fptr) {
            res = table->init_fptr(table, impl);
            if (res.status != WisStatusOk) {
                res.status = WisStatusPartial; // mark as partial success if any extension fails
            }
        }
    }

    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyInstance(WisDX12Instance* self)
{
    auto& [factory, debug_layer] = wis::from_handle_ref<wis::impl::DX12InstanceImpl>(self);
    if (!factory) {
        return;
    }

    factory->Release();
    if (debug_layer) {
        debug_layer->Release();
    }

    factory = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
                                                               WisAdapterPreference   preference,
                                                               WisDX12AdapterQuery*   query)
{
    const auto&                 instance_impl = wis::from_handle_ref<const wis::impl::DX12InstanceImpl>(self);
    wis::com_ptr<IDXGIFactory6> factory_ref{ instance_impl.factory }; // hold a reference

    constexpr static uint32_t reasonable_count = 8;

    std::unique_ptr<IDXGIAdapter4*[]> adapters = wis::make_unique<IDXGIAdapter4*[]>(reasonable_count);

    std::size_t count    = 0;
    std::size_t capacity = reasonable_count;

    if (!adapters) {
        return wis::detail::make_result<wis::detail::Func(), "Out of memory while enumerating adapters">(E_OUTOFMEMORY);
    }

    // Dynamic reallocation loop
    while (true) {
        auto hr = factory_ref->EnumAdapterByGpuPreference(static_cast<uint32_t>(count),
                                                          wis::detail::convert_dx(preference),
                                                          IID_IDXGIAdapter4,
                                                          reinterpret_cast<void**>(adapters.get() + count));

        if (hr == DXGI_ERROR_NOT_FOUND) {
            break;
        }
        if (!wis::detail::succeeded(hr)) {
            for (std::size_t i = 0; i < count; ++i) {
                adapters[i]->Release();
            }
            return wis::detail::make_result<wis::detail::Func(), "Failed to enumerate adapters">(hr);
        }
        ++count;

        // Reallocate if needed
        if (count >= capacity) {
            capacity          = capacity * 3 / 2; // increase by 50%
            auto new_adapters = wis::make_unique<IDXGIAdapter4*[]>(capacity);
            if (!new_adapters) {
                return wis::detail::make_result<wis::detail::Func(), "Out of memory while enumerating adapters">(E_OUTOFMEMORY);
            }

            std::memmove(new_adapters.get(), adapters.get(), count * sizeof(IDXGIAdapter4*));
            adapters = std::move(new_adapters);
        }
    }

    if (instance_impl.debug_layer) {
        instance_impl.debug_layer->AddRef();
    }
    auto& impl = *new (query) wis::impl::DX12AdapterQueryImpl{
        .physical_devices = adapters.release(),
        .adapter_count    = count,
        .factory          = factory_ref.detach(), // transfer ownership
        .debug_layer      = instance_impl.debug_layer,
    };
    return wis::detail::dx_success;
}
#endif // WIS_DX12_INSTANCE_CPP