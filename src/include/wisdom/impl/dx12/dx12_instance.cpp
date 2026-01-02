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
        , user_data(in_user_data)
        , device(device)
        , callback(in_callback)
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
        (void)category;
        (void)id;
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

inline D3D12_ROOT_PARAMETER_TYPE dx12_root_parameter_type(WisDescriptorType type) noexcept
{
    switch (type) {
    case WisDescriptorTypeConstantBuffer:
        return D3D12_ROOT_PARAMETER_TYPE_CBV;
    default:
    case WisDescriptorTypeBuffer:
    case WisDescriptorTypeAccelerationStructure:
        return D3D12_ROOT_PARAMETER_TYPE_SRV;
    case WisDescriptorTypeRWBuffer:
        return D3D12_ROOT_PARAMETER_TYPE_UAV;
    }
}
inline bool dx12_is_pushable(WisDescriptorType type) noexcept
{
    switch (type) {
    case WisDescriptorTypeBuffer:
    case WisDescriptorTypeConstantBuffer:
    case WisDescriptorTypeRWBuffer:
    case WisDescriptorTypeAccelerationStructure:
        return true;
    default:
        return false;
    }
}

} // namespace detail
} // namespace wis

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
                                                        WisDX12InstanceExtensionHeader** extensions,
                                                        size_t                           extension_count,
                                                        WisDX12Instance*                 instance)
{
    WisResult              res = dx_success;
    com_ptr<IDXGIFactory6> ref;
    uint32_t               debug_layer = debug_desc && debug_desc->enable_debug_layer;

    auto hr = CreateDXGIFactory2(debug_layer * DXGI_CREATE_FACTORY_DEBUG,
                                 IID_IDXGIFactory6,
                                 ref.put_void_unchecked());

    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create DXGI Factory">(hr);
    }

    // Create and setup debug layer if requested
    auto& impl = *new (instance) DX12InstanceImpl();
    if (debug_layer) {
        com_ptr<ID3D12Debug> debug_controller;
        auto                 hr2 = D3D12GetDebugInterface(IID_ID3D12Debug, reinterpret_cast<void**>(debug_controller.put_void_unchecked()));
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
            res = table->CallInit(ext, impl);
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
    if (!impl.factory) {
        return;
    }

    safe_release(impl.factory);
    safe_release(impl.debug_layer);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
                                                               WisAdapterPreference   preference,
                                                               WisDX12AdapterQuery*   query)
{
    WisResult              res           = dx_success;
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
                                                          convert_dx(preference),
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

    auto& impl            = *new (query) DX12AdapterQueryImpl();
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
    if (!impl.physical_devices) {
        return;
    }

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
        .description            = {},
        .vendor_id              = adapter_desc.VendorId,
        .device_id              = adapter_desc.DeviceId,
        .dedicated_video_memory = static_cast<uint64_t>(adapter_desc.DedicatedVideoMemory),
        .shared_system_memory   = static_cast<uint64_t>(adapter_desc.SharedSystemMemory),
        .adapter_id             = *reinterpret_cast<uint64_t*>(&adapter_desc.AdapterLuid),
        .adapter_uuid           = {},
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
        com_ptr<ID3D12InfoQueue1> info_queue;
        auto                      hr2 = device_ref->QueryInterface(IID_ID3D12InfoQueue1, reinterpret_cast<void**>(info_queue.put_void_unchecked()));
        if (succeeded(hr2)) {
            auto thunk = Microsoft::WRL::Make<detail::DX12DebugLayerThunk>(info_queue.get(),
                                                                           reinterpret_cast<uint64_t>(device_ref.get()),
                                                                           impl.debug_layer->callback,
                                                                           impl.debug_layer->user_data);

            // Debug layer creation failure is allowed to silently fail
            if (thunk) {
                // set as private data to keep alive
                hr2 = device_ref->SetPrivateDataInterface(IID_IUnknown, thunk.Get());
                (void)hr2;
            }
        }
    }

    auto& device_impl           = *new (device) DX12DeviceImpl();
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
    if (!impl.device) {
        return;
    }

    safe_release(impl.device);
    safe_release(impl.physical_device);
    safe_release(impl.factory);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateCommandQueue(const WisDX12Device* self,
                                                                  WisCommandQueueType  type,
                                                                  WisDX12CommandQueue* queue)
{
    WisResult result = dx_success;
    auto&     device = *reinterpret_cast<const DX12DeviceImpl*>(self);

    D3D12_COMMAND_QUEUE_DESC desc{
        .Type     = convert_dx(type),
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0,
    };

    com_ptr<ID3D12CommandQueue> out_queue;

    HRESULT hr = device.device->CreateCommandQueue(&desc, IID_ID3D12CommandQueue, out_queue.put_void_unchecked());
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create command queue">(hr);
    }

    auto& internal = *new (queue) DX12CommandQueueImpl();
    internal.queue = out_queue.detach();
    return result;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandQueue(WisDX12CommandQueue* self)
{
    auto& impl = *reinterpret_cast<DX12CommandQueueImpl*>(self);
    if (!impl.queue) {
        return;
    }
    safe_release(impl.queue);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateCommandList(const WisDX12Device* self,
                                                                 WisCommandQueueType  type,
                                                                 WisDX12CommandList*  list)
{
    WisResult result = dx_success;
    auto&     device = *reinterpret_cast<const DX12DeviceImpl*>(self);

    com_ptr<ID3D12GraphicsCommandList7> command_list;

    auto hr = device.device->CreateCommandList1(0,
                                                convert_dx(type),
                                                D3D12_COMMAND_LIST_FLAG_NONE,
                                                IID_ID3D12GraphicsCommandList7,
                                                command_list.put_void_unchecked());
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create command list">(hr);
    }

    auto& internal = *new (list) DX12CommandListImpl();
    internal.list  = command_list.detach();
    return result;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self)
{
    auto& impl = *reinterpret_cast<DX12CommandListImpl*>(self);
    if (!impl.list) {
        return;
    }

    safe_release(impl.list);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateFence(const WisDX12Device* self,
                                                           uint64_t             initial_value,
                                                           WisDX12Fence*        fence)
{
    WisResult            result = dx_success;
    auto&                device = *reinterpret_cast<const DX12DeviceImpl*>(self);
    com_ptr<ID3D12Fence> out_fence;
    auto                 hr = device.device->CreateFence(initial_value,
                                         D3D12_FENCE_FLAG_NONE,
                                         IID_ID3D12Fence,
                                         out_fence.put_void_unchecked());
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create fence">(hr);
    }

    // Create event handle
    auto event_handle = CreateEventW(nullptr, false, false, nullptr);
    if (!event_handle) {
        return make_result<Func(), "Failed to create fence event handle">(HRESULT_FROM_WIN32(GetLastError()));
    }

    auto& internal = *new (fence) DX12FenceImpl();
    internal.fence = out_fence.detach();
    internal.event = event_handle;
    return result;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyFence(WisDX12Fence* self)
{
    auto& impl = *reinterpret_cast<DX12FenceImpl*>(self);
    if (!impl.fence) {
        return;
    }

    safe_release(impl.fence);
    CloseHandle(impl.event);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateResourceAllocator(const WisDX12Device*      self,
                                                                       WisDX12ResourceAllocator* allocator)
{
    WisResult res    = dx_success;
    auto&     device = *reinterpret_cast<const DX12DeviceImpl*>(self);
    // Create D3D12MA Allocator
    D3D12MA::ALLOCATOR_DESC allocator_desc = {};
    allocator_desc.pDevice                 = device.device;
    allocator_desc.pAdapter                = device.physical_device;
    allocator_desc.Flags                   = D3D12MA::ALLOCATOR_FLAG_NONE;
    D3D12MA::Allocator* out_allocator      = nullptr;
    auto                hr                 = D3D12MA::CreateAllocator(&allocator_desc, &out_allocator);
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create D3D12 memory allocator">(hr);
    }

    // Fill allocator impl
    auto& allocator_impl     = *new (allocator) DX12ResourceAllocatorImpl();
    allocator_impl.allocator = out_allocator;
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self)
{
    auto& impl = *reinterpret_cast<DX12ResourceAllocatorImpl*>(self);
    if (!impl.allocator) {
        return;
    }

    safe_release(impl.allocator);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreatePipelineLayout(const WisDX12Device*         self,
                                                                    const WisPipelineLayoutDesc* desc,
                                                                    WisDX12PipelineLayout*       layout)
{
    auto&     device = *reinterpret_cast<const DX12DeviceImpl*>(self);
    WisResult res    = dx_success;

    // https://learn.microsoft.com/en-us/windows/win32/direct3d12/root-signature-limits
    static constexpr std::size_t max_root_parameters = 64;

    // Check limits
    if (desc->push_constant_count + 2 * desc->push_descriptor_count > max_root_parameters) {
        return make_result<Func(), "Exceeded maximum number of root parameters">(E_INVALIDARG);
    }

    D3D12_ROOT_PARAMETER1 root_parameters[max_root_parameters];
    std::size_t           num_root_parameters = desc->push_constant_count + desc->push_descriptor_count;
    std::size_t           offset_parameters   = 0;

    // push constants
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        root_parameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        root_parameters[i].Constants     = {
                .ShaderRegister = static_cast<UINT>(desc->push_constants[i].bind_register),
                .RegisterSpace  = static_cast<UINT>(desc->push_constants[i].bind_space),
                .Num32BitValues = static_cast<UINT>(desc->push_constants[i].size_bytes / 4),
        };
        root_parameters[i].ShaderVisibility = detail::convert_dx(desc->push_constants[i].stage);
    }
    offset_parameters += desc->push_constant_count;

    // push descriptors
    uint32_t descriptor_space = 0;
    for (std::size_t i = 0; i < desc->push_descriptor_count; ++i) {
        auto& param = root_parameters[offset_parameters + i];
        auto& src   = desc->push_descriptors[i];

        if (!detail::dx12_is_pushable(src.type)) {
            return make_result<Func(), "Descriptor type is not pushable to DX12 root signature">(E_INVALIDARG);
        }

        param.ParameterType = detail::dx12_root_parameter_type(src.type);
        param.Descriptor    = {
               .ShaderRegister = src.bind_register,
               .RegisterSpace  = descriptor_space,
               .Flags          = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
        };
        param.ShaderVisibility = detail::convert_dx(desc->push_descriptors[i].stage);
    }
    descriptor_space += desc->push_descriptor_count > 0;

    // TODO: static samplers
    std::unique_ptr<D3D12_STATIC_SAMPLER_DESC1[]> static_samplers;
    if (desc->static_sampler_count > 0) {
        static_samplers = make_unique<D3D12_STATIC_SAMPLER_DESC1[]>(desc->static_sampler_count);
        if (!static_samplers) {
            return make_result<Func(), "Out of memory while creating static samplers">(E_OUTOFMEMORY);
        }
        for (std::size_t i = 0; i < desc->static_sampler_count; ++i) {
            const auto& src  = desc->static_samplers[i];
            auto&       samp = src.sampler;

            auto min_filter   = !samp.is_anisotropic ? convert_dx(samp.min_filter) : D3D12_FILTER_TYPE_LINEAR;
            auto mag_filter   = !samp.is_anisotropic ? convert_dx(samp.mag_filter) : D3D12_FILTER_TYPE_LINEAR;
            auto basic_filter = D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, convert_dx(samp.mip_filter), D3D12_FILTER_REDUCTION_TYPE::D3D12_FILTER_REDUCTION_TYPE_STANDARD);

            static_samplers[i] = {
                .Filter           = D3D12_FILTER(samp.is_anisotropic * D3D12_ANISOTROPIC_FILTERING_BIT | basic_filter),
                .AddressU         = convert_dx(samp.address_u),
                .AddressV         = convert_dx(samp.address_v),
                .AddressW         = convert_dx(samp.address_w),
                .MipLODBias       = samp.mip_lod_bias,
                .MaxAnisotropy    = samp.max_anisotropy,
                .ComparisonFunc   = convert_dx(samp.comparison_op),
                .BorderColor      = convert_dx(samp.static_border_color),
                .MinLOD           = samp.min_lod,
                .MaxLOD           = samp.max_lod,
                .ShaderRegister   = src.bind_register,
                .RegisterSpace    = descriptor_space,
                .ShaderVisibility = detail::convert_dx(src.stage),
                .Flags            = convert_dx(samp.flags)
            };
        }
    }

    // TODO: tables

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsig_desc{
        .Version  = D3D_ROOT_SIGNATURE_VERSION_1_2,
        .Desc_1_2 = {
                     .NumParameters     = static_cast<UINT>(num_root_parameters),
                     .pParameters       = root_parameters,
                     .NumStaticSamplers = static_cast<UINT>(desc->static_sampler_count),
                     .pStaticSamplers   = static_samplers.get(),
                     .Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
                     },
    };

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT                hr = D3D12SerializeVersionedRootSignature(&rsig_desc, signature.put(), error.put());

    // Check for serialization errors
    if (!succeeded(hr)) {
        // If error blob is available, include its message into debug output
#ifdef _DEBUG
        if (error) {
            OutputDebugStringA(reinterpret_cast<const char*>(error->GetBufferPointer()));
        }
#endif

        return make_result<Func(), "Failed to serialize root signature">(hr);
    }

    wis::com_ptr<ID3D12RootSignature> root_signature;
    hr = device.device->CreateRootSignature(0,
                                            signature->GetBufferPointer(),
                                            signature->GetBufferSize(),
                                            IID_ID3D12RootSignature,
                                            root_signature.put_void_unchecked());
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create root signature">(hr);
    }

    auto& layout_impl          = *new (layout) DX12PipelineLayoutImpl();
    layout_impl.root_signature = root_signature.detach();
    return res;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyPipelineLayout(WisDX12PipelineLayout* self)
{
    auto& impl = *reinterpret_cast<DX12PipelineLayoutImpl*>(self);
    if (!impl.root_signature) {
        return;
    }

    safe_release(impl.root_signature);
}

#endif // !WIS_DX12_INSTANCE_CPP
