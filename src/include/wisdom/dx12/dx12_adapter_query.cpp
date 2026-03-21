#ifndef WIS_DX12_ADAPTER_QUERY_CPP
#define WIS_DX12_ADAPTER_QUERY_CPP

#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/util/com_ptr.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyAdapterQuery(WisDX12AdapterQuery* self)
{
    auto& [physical_devices, adapter_count, factory, debug_layer] = wis::from_handle_ref<wis::impl::DX12AdapterQueryImpl>(self);
    if (!physical_devices) {
        return;
    }

    for (std::size_t i = 0; i < adapter_count; ++i) {
        if (physical_devices[i]) {
            physical_devices[i]->Release();
        }
    }
    delete[] physical_devices;
    physical_devices = nullptr;

    if (debug_layer) {
        debug_layer->Release();
    }

    factory->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API size_t wisDX12AdapterQueryGetAdapterCount(const WisDX12AdapterQuery* self)
{
    return wis::from_handle<const wis::impl::DX12AdapterQueryImpl>(self)->adapter_count;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12AdapterQueryGetAdapterDesc(const WisDX12AdapterQuery* self,
                                                                    size_t                     index,
                                                                    WisAdapterDesc*            desc)
{
    WisResult res  = wis::detail::dx_success;
    auto&     impl = wis::from_handle_ref<const wis::impl::DX12AdapterQueryImpl>(self);
    if (index >= impl.adapter_count) {
        return wis::detail::make_result<wis::detail::Func(), "Adapter index out of bounds">(E_INVALIDARG);
    }
    DXGI_ADAPTER_DESC3 adapter_desc;
    auto               hr = impl.physical_devices[index]->GetDesc3(&adapter_desc);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to get adapter description">(hr);
    }

    *desc = WisAdapterDesc{
        .description            = {},
        .vendor_id              = adapter_desc.VendorId,
        .device_id              = adapter_desc.DeviceId,
        .dedicated_video_memory = (adapter_desc.DedicatedVideoMemory),
        .shared_system_memory   = (adapter_desc.SharedSystemMemory),
        .adapter_id             = *reinterpret_cast<uint64_t*>(&adapter_desc.AdapterLuid),
        .adapter_uuid           = {},
        .flags                  = static_cast<WisAdapterFlags>(adapter_desc.Flags),
    };

    // Copy description
    const std::wstring_view desc_wview{ adapter_desc.Description, std::size(adapter_desc.Description) };

    // Convert to UTF-8 (may terminate early)
    WideCharToMultiByte(CP_UTF8,
                        0,
                        desc_wview.data(),
                        static_cast<int32_t>(desc_wview.size()),
                        desc->description,
                        static_cast<int>(std::size(desc->description)),
                        nullptr,
                        nullptr);
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API bool wisDX12AdapterQueryGetSurfaceSupport(const WisDX12AdapterQuery* self,
                                                                  size_t                     index,
                                                                  WisDX12SurfaceView         surface)
{
    (void)self;
    (void)index;
    (void)surface;
    return true; // D3D12 supports presentation on all adapters
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery*       self,
                                                                  size_t                           index,
                                                                  const WisDX12DeviceRequirements* requirements,
                                                                  WisDX12Device*                   device)
{
    WisResult res  = wis::detail::dx_success;
    auto&     impl = wis::from_handle_ref<const wis::impl::DX12AdapterQueryImpl>(self);
    if (index >= impl.adapter_count) {
        return wis::detail::make_result<wis::detail::Func(), "Adapter index out of bounds">(E_INVALIDARG);
    }
    wis::com_ptr<ID3D12Device10> device_ref;
    auto                         hr = D3D12CreateDevice(impl.physical_devices[index],
                                D3D_FEATURE_LEVEL_12_0,
                                IID_ID3D12Device10,
                                reinterpret_cast<void**>(device_ref.put_void_unchecked()));
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create D3D12 device">(hr);
    }

    D3D12_FEATURE_DATA_D3D12_OPTIONS12 options12                 = {};
    bool                               EnhancedBarriersSupported = false;
    if (wis::detail::succeeded(device_ref->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS12, &options12, sizeof(options12)))) {
        EnhancedBarriersSupported = options12.EnhancedBarriersSupported;
    }
    if (!EnhancedBarriersSupported) {
        return wis::detail::make_result<wis::detail::Func(), "D3D12 device does not support Enhanced Barriers">(E_FAIL);
    }

    // Bind debug callback if available
    if (impl.debug_layer && impl.debug_layer->callback) {
        wis::com_ptr<ID3D12InfoQueue1> info_queue;
        if (auto hr2 = device_ref->QueryInterface(IID_ID3D12InfoQueue1, reinterpret_cast<void**>(info_queue.put_void_unchecked())); wis::detail::succeeded(hr2)) {
            const wis::com_ptr<wis::detail::DX12DebugLayerThunk> thunk{
                new wis::detail::DX12DebugLayerThunk(info_queue.get(),
                                                     reinterpret_cast<uint64_t>(device_ref.get()),
                                                     impl.debug_layer->callback,
                                                     impl.debug_layer->user_data),
                wis::take_ownership
            };

            // Debug layer creation failure is allowed to silently fail
            if (thunk) {
                // set as private data to keep alive
                hr2 = device_ref->SetPrivateDataInterface(IID_IUnknown, thunk.get());
                (void)hr2;
            }
        }
    }

    // Create D3D12 memory allocator
    D3D12MA::ALLOCATOR_DESC allocator_desc = {};
    allocator_desc.pDevice                 = device_ref.get();
    allocator_desc.pAdapter                = impl.physical_devices[index];
    allocator_desc.Flags                   = D3D12MA::ALLOCATOR_FLAG_NONE;
    D3D12MA::Allocator* out_allocator      = nullptr;
    hr                                     = D3D12MA::CreateAllocator(&allocator_desc, &out_allocator);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create D3D12 memory allocator">(hr);
    }

    auto& device_impl = *new (device) wis::impl::DX12DeviceImpl{
        .device          = device_ref.detach(),
        .physical_device = impl.physical_devices[index],
        .factory         = impl.factory,
        .allocator       = out_allocator,
    };
    device_impl.physical_device->AddRef();
    device_impl.factory->AddRef();

    // If no requirements provided, return early with default device impl
    if (!requirements) {
        return res;
    }

    // Scan queue descriptions and store priorities in device impl
    for (size_t i = 0; i < requirements->queue_desc_count; ++i) {
        const auto& desc = requirements->queue_descs[i];
        if (desc.type >= WisCommandQueueTypeCount || desc.type < 0) {
            return wis::detail::make_result<wis::detail::Func(), "Invalid command queue type specified in requirements">(E_INVALIDARG);
        }

        if (desc.priority > WisCommandQueuePriorityNormal) {
            // Check if selected queue type is supported by the device
            D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY queue_priority = {
                .CommandListType = wis::detail::DX12Convert(desc.type),
                .Priority        = static_cast<UINT>(wis::detail::DX12Convert(desc.priority)),
            };
            device_impl.device->CheckFeatureSupport(D3D12_FEATURE_COMMAND_QUEUE_PRIORITY, &queue_priority, sizeof(queue_priority));
            device_impl.queue_priorities[desc.type] = queue_priority.PriorityForTypeIsSupported ? desc.priority : WisCommandQueuePriorityNormal;
        }

        device_impl.queue_priorities[desc.type] |= 1 << 7; // set support bit for this queue type
    }

    for (auto* ext : wis::span<WisDX12DeviceExtensionHeader*>{ requirements->extensions, requirements->extension_count }) {
        if (auto* table = wis::from_handle<wis::DX12DeviceExtensionHeader>(ext); table && table->init_fptr) {
            if (const auto xres = table->init_fptr(table, device_impl); xres.status != WisStatusOk) {
                res.status        = WisStatusPartial; // mark as partial success if any extension fails
                res.error         = xres.error;
                res.platform_code = xres.platform_code;
            }
        }
    }

    return res;
}

#endif // WIS_DX12_ADAPTER_QUERY_CPP