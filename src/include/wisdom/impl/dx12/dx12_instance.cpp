#ifndef WIS_DX12_INSTANCE_CPP
#define WIS_DX12_INSTANCE_CPP
#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/generated/c_api.h>
#include <wisdom/util/com_ptr.hpp>
#include <wisdom/impl/dx12/dx12_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

namespace wis {
namespace detail {
class DX12DebugLayerThunk : public Microsoft::WRL::RuntimeClass<
                                    Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom | Microsoft::WRL::InhibitRoOriginateError>,
                                    IUnknown>
{
public:
    DX12DebugLayerThunk(ID3D12InfoQueue1* in_info_queue,
                        uint64_t          device,
                        WisDebugCallback  in_callback,
                        void*             in_user_data) noexcept
        : info_queue(in_info_queue)
        , callback(in_callback)
        , user_data(in_user_data)
    {
        if (info_queue) {
            auto hr = info_queue->RegisterMessageCallback(DX12CallbackThunk,
                                                          D3D12_MESSAGE_CALLBACK_FLAG_NONE,
                                                          this,
                                                          &cookie);
            // Debug layer creation failure is allowed to silently fail
            (void)hr;
        }
    }
    ~DX12DebugLayerThunk()
    {
        if (info_queue && cookie != 0) {
            info_queue->UnregisterMessageCallback(cookie);
        }
    }

private:
    static void DX12CallbackThunk(D3D12_MESSAGE_CATEGORY category,
                                  D3D12_MESSAGE_SEVERITY severity,
                                  D3D12_MESSAGE_ID       id,
                                  LPCSTR                 pDescription,
                                  void*                  pContext)
    {
        auto* layer = reinterpret_cast<const DX12DebugLayerThunk*>(pContext);
        layer->DX12Callback(category, severity, id, pDescription);
    }
    void DX12Callback(D3D12_MESSAGE_CATEGORY category,
                      D3D12_MESSAGE_SEVERITY severity,
                      D3D12_MESSAGE_ID       id,
                      LPCSTR                 pDescription) const
    {

        WisSeverity wis_severity = WisSeverityInfo;
        switch (severity) {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            wis_severity = WisSeverityFatal;
            break;
        case D3D12_MESSAGE_SEVERITY_ERROR:
            wis_severity = WisSeverityError;
            break;
        case D3D12_MESSAGE_SEVERITY_WARNING:
            wis_severity = WisSeverityWarning;
            break;
        case D3D12_MESSAGE_SEVERITY_INFO:
            wis_severity = WisSeverityInfo;
            break;
        case D3D12_MESSAGE_SEVERITY_MESSAGE:
            wis_severity = WisSeverityVerbose;
            break;
        default:
            wis_severity = WisSeverityInfo;
            break;
        }
        callback(wis_severity, pDescription, device, user_data);
    }

public:
    ID3D12InfoQueue1* info_queue;
    DWORD             cookie    = 0;
    void*             user_data = nullptr;
    uint64_t          device    = 0;
    WisDebugCallback  callback;
};
} // namespace detail
} // namespace wis

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
                                                        WisDX12InstanceExtensionHeader** extensions,
                                                        size_t                           extension_count,
                                                        WisDX12Instance*                 instance)
{
    WisResult res = dx_success;
    // Instance can come as partially constructed from C side
    auto& impl = *reinterpret_cast<DX12InstanceImpl*>(instance);

    com_ptr<IDXGIFactory6> ref;
    uint32_t               debug_layer = debug_desc && debug_desc->debug_layer;

    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 IID_IDXGIFactory6,
                                 ref.put_void_unchecked());

    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create DXGI Factory">(hr);
    }

    // Create and setup debug layer if requested
    if (debug_layer) {
        com_ptr<ID3D12Debug> debug_controller;
        auto hr2 = D3D12GetDebugInterface(IID_ID3D12Debug, reinterpret_cast<void**>(debug_controller.put_void_unchecked()));
        if (succeeded(hr2)) {
            debug_controller->EnableDebugLayer();
            auto debug_layer_impl       = Microsoft::WRL::Make<DX12DebugLayer>();
            debug_layer_impl->callback  = debug_desc->callback;
            debug_layer_impl->user_data = debug_desc->user_data;
            impl.debug_layer            = debug_layer_impl.Detach();
        } else {
            impl.debug_layer = nullptr;
        }
    } else {
        impl.debug_layer = nullptr;
    }

    impl.factory = ref.detach();
    for (auto* ext : wis::span<WisDX12InstanceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<DX12InstanceExtensionHeader*>(ext);
        if (table) {
            auto xres = table->CallInit(ext, impl);
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
    auto& impl = *reinterpret_cast<DX12InstanceImpl*>(self);
    safe_release(impl.factory);
    safe_release(impl.debug_layer);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
                                                               WisAdapterPreference   preference,
                                                               WisDX12AdapterQuery*   query)
{
    WisResult res = dx_success;
    // Query can come as partially constructed from C side
    auto& impl            = *reinterpret_cast<DX12AdapterQueryImpl*>(query);
    impl.physical_devices = nullptr;
    impl.adapter_count    = 0;

    auto&                  instance_impl = *reinterpret_cast<const DX12InstanceImpl*>(self);
    com_ptr<IDXGIFactory6> factory_ref{ instance_impl.factory }; // hold a reference

    constexpr static uint32_t reasonable_count = 8;

    std::unique_ptr<IDXGIAdapter4*[]> adapters = make_unique<IDXGIAdapter4*[]>(reasonable_count);

    std::size_t count    = 0;
    std::size_t capacity = reasonable_count;

    if (!adapters) {
        return make_result<Func(), "Out of memory while enumerating adapters">(E_OUTOFMEMORY);
    }

    // Dynamic reallocation loop
    while (true) {
        auto hr = factory_ref->EnumAdapterByGpuPreference(uint32_t(count),
                                                          convert(preference),
                                                          IID_IDXGIAdapter4,
                                                          reinterpret_cast<void**>(adapters.get() + count));

        if (hr == DXGI_ERROR_NOT_FOUND) {
            break;
        }
        if (!succeeded(hr)) {
            for (std::size_t i = 0; i < count; ++i) {
                adapters[i]->Release();
            }
            return make_result<Func(), "Failed to enumerate adapters">(hr);
        }
        ++count;

        // Reallocate if needed
        if (count >= capacity) {
            capacity          = capacity * 3 / 2; // increase by 50%
            auto new_adapters = make_unique<IDXGIAdapter4*[]>(capacity);
            if (!new_adapters) {
                return make_result<Func(), "Out of memory while enumerating adapters">(E_OUTOFMEMORY);
            }

            std::memmove(new_adapters.get(), adapters.get(), count * sizeof(IDXGIAdapter4*));
            adapters = std::move(new_adapters);
        }
    }
    impl.physical_devices = adapters.release();
    impl.adapter_count    = count;
    impl.factory          = factory_ref.detach(); // transfer ownership
    impl.debug_layer      = instance_impl.debug_layer;
    if (impl.debug_layer) {
        impl.debug_layer->AddRef();
    }
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyAdapterQuery(WisDX12AdapterQuery* self)
{
    auto& impl = *reinterpret_cast<DX12AdapterQueryImpl*>(self);
    safe_release_array(impl.physical_devices, impl.adapter_count);
    delete[] impl.physical_devices;
    safe_release(impl.debug_layer);
    safe_release(impl.factory);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisDX12AdapterQueryGetAdapterCount(const WisDX12AdapterQuery* self)
{
    return reinterpret_cast<const DX12AdapterQueryImpl*>(self)->adapter_count;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12AdapterQueryGetAdapterDesc(const WisDX12AdapterQuery* self,
                                                                    size_t                     index,
                                                                    WisAdapterDesc*            desc)
{
    WisResult res  = dx_success;
    auto&     impl = *reinterpret_cast<const DX12AdapterQueryImpl*>(self);
    if (index >= impl.adapter_count) {
        return make_result<Func(), "Adapter index out of bounds">(E_INVALIDARG);
    }
    DXGI_ADAPTER_DESC3 adapter_desc;
    auto               hr = impl.physical_devices[index]->GetDesc3(&adapter_desc);
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to get adapter description">(hr);
    }

    *desc = WisAdapterDesc{
        .vendor_id              = adapter_desc.VendorId,
        .device_id              = adapter_desc.DeviceId,
        .dedicated_video_memory = static_cast<uint64_t>(adapter_desc.DedicatedVideoMemory),
        .shared_system_memory   = static_cast<uint64_t>(adapter_desc.SharedSystemMemory),
        .adapter_id             = *reinterpret_cast<uint64_t*>(&adapter_desc.AdapterLuid),
        .flags                  = WisAdapterFlags(adapter_desc.Flags),
    };

    // Copy description
    std::wstring_view desc_wview{ adapter_desc.Description, std::size(adapter_desc.Description) };

    // Convert to UTF-8 (may terminate early)
    WideCharToMultiByte(CP_UTF8,
                        0,
                        desc_wview.data(),
                        static_cast<int32_t>(desc_wview.size()),
                        desc->description,
                        std::size(desc->description),
                        nullptr,
                        nullptr);
    return res;
}

WIS_EXTERN_C WISDOM_API WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery*     self,
                                                                  size_t                         index,
                                                                  WisDX12DeviceExtensionHeader** extensions,
                                                                  size_t                         extension_count,
                                                                  WisDX12Device*                 device)
{
    auto& impl = *reinterpret_cast<const DX12AdapterQueryImpl*>(self);
    if (index >= impl.adapter_count) {
        return make_result<Func(), "Adapter index out of bounds">(E_INVALIDARG);
    }
    com_ptr<ID3D12Device10> device_ref;
    auto                    hr = D3D12CreateDevice(impl.physical_devices[index],
                                D3D_FEATURE_LEVEL_12_0,
                                IID_ID3D12Device10,
                                reinterpret_cast<void**>(device_ref.put_void_unchecked()));
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create D3D12 device">(hr);
    }

    D3D12_FEATURE_DATA_D3D12_OPTIONS12 options12                 = {};
    bool                               EnhancedBarriersSupported = false;
    if (succeeded(device_ref->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS12, &options12, sizeof(options12)))) {
        EnhancedBarriersSupported = options12.EnhancedBarriersSupported;
    }
    if (!EnhancedBarriersSupported) {
        return make_result<Func(), "D3D12 device does not support Enhanced Barriers">(E_FAIL);
    }

    // Bind debug callback if available
    if (impl.debug_layer && impl.debug_layer->callback) {
        DWORD                     debug_cookie = 0;
        com_ptr<ID3D12InfoQueue1> info_queue;
        auto hr2 = device_ref->QueryInterface(IID_ID3D12InfoQueue1, reinterpret_cast<void**>(info_queue.put_void_unchecked()));
        if (succeeded(hr2)) {
            auto thunk = Microsoft::WRL::Make<detail::DX12DebugLayerThunk>(info_queue.get(),
                                                                           reinterpret_cast<uint64_t>(device_ref.get()),
                                                                           impl.debug_layer->callback,
                                                                           impl.debug_layer->user_data);

            // Debug layer creation failure is allowed to silently fail
            if (thunk) {
                // set as private data to keep alive
                hr2 = device_ref->SetPrivateDataInterface(__uuidof(IUnknown), thunk.Get());
                (void)hr2;
            }
        }
    }

    auto& device_impl           = *reinterpret_cast<DX12DeviceImpl*>(device);
    device_impl.device          = device_ref.detach();
    device_impl.physical_device = impl.physical_devices[index];
    device_impl.factory         = impl.factory;
    device_impl.physical_device->AddRef();
    device_impl.factory->AddRef();

    for (auto* ext : wis::span<WisDX12DeviceExtensionHeader*>{ extensions, extension_count }) {
        auto* table = reinterpret_cast<DX12DeviceExtensionHeader*>(ext);
        if (table) {
            // TODO: implement device extension initialization
        }
    }

    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDevice(WisDX12Device* self)
{
    auto& impl = *reinterpret_cast<DX12DeviceImpl*>(self);
    safe_release(impl.device);
    safe_release(impl.physical_device);
    safe_release(impl.factory);
}

#endif // !WIS_DX12_INSTANCE_CPP
