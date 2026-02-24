#ifndef WIS_DX12_DEVICE_CPP
#define WIS_DX12_DEVICE_CPP

#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/util/com_ptr.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDevice(WisDX12Device* self)
{
    auto& impl = *reinterpret_cast<DX12DeviceImpl*>(self);
    if (!impl.device) {
        return;
    }

    impl.device->Release();
    impl.physical_device->Release();
    impl.factory->Release();
    impl.allocator->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateCommandQueue(const WisDX12Device* self,
                                                                  WisCommandQueueType  type,
                                                                  WisDX12CommandQueue* queue)
{
    auto& device = *reinterpret_cast<const DX12DeviceImpl*>(self);

    bool supported = (device.queue_priorities[type] & ~0x7fu) != 0;
    if (!supported) {
        return make_result<Func(), "Requested command queue type is not supported or not enabled by the device">(E_INVALIDARG);
    }

    D3D12_COMMAND_QUEUE_DESC desc{
        .Type     = convert_dx(type),
        .Priority = convert_dx(WisCommandQueuePriority(device.queue_priorities[type] & 0x7f)),
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
    return dx_success;
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
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceGetResourceAllocator(const WisDX12Device*      self,
                                                                    WisDX12ResourceAllocator* allocator)
{
    auto& device = *reinterpret_cast<const DX12DeviceImpl*>(self);

    // Fill allocator impl
    auto& allocator_impl     = *new (allocator) DX12ResourceAllocatorImpl();
    allocator_impl.allocator = device.allocator;
    allocator_impl.allocator->AddRef(); // hold reference to allocator
    return dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateDescriptorHeap(const WisDX12Device*         self,
                                                                    const WisDescriptorHeapDesc* desc,
                                                                    WisDX12DescriptorHeap*       heap)
{
    auto& device = *reinterpret_cast<const DX12DeviceImpl*>(self);

    // Create descriptor heap container
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type           = convert_dx(desc->type),
        .NumDescriptors = static_cast<UINT>(desc->descriptor_count),
        .Flags          = convert_dx(desc->memory_type),
        .NodeMask       = 0,
    };

    wis::com_ptr<ID3D12DescriptorHeap> descriptor_heap;
    HRESULT                            hr = device.device->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap, descriptor_heap.put_void_unchecked());
    if (!succeeded(hr)) {
        return make_result<Func(), "Failed to create descriptor heap">(hr);
    }

    auto& heap_impl           = *new (heap) DX12DescriptorHeapImpl();
    heap_impl.descriptor_heap = descriptor_heap.detach();
    return dx_success;
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
    if (desc->push_constant_count + 2 * desc->push_descriptor_count + desc->descriptor_table_count > max_root_parameters) {
        return make_result<Func(), "Exceeded maximum number of root parameters">(E_INVALIDARG);
    }

    D3D12_ROOT_PARAMETER1            root_parameters[max_root_parameters];
    std::size_t                      num_root_parameters = desc->push_constant_count + desc->push_descriptor_count + desc->descriptor_table_count;
    wis::span<D3D12_ROOT_PARAMETER1> root_parameters_span{ root_parameters, num_root_parameters };

    // Push constants
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        root_parameters_span[i] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
            .Constants     = {
                              .ShaderRegister = static_cast<UINT>(desc->push_constants[i].bind_register),
                              .RegisterSpace  = static_cast<UINT>(desc->push_constants[i].bind_space),
                              .Num32BitValues = static_cast<UINT>(desc->push_constants[i].size_bytes / 4),
                              },
            .ShaderVisibility = detail::convert_dx(desc->push_constants[i].stage),
        };
    }
    root_parameters_span = root_parameters_span.subspan(desc->push_constant_count);

    // Push descriptors
    uint32_t descriptor_space = 0;
    for (std::size_t i = 0; i < desc->push_descriptor_count; ++i) {
        auto& src = desc->push_descriptors[i];

        if (!detail::dx12_is_pushable(src.type)) {
            return make_result<Func(), "Descriptor type is not pushable to DX12 root signature">(E_INVALIDARG);
        }

        root_parameters_span[i] = {
            .ParameterType = detail::dx12_root_parameter_type(src.type),
            .Descriptor    = {
                              .ShaderRegister = src.bind_register,
                              .RegisterSpace  = descriptor_space,
                              .Flags          = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
                              },
            .ShaderVisibility = detail::convert_dx(src.stage),
        };
    }
    root_parameters_span = root_parameters_span.subspan(desc->push_descriptor_count);
    descriptor_space += desc->push_descriptor_count > 0;

    // Static samplers
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

    // Tables
    std::unique_ptr<D3D12_DESCRIPTOR_RANGE1[]> ranges;
    if (desc->descriptor_table_count > 0) {
        wis::span<const WisDescriptorTable> tables{ desc->descriptor_tables, desc->descriptor_table_count };
        uint32_t                            range_count = 0;

        // Precompute range count
        for (uint32_t i = 0; i < desc->descriptor_table_count; ++i) {
            const auto& src = tables[i];
            if (src.space_overlap != 0) {
                // Check if entry is single range
                if (src.entry_count > 1) {
                    return make_result<Func(), "Space overlap is only supported for single range descriptor tables">(E_INVALIDARG);
                }
                range_count += src.space_overlap + 1;
                continue;
            }
            range_count += static_cast<uint32_t>(src.entry_count);
        }

        ranges = make_unique<D3D12_DESCRIPTOR_RANGE1[]>(range_count);
        if (!ranges) {
            return make_result<Func(), "Out of memory while creating descriptor ranges">(E_OUTOFMEMORY);
        }

        wis::span<D3D12_DESCRIPTOR_RANGE1> ranges_span{ ranges.get(), range_count };
        std::size_t                        current_range = 0;
        for (std::size_t i = 0; i < desc->descriptor_table_count; ++i) {
            const auto& src           = tables[i];
            uint32_t    filled_ranges = detail::dx12_fill_descriptor_range(src,
                                                                        descriptor_space,
                                                                        ranges_span.subspan(current_range));
            descriptor_space += src.space_overlap != 0 ? src.space_overlap : 1;
            root_parameters_span[i] = {
                .ParameterType   = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
                .DescriptorTable = {
                                    .NumDescriptorRanges = filled_ranges,
                                    .pDescriptorRanges   = ranges.get() + current_range,
                                    },
                .ShaderVisibility = detail::convert_dx(src.stage),
            };
            current_range += filled_ranges;
        }
    }

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
            // Query debug info queue
            wis::com_ptr<ID3D12InfoQueue> info_queue;
            auto                          hr2 = device.device->QueryInterface(IID_ID3D12InfoQueue, info_queue.put_void_unchecked());
            if (succeeded(hr2) && info_queue) {
                info_queue->AddMessage(D3D12_MESSAGE_CATEGORY::D3D12_MESSAGE_CATEGORY_COMPILATION,
                                       D3D12_MESSAGE_SEVERITY_ERROR,
                                       D3D12_MESSAGE_ID::D3D12_MESSAGE_ID_CREATE_ROOTSIGNATURE,
                                       reinterpret_cast<const char*>(error->GetBufferPointer()));
            } else {
                // Fallback to OutputDebugString
                OutputDebugStringA(reinterpret_cast<const char*>(error->GetBufferPointer()));
            }
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

WIS_EXTERN_C WISDOM_API void wisDX12DeviceQueryProperties(const WisDX12Device* self,
                                                          void*                properties)
{
    if (!properties) {
        return;
    }

    auto& device = *reinterpret_cast<const DX12DeviceImpl*>(self);
    void* next   = properties;

    do {
        WisQueryStructHeader header_local{};
        std::memcpy(&header_local, next, sizeof(WisQueryStructHeader));

        switch (header_local.property_type) {
        case WisQueryPropertyTypeDeviceCommandQueueProperties: {
            auto* props = static_cast<WisDeviceCommandQueuesProperties*>(next);
            for (size_t i = 0; i < WisCommandQueueTypeCount; ++i) {
                props->supported_queues[i]   = (device.queue_priorities[i] & ~0x7fu) != 0;
                props->max_queue_priority[i] = WisCommandQueuePriority(device.queue_priorities[i] & 0x7f);
            }
        } break;
        case WisQueryPropertyTypeDeviceDescriptorHeapProperties: {
            auto*                              props     = static_cast<WisDeviceDescriptorHeapProperties*>(next);
            D3D12_FEATURE_DATA_D3D12_OPTIONS19 options19 = {};
            if (succeeded(device.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS19, &options19, sizeof(options19)))) {
                props->max_descriptor_heap_size            = options19.MaxViewDescriptorHeapSize;
                props->max_sampler_heap_size               = options19.MaxSamplerDescriptorHeapSize;
                props->max_sampler_heap_size_with_embedded = options19.MaxSamplerDescriptorHeapSizeWithStaticSamplers;
                props->descriptor_increment_size           = device.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                props->sampler_increment_size              = device.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            }
        } break;
        case WisQueryPropertyTypeDeviceMemoryProperties: {
            auto*                              props     = static_cast<WisDeviceMemoryProperties*>(next);
            D3D12_FEATURE_DATA_D3D12_OPTIONS16 options16 = {};
            if (succeeded(device.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS16, &options16, sizeof(options16)))) {
                props->gpu_upload_supported = options16.GPUUploadHeapSupported;
                props->host_image_copy_supported = options16.GPUUploadHeapSupported;
            }
        } break;
        default:
            break;
        }
        next = header_local.next_in_chain;
    } while (next);
}

#endif // WIS_DX12_DEVICE_CPP
