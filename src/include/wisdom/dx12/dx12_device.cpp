#ifndef WIS_DX12_DEVICE_CPP
#define WIS_DX12_DEVICE_CPP

#include <wisdom/dx12/detail/dx12_detail.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/generated/backend_api.hpp>
#include <wisdom/util/allocation.hpp>
#include <wisdom/util/com_ptr.hpp>
#include <wisdom/util/xxhash.h>
#include <wisdom/bridge/format.hpp>
#include <d3dx12/d3dx12_pipeline_state_stream.h>
#include <bit>
#include <ranges>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyDevice(WisDX12Device* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::DX12DeviceImpl>(self);
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
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    bool supported = (device.queue_priorities[type] & ~0x7fu) != 0;
    if (!supported) {
        return wis::detail::make_result<wis::detail::Func(), "Requested command queue type is not supported or not enabled by the device">(E_INVALIDARG);
    }

    D3D12_COMMAND_QUEUE_DESC desc{
        .Type     = wis::detail::DX12Convert(type),
        .Priority = wis::detail::DX12Convert(WisCommandQueuePriority(device.queue_priorities[type] & 0x7f)),
        .Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0,
    };

    wis::com_ptr<ID3D12CommandQueue> out_queue;

    HRESULT hr = device.device->CreateCommandQueue(&desc, IID_ID3D12CommandQueue, out_queue.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create command queue">(hr);
    }

    auto& internal = *new (queue) wis::impl::DX12CommandQueueImpl{
        .queue = out_queue.detach(),
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateCommandAllocator(const WisDX12Device*     self,
                                                                      WisCommandQueueType      type,
                                                                      WisDX12CommandAllocator* list)
{
    WisResult                            result = wis::detail::dx_success;
    auto&                                device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);
    wis::com_ptr<ID3D12CommandAllocator> allocator;

    auto hr = device.device->CreateCommandAllocator(wis::detail::DX12Convert(type),
                                                    IID_ID3D12CommandAllocator,
                                                    allocator.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create command allocator">(hr);
    }

    auto& internal = *new (list) wis::impl::DX12CommandAllocatorImpl{
        .allocator = allocator.detach(),
        .device    = device.device, // store device pointer for later use when creating command lists with this allocator (don't refcount)
        .type      = type,
    };
    return result;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateFence(const WisDX12Device* self,
                                                           uint64_t             initial_value,
                                                           WisDX12Fence*        fence)
{
    WisResult result = wis::detail::dx_success;
    auto&     device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    wis::com_ptr<ID3D12Fence> out_fence;

    auto hr = device.device->CreateFence(initial_value,
                                         D3D12_FENCE_FLAG_NONE,
                                         IID_ID3D12Fence,
                                         out_fence.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create fence">(hr);
    }

    // Create event handle
    auto event_handle = CreateEventW(nullptr, false, false, nullptr);
    if (!event_handle) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create fence event handle">(HRESULT_FROM_WIN32(GetLastError()));
    }

    auto& internal = *new (fence) wis::impl::DX12FenceImpl{
        .fence = out_fence.detach(),
        .event = event_handle,
    };
    return result;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceGetResourceAllocator(const WisDX12Device*      self,
                                                                    WisDX12ResourceAllocator* allocator)
{
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    // Fill allocator impl
    device.allocator->AddRef(); // hold reference to allocator
    auto& allocator_impl = *new (allocator) wis::impl::DX12ResourceAllocatorImpl{
        .allocator = device.allocator,
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateDescriptorHeap(const WisDX12Device*         self,
                                                                    const WisDescriptorHeapDesc* desc,
                                                                    WisDX12DescriptorHeap*       heap)
{
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    // Create descriptor heap container
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type           = wis::detail::DX12Convert(desc->type),
        .NumDescriptors = static_cast<UINT>(desc->descriptor_count),
        .Flags          = wis::detail::DX12Convert(desc->memory_type),
        .NodeMask       = 0,
    };

    wis::com_ptr<ID3D12DescriptorHeap> descriptor_heap;
    HRESULT                            hr = device.device->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap, descriptor_heap.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create descriptor heap">(hr);
    }

    auto* raw_heap  = descriptor_heap.detach();
    auto& heap_impl = *new (heap) wis::impl::DX12DescriptorHeapImpl{
        .descriptor_heap = raw_heap,
        .device          = device.device,
        .gpu_handle      = raw_heap->GetGPUDescriptorHandleForHeapStart(),
        .cpu_handle      = raw_heap->GetCPUDescriptorHandleForHeapStart(),
        .descriptor_size = device.device->GetDescriptorHandleIncrementSize(heap_desc.Type),
        .type            = heap_desc.Type,
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateViewHeap(const WisDX12Device* self,
                                                              WisViewHeapType      type,
                                                              uint32_t             capacity,
                                                              WisDX12ViewHeap*     heap)
{
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    // Create descriptor heap container
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{
        .Type           = wis::detail::DX12Convert(type),
        .NumDescriptors = capacity,
        .Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask       = 0,
    };

    wis::com_ptr<ID3D12DescriptorHeap> descriptor_heap;
    HRESULT                            hr = device.device->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap, descriptor_heap.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create descriptor heap">(hr);
    }

    auto* raw_heap  = descriptor_heap.detach();
    auto& heap_impl = *new (heap) wis::impl::DX12ViewHeapImpl{
        .view_heap = raw_heap,
        .device          = device.device,
        .cpu_handle      = raw_heap->GetCPUDescriptorHandleForHeapStart(),
        .descriptor_size = device.device->GetDescriptorHandleIncrementSize(heap_desc.Type),
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateRootSignature(const WisDX12Device*        self,
                                                                   const WisRootSignatureDesc* desc,
                                                                   WisDX12RootSignature*       layout)
{
    auto&     device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);
    WisResult res    = wis::detail::dx_success;

    // https://learn.microsoft.com/en-us/windows/win32/direct3d12/root-signature-limits
    static constexpr std::size_t max_root_parameters = 64;
    std::size_t                  push_constant_size  = 0;
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        const auto& push_constant = desc->push_constants[i];
        if (push_constant.size_bytes % 4 != 0) {
            return wis::detail::make_result<wis::detail::Func(), "Push constant size must be divisible by 4 bytes">(E_INVALIDARG);
        }
        push_constant_size += push_constant.size_bytes;
    }
    push_constant_size /= 4;

    // Check limits
    if (push_constant_size + 2 * desc->push_descriptor_count + desc->descriptor_table_count > max_root_parameters) {
        return wis::detail::make_result<wis::detail::Func(), "Exceeded maximum number of root parameters">(E_INVALIDARG);
    }

    D3D12_ROOT_PARAMETER1            root_parameters[max_root_parameters];
    std::size_t                      num_root_parameters = desc->push_constant_count + desc->push_descriptor_count + desc->descriptor_table_count;
    wis::span<D3D12_ROOT_PARAMETER1> root_parameters_span{ root_parameters, num_root_parameters };

    // Push constants
    for (std::size_t i = 0; i < desc->push_constant_count; ++i) {
        auto& src               = desc->push_constants[i];
        root_parameters_span[i] = {
            .ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
            .Constants     = {
                              .ShaderRegister = static_cast<UINT>(src.bind_register),
                              .RegisterSpace  = static_cast<UINT>(src.bind_space),
                              .Num32BitValues = static_cast<UINT>(src.size_bytes / 4),
                              },
            .ShaderVisibility = wis::detail::DX12Convert(src.visibility),
        };
    }
    root_parameters_span = root_parameters_span.subspan(desc->push_constant_count);

    // Push descriptors
    for (std::size_t i = 0; i < desc->push_descriptor_count; ++i) {
        auto& src = desc->push_descriptors[i];

        if (!wis::detail::dx12_is_pushable(src.type)) {
            return wis::detail::make_result<wis::detail::Func(), "Descriptor type is not pushable to DX12 root signature">(E_INVALIDARG);
        }

        root_parameters_span[i] = {
            .ParameterType = wis::detail::dx12_root_parameter_type(src.type),
            .Descriptor    = {
                              .ShaderRegister = src.bind_register,
                              .RegisterSpace  = src.bind_space,
                              .Flags          = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
                              },
            .ShaderVisibility = wis::detail::DX12Convert(src.visibility),
        };
    }
    root_parameters_span = root_parameters_span.subspan(desc->push_descriptor_count);

    // Tables
    std::unique_ptr<D3D12_DESCRIPTOR_RANGE1[]> ranges;
    if (desc->descriptor_table_count > 0) {
        wis::span<const WisDescriptorTable> tables{ desc->descriptor_tables, desc->descriptor_table_count };
        uint32_t                            range_count = 0;

        // Precompute range count
        for (uint32_t i = 0; i < desc->descriptor_table_count; ++i) {
            const auto& src = tables[i];
            range_count += static_cast<uint32_t>(src.entry_count);
        }

        ranges = wis::make_unique<D3D12_DESCRIPTOR_RANGE1[]>(range_count);
        if (!ranges) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory while creating descriptor ranges">(E_OUTOFMEMORY);
        }

        wis::span<D3D12_DESCRIPTOR_RANGE1> ranges_span{ ranges.get(), range_count };
        std::size_t                        range_offset = 0;
        for (std::size_t i = 0; i < desc->descriptor_table_count; ++i) {
            const auto& table = tables[i];

            for (size_t i = 0; i < table.entry_count; ++i) {
                auto& src                     = table.entries[i];
                ranges_span[i + range_offset] = {
                    .RangeType                         = wis::detail::DX12Convert(src.type),
                    .NumDescriptors                    = (src.count == 0 ? 1 : src.count),
                    .BaseShaderRegister                = src.bind_register,
                    .RegisterSpace                     = src.bind_space,
                    .Flags                             = src.count > 1 ? D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE : D3D12_DESCRIPTOR_RANGE_FLAG_NONE,
                    .OffsetInDescriptorsFromTableStart = src.descriptor_offset,
                };
            }

            root_parameters_span[i] = {
                .ParameterType   = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
                .DescriptorTable = {
                                    .NumDescriptorRanges = static_cast<uint32_t>(table.entry_count),
                                    .pDescriptorRanges   = ranges.get() + range_offset,
                                    },
                .ShaderVisibility = wis::detail::DX12Convert(table.visibility),
            };
            range_offset += table.entry_count;
        }
    }

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsig_desc{
        .Version  = D3D_ROOT_SIGNATURE_VERSION_1_2,
        .Desc_1_2 = {
                     .NumParameters     = static_cast<UINT>(num_root_parameters),
                     .pParameters       = root_parameters,
                     .NumStaticSamplers = 0,
                     .pStaticSamplers   = nullptr,
                     .Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
                     },
    };

    wis::com_ptr<ID3DBlob> signature;
    wis::com_ptr<ID3DBlob> error;
    HRESULT                hr = D3D12SerializeVersionedRootSignature(&rsig_desc, signature.put(), error.put());

    // Check for serialization errors
    if (!wis::detail::succeeded(hr)) {
        // If error blob is available, include its message into debug output
#ifdef _DEBUG
        if (error) {
            // Query debug info queue
            wis::com_ptr<ID3D12InfoQueue> info_queue;
            auto                          hr2 = device.device->QueryInterface(IID_ID3D12InfoQueue, info_queue.put_void_unchecked());
            if (wis::detail::succeeded(hr2) && info_queue) {
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

        return wis::detail::make_result<wis::detail::Func(), "Failed to serialize root signature">(hr);
    }

    wis::com_ptr<ID3D12RootSignature> root_signature;
    hr = device.device->CreateRootSignature(0,
                                            signature->GetBufferPointer(),
                                            signature->GetBufferSize(),
                                            IID_ID3D12RootSignature,
                                            root_signature.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create root signature">(hr);
    }

    // Compute hash of root signature description for caching purposes
    XXH128_hash_t                     hash = XXH3_128bits(signature->GetBufferPointer(), signature->GetBufferSize());
    wis::detail::DX12RootSignatureKey key{
        .hash{ hash.low64, hash.high64 }
    };
    root_signature->SetPrivateData(
            wis::detail::DX12RootSignatureKey::guid,
            sizeof(wis::detail::DX12RootSignatureKey),
            &key);

    auto& layout_impl = *new (layout) wis::impl::DX12RootSignatureImpl{
        .root_signature = root_signature.detach()
    };
    return res;
}

WIS_EXTERN_C WISDOM_API void wisDX12DeviceQueryProperties(const WisDX12Device* self,
                                                          void*                properties)
{
    if (!properties) {
        return;
    }

    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);
    void* next   = properties;

    do {
        WisQueryStructHeader header_local{};
        std::memcpy(&header_local, next, sizeof(WisQueryStructHeader));

        switch (header_local.property_type) {
        case WisQueryPropertyTypeDeviceCommandQueueProperties: {
            auto* props = static_cast<WisDeviceCommandQueueProperties*>(next);
            for (size_t i = 0; i < WisCommandQueueTypeCount; ++i) {
                props->supported_queues[i]   = (device.queue_priorities[i] & ~0x7fu) != 0;
                props->max_queue_priority[i] = WisCommandQueuePriority(device.queue_priorities[i] & 0x7f);
            }
            props->relaxed_queue_transition = true;
        } break;
        case WisQueryPropertyTypeDeviceDescriptorHeapProperties: {
            auto*                              props     = static_cast<WisDeviceDescriptorHeapProperties*>(next);
            D3D12_FEATURE_DATA_D3D12_OPTIONS19 options19 = {};
            if (wis::detail::succeeded(device.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS19, &options19, sizeof(options19)))) {
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
            if (wis::detail::succeeded(device.device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS16, &options16, sizeof(options16)))) {
                props->gpu_upload_supported          = options16.GPUUploadHeapSupported;
                props->host_image_copy_supported     = options16.GPUUploadHeapSupported;
                props->supported_initial_transitions = 0b0001'1111'1111'1111; // All thansitions are supported
            }
        } break;
        case WisQueryPropertyTypeDeviceBindingProperties: {
            auto* props                         = static_cast<WisDeviceBindingProperties*>(next);
            props->max_vertex_input_bindings    = D3D12_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
            props->max_vertex_input_attributes  = D3D12_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT;
            props->multiple_viewports_supported = true; // D3D12 supports up to 16 viewports and scissor rectangles
        } break;
        default:
            break;
        }
        next = header_local.next_in_chain;
    } while (next);
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceWaitForMultipleFences(const WisDX12Device*    self,
                                                                     const WisDX12FenceView* fences,
                                                                     const uint64_t*         fence_values,
                                                                     size_t                  fence_count,
                                                                     WisMutiWaitType         wait_for,
                                                                     uint64_t                timeout)
{
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    HANDLE event_handle = CreateEventW(nullptr, false, false, nullptr);
    if (!event_handle) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create fence event handle">(HRESULT_FROM_WIN32(GetLastError()));
    }

    auto hr = device.device->SetEventOnMultipleFenceCompletion(
            reinterpret_cast<ID3D12Fence* const*>(fences),
            fence_values,
            static_cast<UINT>(fence_count),
            static_cast<D3D12_MULTIPLE_FENCE_WAIT_FLAGS>(wait_for),
            event_handle);

    CloseHandle(event_handle);

    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to set event on multiple fence completion">(hr);
    }
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreatePipelineCache(const WisDX12Device*  self,
                                                                   const uint8_t*        initial_data,
                                                                   size_t                data_size,
                                                                   WisDX12PipelineCache* cache)
{
    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    uint8_t* data_copy = nullptr;
    if (initial_data && data_size > 0) {
        data_copy = static_cast<uint8_t*>(malloc(data_size));
        if (!data_copy) {
            return wis::detail::make_result<wis::detail::Func(), "Out of memory while copying pipeline cache data">(E_OUTOFMEMORY);
        }
        std::memcpy(data_copy, initial_data, data_size);
    }

    wis::com_ptr<ID3D12PipelineLibrary1> pipeline_library;

    auto hr = device.device->CreatePipelineLibrary(data_copy,
                                                   data_size,
                                                   IID_ID3D12PipelineLibrary1,
                                                   pipeline_library.put_void_unchecked());

    if (!wis::detail::succeeded(hr)) {
        free(data_copy);
        return wis::detail::make_result<wis::detail::Func(), "Failed to create pipeline library">(hr);
    }

    auto& cache_impl = *new (cache) wis::impl::DX12PipelineCacheImpl{
        .library = pipeline_library.detach(),
        .data    = data_copy,
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateShader(const WisDX12Device* self,
                                                            const uint8_t*       data,
                                                            size_t               size,
                                                            WisDX12Shader*       shader)
{
    if (!data || size == 0) {
        return wis::detail::make_result<wis::detail::Func(), "Shader bytecode data is null or empty">(E_INVALIDARG);
    }

    auto& device = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);

    std::unique_ptr<wis::detail::DX12ShaderHeader> shader_header{
        reinterpret_cast<wis::detail::DX12ShaderHeader*>(operator new(wis::aligned_size(size, 8ull) + sizeof(wis::detail::DX12ShaderHeader), std::nothrow))
    };
    if (!shader_header) {
        return wis::detail::make_result<wis::detail::Func(), "Out of memory while creating shader header">(E_OUTOFMEMORY);
    }

    std::construct_at(shader_header.get());

    auto bc = shader_header->GetMutableBytecode();
    std::memcpy(bc.data(), data, size);
    shader_header->size = size;

    // xxHash the shader bytecode to allow quick comparisons for pipeline caching
    XXH128_hash_t hash     = XXH3_128bits(data, size);
    shader_header->hash[0] = hash.low64;
    shader_header->hash[1] = hash.high64;

    auto& shader_impl = *new (shader) wis::impl::DX12ShaderImpl{
        .shader = shader_header.release(),
    };

    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateComputePipeline(const WisDX12Device*              self,
                                                                     const WisDX12ComputePipelineDesc* desc,
                                                                     WisDX12Pipeline*                  pipeline)
{
    auto& device  = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);
    auto* rootsig = std::bit_cast<ID3D12RootSignature*>(desc->root_signature);
    auto* shader  = std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->compute_shader);
    auto* cache   = std::bit_cast<ID3D12PipelineLibrary1*>(desc->cache);

    // Validate root signature
    if (!rootsig) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid root signature provided for compute pipeline creation">(E_INVALIDARG);
    }
    // Validate shader
    if (!shader) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid shader provided for compute pipeline creation">(E_INVALIDARG);
    }

    auto bytecode = shader->GetBytecode();

    struct ComputePipelineStream {
        CD3DX12_PIPELINE_STATE_STREAM_FLAGS          flags;
        CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK      node_mask;
        CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE root_signature;
        CD3DX12_PIPELINE_STATE_STREAM_CS             compute_shader;
    } stream{
        .flags          = D3D12_PIPELINE_STATE_FLAG_NONE,
        .node_mask      = 0,
        .root_signature = rootsig,
        .compute_shader = { { bytecode.data(), bytecode.size() } },
    };

    D3D12_PIPELINE_STATE_STREAM_DESC pso_desc{
        .SizeInBytes                   = sizeof(ComputePipelineStream),
        .pPipelineStateSubobjectStream = &stream,
    };

    wis::com_ptr<ID3D12PipelineState> pipeline_state;

    // Calculate hash of pipeline state description for caching purposes
    wchar_t name_buffer[256] = {};

    if (cache) {
        // Get root signature hash
        UINT                              root_sig_data_size = sizeof(wis::detail::DX12RootSignatureKey);
        wis::detail::DX12RootSignatureKey root_sig_key{};
        rootsig->GetPrivateData(wis::detail::DX12RootSignatureKey::guid, &root_sig_data_size, &root_sig_key);

        uint64_t rehash_input[4] = {
            // Hash root signature and shader bytecode together to get a unique hash for the pipeline state
            root_sig_key.hash[0],
            root_sig_key.hash[1],
            shader->hash[0],
            shader->hash[1],
        };

        // Rehash the combined data to get a final hash for the pipeline state
        XXH128_hash_t pso_hash = XXH3_128bits(rehash_input, sizeof(rehash_input));

        // convert hash to hex string for use as pipeline cache key
        wis::format_to(name_buffer, L"CPSO_{:016x}{:016x}", pso_hash.low64, pso_hash.high64);

        // Try to load pipeline from cache first if available
        HRESULT hr = cache->LoadPipeline(name_buffer, &pso_desc, IID_ID3D12PipelineState, pipeline_state.put_void_unchecked());
        if (wis::detail::succeeded(hr)) {
            auto& pipeline_impl = *new (pipeline) wis::impl::DX12PipelineImpl{
                .pipeline_state = pipeline_state.detach(),
            };
            return wis::detail::dx_success;
        }

        // Cache miss
        if (desc->flags & WisPipelineFlagsFailOnCacheMiss) {
            return wis::detail::make_result<wis::detail::Func(), "Pipeline not found in cache and creation is set to fail on cache miss">(WisStatusError, E_FAIL);
        }
    }

    auto hr = device.device->CreatePipelineState(&pso_desc, IID_ID3D12PipelineState, pipeline_state.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create compute pipeline state object">(hr);
    }

    if (cache) {
        // Store pipeline in cache for future reuse
        cache->StorePipeline(name_buffer, pipeline_state.get());
    }

    auto& pipeline_impl = *new (pipeline) wis::impl::DX12PipelineImpl{
        .pipeline_state = pipeline_state.detach(),
    };
    return wis::detail::dx_success;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12DeviceCreateGraphicsPipeline(const WisDX12Device*               self,
                                                                      const WisDX12GraphicsPipelineDesc* desc,
                                                                      WisDX12Pipeline*                   pipeline)
{
    auto& device  = wis::from_handle_ref<const wis::impl::DX12DeviceImpl>(self);
    auto* cache   = std::bit_cast<ID3D12PipelineLibrary1*>(desc->cache);
    auto* rootsig = std::bit_cast<ID3D12RootSignature*>(desc->root_signature);
    if (!rootsig) {
        return wis::detail::make_result<wis::detail::Func(), "Invalid root signature provided for graphics pipeline creation">(E_INVALIDARG);
    }

    struct GraphicsPipelineStream {
        CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE  root_signature;
        CD3DX12_PIPELINE_STATE_STREAM_VS              vertex_shader;
        CD3DX12_PIPELINE_STATE_STREAM_PS              pixel_shader;
        CD3DX12_PIPELINE_STATE_STREAM_GS              geometry_shader;
        CD3DX12_PIPELINE_STATE_STREAM_HS              hull_shader;
        CD3DX12_PIPELINE_STATE_STREAM_DS              domain_shader;
        CD3DX12_PIPELINE_STATE_STREAM_VIEW_INSTANCING view_instancing;

        // Hashable fields for pipeline caching
        CD3DX12_PIPELINE_STATE_STREAM_FLAGS                 flags;
        CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK             node_mask;
        CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT          input_layout;
        CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY    topology;
        CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER2           rasterizer;
        CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtv_formats;
        CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT  depth_stencil_format;
        CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC           sample_desc;
        CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_MASK           sample_mask;
        CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL2        depth_stencil;
        CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC            blend_state;
    } stream{
        .root_signature = rootsig,
        .flags          = desc->flags & WisPipelineFlagsEnablePrimitiveRestart
                         ? D3D12_PIPELINE_STATE_FLAG_DYNAMIC_INDEX_BUFFER_STRIP_CUT
                         : D3D12_PIPELINE_STATE_FLAG_NONE,
    };

    static constexpr size_t              shader_stage_count                 = 5;
    const wis::detail::DX12ShaderHeader* shader_headers[shader_stage_count] = {
        std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->vertex_shader),
        std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->pixel_shader),
        std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->geometry_shader),
        std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->hull_shader),
        std::bit_cast<const wis::detail::DX12ShaderHeader*>(desc->domain_shader),
    };

    //--Shader stages
    if (auto vs = shader_headers[0]) {
        auto bytecode        = vs->GetBytecode();
        stream.vertex_shader = {
            { bytecode.data(), bytecode.size() }
        };
    } else {
        return wis::detail::make_result<wis::detail::Func(), "Vertex shader is required for graphics pipeline creation">(E_INVALIDARG);
    }
    if (auto ps = shader_headers[1]) {
        auto bytecode       = ps->GetBytecode();
        stream.pixel_shader = {
            { bytecode.data(), bytecode.size() }
        };
    }
    if (auto gs = shader_headers[2]) {
        auto bytecode          = gs->GetBytecode();
        stream.geometry_shader = {
            { bytecode.data(), bytecode.size() }
        };
    }
    if (auto hs = shader_headers[3]) {
        auto bytecode      = hs->GetBytecode();
        stream.hull_shader = {
            { bytecode.data(), bytecode.size() }
        };
    }
    if (auto ds = shader_headers[4]) {
        auto bytecode        = ds->GetBytecode();
        stream.domain_shader = {
            { bytecode.data(), bytecode.size() }
        };
    }

    //--Render targets
    if (desc->render_attachments.attachments_count > wis::MaxRenderTargets) {
        return wis::detail::make_result<wis::detail::Func(), "Exceeded maximum number of render target attachments (8)">(E_INVALIDARG);
    }

    D3D12_RT_FORMAT_ARRAY& rtv_formats = stream.rtv_formats;
    for (uint32_t i = 0; i < desc->render_attachments.attachments_count; i++) {
        rtv_formats.RTFormats[i] = wis::detail::DX12Convert(desc->render_attachments.attachment_formats[i]);
    }
    rtv_formats.NumRenderTargets = desc->render_attachments.attachments_count;
    if (desc->render_attachments.depth_attachment != WisDataFormatUnknown) {
        stream.depth_stencil_format = wis::detail::DX12Convert(desc->render_attachments.depth_attachment);
    }

    //--Multiview
    D3D12_VIEW_INSTANCE_LOCATION view_locs[wis::MaxRenderTargets]{};
    if (desc->render_attachments.view_mask) {
        uint32_t view_mask = desc->render_attachments.view_mask;
        for (uint32_t i = 0u; i < wis::MaxRenderTargets; i++) {
            if (!(view_mask & (1u << i))) {
                continue;
            }

            view_locs[i] = D3D12_VIEW_INSTANCE_LOCATION{
                .ViewportArrayIndex     = 0,
                .RenderTargetArrayIndex = i,
            };
        }
        stream.view_instancing = CD3DX12_VIEW_INSTANCING_DESC{
            uint32_t(std::popcount(view_mask)),
            view_locs,
            D3D12_VIEW_INSTANCING_FLAG_ENABLE_VIEW_INSTANCE_MASKING
        };
    }

    //--Input layout
    wis::span<const WisInputBindingDesc>        slots{ desc->input_layout.bindings, desc->input_layout.binding_count };
    wis::span<const WisInputAttributeDesc>      attrs{ desc->input_layout.attributes, desc->input_layout.attribute_count };
    D3D12_INPUT_ELEMENT_DESC                    reasonable_max_input_elements[wis::MinSupportedInputAttributes * 2]{};
    std::unique_ptr<D3D12_INPUT_ELEMENT_DESC[]> input_elements;
    wis::span<D3D12_INPUT_ELEMENT_DESC>         input_elements_span;
    if (!slots.empty() && !attrs.empty()) {
        if (attrs.size() > wis::MinSupportedInputAttributes * 2) {
            input_elements      = wis::make_unique<D3D12_INPUT_ELEMENT_DESC[]>(attrs.size());
            input_elements_span = { input_elements.get(), attrs.size() };
        } else {
            input_elements_span = { reasonable_max_input_elements, attrs.size() };
        }
    }

    for (uint32_t i = 0; i < attrs.size(); i++) {
        auto& attr = attrs[i];
        auto& slot = slots[attr.binding_index];

        input_elements_span[i] = { .SemanticName         = attr.semantic_name,
                                   .SemanticIndex        = attr.semantic_index,
                                   .Format               = wis::detail::DX12Convert(attr.format),
                                   .InputSlot            = slot.slot,
                                   .AlignedByteOffset    = attr.offset_bytes,
                                   .InputSlotClass       = D3D12_INPUT_CLASSIFICATION(slot.input_class),
                                   .InstanceDataStepRate = slot.input_class == WisInputClassPerInstance ? slot.stride_bytes : 0 };
    }
    stream.input_layout = { input_elements.get(), uint32_t(attrs.size()) };

    //--Topology
    stream.topology = wis::detail::DX12Convert(desc->topology_type);

    //--Rasterizer
    if (desc->rasterizer_desc) {
        auto& raster = *desc->rasterizer_desc;
        bool  bias   = raster.depth_bias_enable;
        if (bias) {
            stream.flags |= D3D12_PIPELINE_STATE_FLAG_DYNAMIC_DEPTH_BIAS;
        }

        stream.rasterizer = CD3DX12_RASTERIZER_DESC2{
            D3D12_RASTERIZER_DESC2{
                                   .FillMode              = wis::detail::DX12Convert(raster.fill_mode),
                                   .CullMode              = wis::detail::DX12Convert(raster.cull_mode),
                                   .FrontCounterClockwise = wis::detail::DX12Convert(raster.front_face),
                                   .DepthBias             = bias ? raster.depth_bias : 0.0f,
                                   .DepthBiasClamp        = bias ? raster.depth_bias_clamp : 0.0f,
                                   .SlopeScaledDepthBias  = bias ? raster.depth_bias_slope_factor : 0.0f,
                                   .DepthClipEnable       = raster.depth_clip_enable,
                                   .LineRasterizationMode = wis::detail::DX12Convert(raster.line_rasterization),
                                   .ConservativeRaster    = wis::detail::DX12Convert(raster.conservative_rasterization) }
        };
    }

    //--Multisample
    if (desc->sample_desc) {
        stream.sample_desc = DXGI_SAMPLE_DESC{
            .Count   = wis::detail::DX12Convert(desc->sample_desc->rate),
            .Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN,
        };
        stream.sample_mask = desc->sample_desc->sample_mask;

        if (!desc->blend_state_desc && desc->sample_desc->alpha_to_coverage_enable) {
            CD3DX12_BLEND_DESC& bd   = stream.blend_state;
            bd.AlphaToCoverageEnable = true;
        }
    }

    //--Depth stencil
    if (desc->depth_stencil_desc) {
        auto& ds             = *desc->depth_stencil_desc;
        stream.depth_stencil = CD3DX12_DEPTH_STENCIL_DESC2{
            { .DepthEnable    = ds.depth_enable,
             .DepthWriteMask = D3D12_DEPTH_WRITE_MASK(ds.depth_write_enable),
             .DepthFunc      = wis::detail::DX12Convert(ds.depth_comp),
             .StencilEnable  = ds.stencil_enable,
             .FrontFace =
                      D3D12_DEPTH_STENCILOP_DESC1{
                              .StencilFailOp      = wis::detail::DX12Convert(ds.stencil_front.fail_op),
                              .StencilDepthFailOp = wis::detail::DX12Convert(ds.stencil_front.depth_fail_op),
                              .StencilPassOp      = wis::detail::DX12Convert(ds.stencil_front.pass_op),
                              .StencilFunc        = wis::detail::DX12Convert(ds.stencil_front.stencil_comp),
                              .StencilReadMask    = ds.stencil_front.read_mask,
                              .StencilWriteMask   = ds.stencil_front.write_mask,
                      },
             .BackFace =
                      D3D12_DEPTH_STENCILOP_DESC1{
                              .StencilFailOp      = wis::detail::DX12Convert(ds.stencil_back.fail_op),
                              .StencilDepthFailOp = wis::detail::DX12Convert(ds.stencil_back.depth_fail_op),
                              .StencilPassOp      = wis::detail::DX12Convert(ds.stencil_back.pass_op),
                              .StencilFunc        = wis::detail::DX12Convert(ds.stencil_back.stencil_comp),
                              .StencilReadMask    = ds.stencil_back.read_mask,
                              .StencilWriteMask   = ds.stencil_back.write_mask,
                      },
             .DepthBoundsTestEnable = ds.depth_bound_test }
        };
    }

    //--Blend
    if (desc->blend_state_desc) {
        auto&             blend      = *desc->blend_state_desc;
        D3D12_BLEND_DESC& bdesc      = stream.blend_state;
        bdesc.AlphaToCoverageEnable  = desc->sample_desc ? desc->sample_desc->alpha_to_coverage_enable : false;
        bdesc.IndependentBlendEnable = blend.attachment_count > 0 && !blend.logic_op_enable;

        if (bdesc.IndependentBlendEnable) {
            for (size_t i = 0; i < blend.attachment_count; i++) {
                auto& a               = blend.attachments[i];
                bdesc.RenderTarget[i] = D3D12_RENDER_TARGET_BLEND_DESC{
                    .BlendEnable           = a.blend_enable,
                    .LogicOpEnable         = false,
                    .SrcBlend              = wis::detail::DX12Convert(a.src_color_blend),
                    .DestBlend             = wis::detail::DX12Convert(a.dst_color_blend),
                    .BlendOp               = wis::detail::DX12Convert(a.color_blend_op),
                    .SrcBlendAlpha         = wis::detail::DX12Convert(a.src_alpha_blend),
                    .DestBlendAlpha        = wis::detail::DX12Convert(a.dst_alpha_blend),
                    .BlendOpAlpha          = wis::detail::DX12Convert(a.alpha_blend_op),
                    .LogicOp               = D3D12_LOGIC_OP_NOOP,
                    .RenderTargetWriteMask = UINT8(a.color_write_mask),
                };
            }
        } else {
            auto& a               = blend.attachments[0];
            bdesc.RenderTarget[0] = D3D12_RENDER_TARGET_BLEND_DESC{
                .BlendEnable           = a.blend_enable,
                .LogicOpEnable         = false,
                .SrcBlend              = wis::detail::DX12Convert(a.src_color_blend),
                .DestBlend             = wis::detail::DX12Convert(a.dst_color_blend),
                .BlendOp               = wis::detail::DX12Convert(a.color_blend_op),
                .SrcBlendAlpha         = wis::detail::DX12Convert(a.src_alpha_blend),
                .DestBlendAlpha        = wis::detail::DX12Convert(a.dst_alpha_blend),
                .BlendOpAlpha          = wis::detail::DX12Convert(a.alpha_blend_op),
                .LogicOp               = D3D12_LOGIC_OP_NOOP,
                .RenderTargetWriteMask = UINT8(a.color_write_mask),
            };
        }
        if (blend.logic_op_enable) {
            bdesc.RenderTarget[0].LogicOpEnable = true;
            bdesc.RenderTarget[0].LogicOp       = wis::detail::DX12Convert(blend.logic_op);
        }
    }

    D3D12_PIPELINE_STATE_STREAM_DESC psstream_desc{
        .SizeInBytes                   = sizeof(GraphicsPipelineStream),
        .pPipelineStateSubobjectStream = &stream,
    };
    wis::com_ptr<ID3D12PipelineState> pipeline_state;

    wchar_t name_buffer[128] = {};
    if (cache) {
        uint32_t name_offset = 0; // max 7
        struct RehashInput {
            uint64_t root_sig_hash[2];
            uint64_t shader_hashes[2 * shader_stage_count];

            uint32_t multiview_mask;
            uint64_t pso_hash[2];
        } rehash_input{};

        // Get root signature hash
        UINT                              root_sig_data_size = sizeof(wis::detail::DX12RootSignatureKey);
        wis::detail::DX12RootSignatureKey root_sig_key{};
        rootsig->GetPrivateData(wis::detail::DX12RootSignatureKey::guid, &root_sig_data_size, &root_sig_key);
        rehash_input.root_sig_hash[0] = root_sig_key.hash[0];
        rehash_input.root_sig_hash[1] = root_sig_key.hash[1];

        // Get shader hashes
        static constexpr auto shader_stage_names = L"VPGHD"; // Vertex, Pixel, Geometry, Hull, Domain
        for (size_t i = 0; i < 5; ++i) {
            if (shader_headers[i]) {
                rehash_input.shader_hashes[2 * i]     = shader_headers[i]->hash[0];
                rehash_input.shader_hashes[2 * i + 1] = shader_headers[i]->hash[1];
                name_buffer[name_offset++]            = shader_stage_names[i];
            }
        }

        // multiview state
        rehash_input.multiview_mask = desc->render_attachments.view_mask;

        // Hash pso stream
        wis::span<const uint8_t> pso_stream_bytes{
            // start after bytecodes
            reinterpret_cast<const uint8_t*>(&stream.flags),
            // end at the end of the struct
            reinterpret_cast<const uint8_t*>(&stream + 1)
        };
        XXH128_hash_t stream_hash = XXH3_128bits(pso_stream_bytes.data(), pso_stream_bytes.size());
        rehash_input.pso_hash[0]  = stream_hash.low64;
        rehash_input.pso_hash[1]  = stream_hash.high64;

        // Rehash the combined data to get a final hash for the pipeline state
        XXH128_hash_t pso_hash = XXH3_128bits(&rehash_input, sizeof(rehash_input));

        // convert hash to hex string for use as pipeline cache key
        wis::format_to(name_buffer + name_offset, L"PSO_{:016x}{:016x}", pso_hash.low64, pso_hash.high64);

        // Try to load pipeline from cache first if available
        HRESULT hr = cache->LoadPipeline(name_buffer, &psstream_desc, IID_ID3D12PipelineState, pipeline_state.put_void_unchecked());
        if (wis::detail::succeeded(hr)) {
            auto& pipeline_impl = *new (pipeline) wis::impl::DX12PipelineImpl{
                .pipeline_state = pipeline_state.detach(),
            };
            return wis::detail::dx_success;
        }

        // Cache miss
        if (desc->flags & WisPipelineFlagsFailOnCacheMiss) {
            return wis::detail::make_result<wis::detail::Func(), "Pipeline not found in cache and creation is set to fail on cache miss">(WisStatusError, E_FAIL);
        }
    }

    HRESULT hr = device.device->CreatePipelineState(&psstream_desc, IID_ID3D12PipelineState, pipeline_state.put_void_unchecked());
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create graphics pipeline state object">(hr);
    }

    if (cache) {
        // Store pipeline in cache for future reuse
        cache->StorePipeline(name_buffer, pipeline_state.get());
    }

    auto& pipeline_impl = *new (pipeline) wis::impl::DX12PipelineImpl{
        .pipeline_state = pipeline_state.detach(),
    };

    return wis::detail::dx_success;
}

#endif // WIS_DX12_DEVICE_CPP
